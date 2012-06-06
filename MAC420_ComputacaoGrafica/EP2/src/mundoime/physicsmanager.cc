#include "physicsmanager.h"
#include <engine/vector3D.h>
#include <btBulletDynamicsCommon.h>

namespace mundoime {

PhysicsManager* PhysicsManager::reference_ = NULL;

PhysicsManager::PhysicsManager() {
}

PhysicsManager::~PhysicsManager() {
	delete broadphase_;
    delete config_;
    delete dispatcher_;
    delete solver_;
    delete world_;
}

void PhysicsManager::Initialize() {
	// Broadphase is the initial collision detecting: checks for colliding pairs given their bounding boxes
	broadphase_ = new btDbvtBroadphase();
 
    // Set up the collision configuration and dispatcher
    config_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(config_);
 
    // The actual physics solver
    solver_ = new btSequentialImpulseConstraintSolver;
 
    // The world.
    world_ = new btDiscreteDynamicsWorld(dispatcher_,broadphase_,solver_,config_);
    world_->setGravity(btVector3(0,-10,0));
}

void PhysicsManager::AddBody(btRigidBody* body) {
    world_->addRigidBody(body);
}

void PhysicsManager::RemoveBody(btRigidBody* body) {
    world_->removeRigidBody(body);
}


}
