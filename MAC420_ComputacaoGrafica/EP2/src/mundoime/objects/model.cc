#include <mundoime/objects/model.h>
#include <engine/objload/objload.h>
#include <engine/light.h>
#include <math.h>
#include <GL/glut.h>
#include <btBulletDynamicsCommon.h>
#include <BulletWorldImporter/btBulletWorldImporter.h>
#include <mundoime/physicsmanager.h>
#include <cstdlib>
#include <string>

#define PI 3.14159265358979323846

namespace mundoime {
namespace objects {

Model::Model(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape) : mass_(mass) {
	position_ = pos;
	direction_ = direction;
	direction_.Normalize();
    mesh_ = mesh;
	mesh_.SetConnectivity();
	mesh_.CalculatePlanes();
	mesh_visible_ = true;
	casts_shadow_ = true;
    
	btTIVA_ = NULL;
    if (shape != NULL) {
        shape_ = shape;
    }
    else {
		btTIVA_ = new btTriangleIndexVertexArray(   mesh_.m_Indices.size()/3,               /*number of triangles in mesh*/
													(int*) &mesh_.m_Indices[0],           /*pointer to first element in index array*/
													3*sizeof(unsigned int),                /*number of bytes in each index element*/
													mesh_.m_Vertices.size(),              /*number of vertices in mesh*/
													(btScalar*) &mesh_.m_Vertices[0].x,   /*pointer to first element in vertex array*/
													3*sizeof(float));                    /*number of bytes in each vertex element*/
		bool useQuantizedAabbCompression = true;
		shape_ = new btBvhTriangleMeshShape(btTIVA_,useQuantizedAabbCompression);
		printf("[Model] created Bullet triangle mesh collision shape for model %s\n", mesh.name.c_str());
		
    }
    
    //transform(rotation, translation)
    engine::Vector3D z_axis (0.0, 0.0, 1.0);
    engine::Vector3D rot_axis(0.0, 0.0, 1.0);
    double angle = 0.0;
    if (direction_.Length() > 0.0) {
        angle = acos( direction_ * z_axis );
        if ( fabs(angle) > 0.0 ) {
            rot_axis = direction_.CrossProduct(z_axis);
            rot_axis.Normalize();
            rot_axis.Scale(-1.0);
        }
    }
    rot_axis_ = rot_axis;
    rot_angle_ = angle;
    
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(btVector3(rot_axis.x, rot_axis.y, rot_axis.z), angle),
                                                                             btVector3(position_.x, position_.y, position_.z)));
    btVector3 inertia(0,0,0);
    if (mass_ > 0.0)
        shape_->calculateLocalInertia(mass_,inertia);
    btRigidBody::btRigidBodyConstructionInfo  bodyInfo(mass_,motionState,shape_,inertia);
    body_ = new btRigidBody(bodyInfo);
    body_->setLinearFactor(btVector3(1,1,1));
	body_->setActivationState(DISABLE_DEACTIVATION);
}

Model::~Model() {
    PhysicsManager::reference()->RemoveBody(body_);
    delete body_->getMotionState();
    delete body_;
    if (btTIVA_)
        delete btTIVA_;
    delete shape_;
}

void Model::Update(double dt) {
    //TODO: update pos and dir according to physics
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);
    //btDefaultMotionState* motionState = body_->getMotionState();
    //printf("Model %s Height: %f\n", mesh_.name.c_str(), t.getOrigin().getY());

    btVector3 pos = t.getOrigin();
    position_.x = pos.x();
    position_.y = pos.y();
    position_.z = pos.z();
    
    //printf("Model %s position=(%f,%f,%f)\n", mesh_.name.c_str(), position_.x, position_.y, position_.z);
    
    btQuaternion rot = t.getRotation();
    rot_angle_ = rot.getAngle();
    rot_axis_.x = rot.getAxis().x();
    rot_axis_.y = rot.getAxis().y();
    rot_axis_.z = rot.getAxis().z();
}

void Model::Render() {
	if (mesh_visible_) {
		glTranslated(position_.x, position_.y, position_.z);
    
		double angle_deg = (180.0 * rot_angle_) / PI;
		glRotated(angle_deg, rot_axis_.x, rot_axis_.y, rot_axis_.z);    

		mesh_.gl();

		/*glDisable(GL_LIGHTING);
		int i;
		glColor4f(0.0,1.0,0.0,0.6);
		glBegin(GL_TRIANGLES);
		for (i=0; i<frontFaceVertices.size(); i++) {
			glVertex3dv(frontFaceVertices[i].val);
		}
		glEnd();
		glColor4f(1.0,0.0,0.0,0.6);
		glBegin(GL_TRIANGLES);
		for (i=0; i<backFaceVertices.size(); i++) {
		//for (i=backFaceVertices.size()-1; i>=0; i--) {
			glVertex3dv(backFaceVertices[i].val);
		}
		glEnd();

		for (i=0; i<prisms.size(); i++) {
			prisms[i].render();
		}

		glEnable(GL_LIGHTING);*/

		
	}
}

void Model::RenderShadowXXX(engine::Light* light) {
	glPushAttrib(GL_ENABLE_BIT);
 	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LESS);

	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glEnable(GL_CULL_FACE);
	/*******
	// for shadow volumes that the camera is not inside
	glCullFace(GL_BACK);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	RenderShadowVolume(light);

	// second pass, stencil operation increases stencil value
	glCullFace(GL_FRONT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	RenderShadowVolume(light);
	/*******/
	// for shadow volumes with the camera inside
	glCullFace(GL_FRONT);
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	//RenderShadowVolume(light);

	glCullFace(GL_BACK);
	glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
	//RenderShadowVolume(light);
	/*******/

	//glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glColorMask(1, 1, 1, 1);

	//draw a shadowing rectangle covering the entire screen
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glStencilFunc(GL_NOEQUAL, 0, 0xffffffff);
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f,-0.10f);
		glVertex3f(-0.1f,-0.1f,-0.10f);
		glVertex3f( 0.1f, 0.1f,-0.10f);
		glVertex3f( 0.1f,-0.1f,-0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glShadeModel(GL_SMOOTH);

	glPopAttrib();
}
void Model::RenderShadow(engine::Light* light) {
	int i, j, k, jj;
	unsigned int	p1, p2;
	engine::Vector3D v1, v2, v3;
	engine::Vector3D& lPos = light->position();

	//set visual parameter - checks visible faces.
	//this function is called 2 times for each light, taking this computation out of here
	//so that it executes only once for each light will improve performance;
	for (i=0; i < mesh_.m_Triangles.size(); i++){
		// chech to see if light is in front or behind the plane (face plane)
		double side = mesh_.m_Triangles[i].planeEquation.a * lPos.x +
					  mesh_.m_Triangles[i].planeEquation.b * lPos.y +
					  mesh_.m_Triangles[i].planeEquation.c * lPos.z +
					  mesh_.m_Triangles[i].planeEquation.d;  // * light pos W coord ?
		if (side > 0) mesh_.m_Triangles[i].visible = true;
		else mesh_.m_Triangles[i].visible = false;
	}

	glTranslated(position_.x, position_.y, position_.z);

	frontFaceVertices.clear();
	backFaceVertices.clear();
	/**********
	//render shadow volume
	for (i=0; i < mesh_.m_Triangles.size(); i++){
		if (mesh_.m_Triangles[i].visible)
			for (j=0; j<3; j++) {
				k = mesh_.m_Triangles[i].neighbourIndices[j];
				if ((k == -1) || (!mesh_.m_Triangles[k].visible)){
					// here we have an edge, we must draw a polygon
					p1 = mesh_.m_Triangles[i].v[j];
					jj = (j+1)%3;
					p2 = mesh_.m_Triangles[i].v[jj];

					engine::Vector3D meshV1 (mesh_.m_Vertices[p1].x, mesh_.m_Vertices[p1].y, mesh_.m_Vertices[p1].z);
					engine::Vector3D meshV2 (mesh_.m_Vertices[p2].x, mesh_.m_Vertices[p2].y, mesh_.m_Vertices[p2].z);
					engine::Vector3D meshV3 (mesh_.m_Vertices[(j+2)%3].x, mesh_.m_Vertices[(j+2)%3].y, mesh_.m_Vertices[(j+2)%3].z);
					//calculate the length of the vector
					double length = 1.0;
					v1 = meshV1 - (lPos+position_);
					v1.Scale(length);

					v2 = meshV2 - (lPos+position_);
					v2.Scale(length);

					v3 = meshV3 - (lPos+position_);
					v3.Scale(length);

					engine::Vector3D offset = light->direction();
					offset.Normalize();
					offset.Scale(-1);

					frontFaceVertices.push_back(meshV1+offset);
					frontFaceVertices.push_back(meshV2+offset);
					frontFaceVertices.push_back(meshV3+offset);
					backFaceVertices.push_back(meshV1 + v1);
					backFaceVertices.push_back(meshV2 + v2);
					backFaceVertices.push_back(meshV3 + v3);

					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
						glVertex3dv( (meshV1+offset).val );
						glVertex3dv( (meshV1+v1).val );

						glVertex3dv( (meshV2+offset).val );
						glVertex3dv( (meshV2+v2).val );
					glEnd();
				}
			}
	}
	glBegin(GL_TRIANGLES);
	for (i=0; i<frontFaceVertices.size(); i++) {
		glVertex3dv(frontFaceVertices[i].val);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	//for (i=0; i<backFaceVertices.size(); i++) {
	for (i=backFaceVertices.size()-1; i>=0; i--) {
		glVertex3dv(backFaceVertices[i].val);
	}
	glEnd();
	/***********/
	//prisms.clear();
	engine::Vector3D lDir = light->direction();
	for (i=0; i < mesh_.m_Triangles.size(); i++){
		if (mesh_.m_Triangles[i].visible) {
			engine::Vector3D meshV1 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].z);
			engine::Vector3D meshV2 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].z);
			engine::Vector3D meshV3 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].z);

			double length = 100.0;
			v1 = meshV1 - (lPos+position_);
			v1.Scale(length);
			v2 = meshV2 - (lPos+position_);
			v2.Scale(length);
			v3 = meshV3 - (lPos+position_);
			v3.Scale(length);

			/*lDir.Normalize();
			lDir.Scale(-1);
			meshV1 = meshV1 + lDir;
			meshV2 = meshV2 + lDir;
			meshV3 = meshV3 + lDir;*/

			v1 = meshV1 + v1;
			v2 = meshV2 + v2;
			v3 = meshV3 + v3;
			//v1 = meshV1 + lDir;
			//v2 = meshV2 + lDir;
			//v3 = meshV3 + lDir;

			/*Prism pm = Prism();
			pm.p1 = (meshV1);
			pm.p2 = (meshV2);
			pm.p3 = (meshV3);
			pm.p4 = (v3);
			pm.p5 = (meshV1);
			pm.p6 = (v1);
			pm.p7 = (meshV2);
			pm.p8 = (v2);
			pm.p9 = (v3);
			pm.p10 = (v1);
			prisms.push_back(pm);*/

			bool se[3];
			for (j=0; j<3; j++) {
				se[j]=false;
				k = mesh_.m_Triangles[i].neighbourIndices[j];
				if ((k == -1) || (!mesh_.m_Triangles[k].visible)){
					se[j] = true;
				}
			}

			glBegin(GL_TRIANGLES);
			glVertex3dv(meshV1.val);
			glVertex3dv(meshV2.val);
			glVertex3dv(meshV3.val);
			glVertex3dv(v3.val);
			glVertex3dv(v2.val);
			glVertex3dv(v1.val);

			if (se[0]) {
				glVertex3dv(v1.val);
				glVertex3dv(meshV2.val);
				glVertex3dv(meshV1.val);

				glVertex3dv(v2.val);
				glVertex3dv(meshV2.val);
				glVertex3dv(v1.val);
			}
			if (se[1]) {
				glVertex3dv(v2.val);
				glVertex3dv(meshV3.val);
				glVertex3dv(meshV2.val);

				glVertex3dv(v3.val);
				glVertex3dv(meshV3.val);
				glVertex3dv(v2.val);
			}
			if (se[2]) {
				glVertex3dv(v3.val);
				glVertex3dv(meshV1.val);
				glVertex3dv(meshV3.val);
				
				glVertex3dv(v1.val);
				glVertex3dv(meshV1.val);
				glVertex3dv(v3.val);
			}
			glEnd();
			/*glBegin(GL_TRIANGLE_STRIP);
				glVertex3dv(meshV1.val);
				glVertex3dv(meshV2.val);
				glVertex3dv(meshV3.val);
				glVertex3dv(v3.val);
				glVertex3dv(meshV1.val);
				glVertex3dv(v1.val);
				glVertex3dv(meshV2.val);
				glVertex3dv(v2.val);
				glVertex3dv(v3.val);
				glVertex3dv(v1.val);
			glEnd();*/

		}
	}
	/*******/
}
void Model::RenderVisibleFaces(engine::Light* light, bool visible) {
	int i, j, k, jj;
	unsigned int	p1, p2;
	engine::Vector3D v1, v2, v3;
	engine::Vector3D& lPos = light->position();

	//set visual parameter - checks visible faces.
	//this function is called 2 times for each light, taking this computation out of here
	//so that it executes only once for each light will improve performance;
	for (i=0; i < mesh_.m_Triangles.size(); i++){
		// chech to see if light is in front or behind the plane (face plane)
		double side = mesh_.m_Triangles[i].planeEquation.a * lPos.x +
					  mesh_.m_Triangles[i].planeEquation.b * lPos.y +
					  mesh_.m_Triangles[i].planeEquation.c * lPos.z +
					  mesh_.m_Triangles[i].planeEquation.d;  // * light pos W coord ?
		if (side > 0) mesh_.m_Triangles[i].visible = true;
		else mesh_.m_Triangles[i].visible = false;
	}

	glTranslated(position_.x, position_.y, position_.z);

	//render shadow volume
	for (i=0; i < mesh_.m_Triangles.size(); i++){
		if (mesh_.m_Triangles[i].visible == visible) {
			engine::Vector3D meshV1 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].z);
			engine::Vector3D meshV2 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].z);
			engine::Vector3D meshV3 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].z);

			glColor4f(0.1,0.8,0.8,0.5);
			glBegin(GL_TRIANGLES);
				glVertex3dv(meshV1.val);
				glVertex3dv(meshV2.val);
				glVertex3dv(meshV3.val);
			glEnd();
		}
	}
}


void Model::set_position(engine::Vector3D& pos) {
	position_ = pos;

	btTransform t;
    body_->getMotionState()->getWorldTransform(t);
	t.setOrigin( btVector3(pos.x, pos.y, pos.z) );

	body_->getMotionState()->setWorldTransform(t);
}

void Prism::render() {
	return;
	glBegin(GL_TRIANGLE_STRIP);
		glColor4f(0.0,1.0,0.0,0.6);
		glVertex3dv(p1.val);
		glVertex3dv(p2.val);
		glVertex3dv(p3.val);

		glColor4f(1.0,0.0,0.0,0.6);
		glVertex3dv(p4.val);

		glColor4f(0.0,1.0,0.0,0.6);
		glVertex3dv(p5.val);

		glColor4f(1.0,0.0,0.0,0.6);
		glVertex3dv(p6.val);

		glColor4f(0.0,1.0,0.0,0.6);
		glVertex3dv(p7.val);

		glColor4f(1.0,0.0,0.0,0.6);
		glVertex3dv(p8.val);
		glVertex3dv(p9.val);
		glVertex3dv(p10.val);
	glEnd();
}

}
}
