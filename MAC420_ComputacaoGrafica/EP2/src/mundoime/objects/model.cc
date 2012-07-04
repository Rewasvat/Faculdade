/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: model.cc                                           **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
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
	}
}

void Model::RenderShadow(engine::Light* light) {
	unsigned int i, j;//, jj;
    int k;
	//unsigned int	p1, p2;
	engine::Vector3D v1, v2, v3;
	engine::Vector3D lPos = light->position();

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

	engine::Vector3D lDir = light->direction();
	for (i=0; i < mesh_.m_Triangles.size(); i++){
		if (mesh_.m_Triangles[i].visible) {
			engine::Vector3D meshV1 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[0]].z);
			engine::Vector3D meshV2 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[1]].z);
			engine::Vector3D meshV3 (mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].x, mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].y, mesh_.m_Vertices[mesh_.m_Triangles[i].v[2]].z);

			double length = 10000.0;
			v1 = meshV1 - (lPos+position_);
			v1.Scale(length);
			v2 = meshV2 - (lPos+position_);
			v2.Scale(length);
			v3 = meshV3 - (lPos+position_);
			v3.Scale(length);

			v1 = meshV1 + v1;
			v2 = meshV2 + v2;
			v3 = meshV3 + v3;

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

}
}
