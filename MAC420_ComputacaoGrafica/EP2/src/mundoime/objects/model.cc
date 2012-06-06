#include <mundoime/objects/model.h>
#include <engine/objload/objload.h>
#include <math.h>
#include <GL/glut.h>
#include <btBulletDynamicsCommon.h>
#include <mundoime/physicsmanager.h>
#include <cstdlib>

#define PI 3.14159265358979323846

namespace mundoime {
namespace objects {

Model::Model(engine::Vector3D& pos, engine::Vector3D& direction, Obj::VertexBuffer& mesh, double mass, btCollisionShape* shape) : mass_(mass) {
	position_ = pos;
	direction_ = direction;
	direction_.Normalize();
    mesh_ = mesh;
    
    if (shape != NULL) {
        shape_ = shape;
        btTIVA_ = NULL;
    }
    else {
        btTIVA_ = new btTriangleIndexVertexArray(   mesh.m_Indices.size(),               /*number of triangles in mesh*/
                                                    (int*) &mesh.m_Indices[0],           /*pointer to first element in index array*/
                                                    sizeof(unsigned int),                /*number of bytes in each index element*/
                                                    mesh.m_Vertices.size(),              /*number of vertices in mesh*/
                                                    (btScalar*) &mesh.m_Vertices[0].x,   /*pointer to first element in vertex array*/
                                                    3*sizeof(float));                    /*number of bytes in each vertex element*/

	    bool useQuantizedAabbCompression = true;
	    shape_ = new btBvhTriangleMeshShape(btTIVA_,useQuantizedAabbCompression);
    }
    
    //transform(rotation, translation)
    engine::Vector3D z_axis (0.0, 0.0, 1.0);
    double angle = acos( direction_ * z_axis );
    engine::Vector3D rot_axis(0.0, 0.0, 0.0);
    if ( direction_.Length() > 0.0 && fabs(angle) > 0.0 ) {
        rot_axis = direction_.CrossProduct(z_axis);
        rot_axis.Normalize();
        rot_axis.Scale(-1.0);
    }
    btQuaternion rotation(btVector3(rot_axis.x, rot_axis.y, rot_axis.z), angle);
    
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation,btVector3(pos.x, pos.y, pos.z)));
    btVector3 inertia(0,0,0);
    if (mass_ > 0.0)
        shape_->calculateLocalInertia(mass_,inertia);
    btRigidBody::btRigidBodyConstructionInfo  bodyInfo(mass_,motionState,shape_,inertia);
    body_ = new btRigidBody(bodyInfo);
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
}

void Model::Render() {
    glTranslated(position_.x, position_.y, position_.z);
    
	engine::Vector3D z_axis (0.0, 0.0, 1.0);
    double angle = acos( direction_ * z_axis );
    if ( direction_.Length() > 0.0 && fabs(angle) > 0.0 ) {
        engine::Vector3D rot_axis = direction_.CrossProduct(z_axis);
        rot_axis.Normalize();
        rot_axis.Scale(-1.0);
        double angle_deg = (180.0 * angle) / PI;
        glRotated(angle_deg, rot_axis.x, rot_axis.y, rot_axis.z);
    }
    
    mesh_.gl();
}

}
}
