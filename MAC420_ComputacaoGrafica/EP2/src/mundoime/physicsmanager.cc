#include "physicsmanager.h"
#include <engine/vector3D.h>
#include <btBulletDynamicsCommon.h>
#include <cstdlib>

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

void teste() {
        btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
 
        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
 
        dynamicsWorld->setGravity(btVector3(0,-10,0));
 
 
        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
 
        btCollisionShape* fallShape = new btSphereShape(1);
 
 
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
        btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
        dynamicsWorld->addRigidBody(groundRigidBody);
 
 
        btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
        btScalar mass = 1;
        btVector3 fallInertia(0,0,0);
        fallShape->calculateLocalInertia(mass,fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
        btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
        dynamicsWorld->addRigidBody(fallRigidBody);
 
 
        for (int i=0 ; i<300 ; i++) {
                dynamicsWorld->stepSimulation(1/60.f,10);
 
                btTransform trans;
                fallRigidBody->getMotionState()->getWorldTransform(trans);
 
                //std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
                printf("Sphere Height: %f\n", trans.getOrigin().getY());
        }
 
        dynamicsWorld->removeRigidBody(fallRigidBody);
        delete fallRigidBody->getMotionState();
        delete fallRigidBody;
 
        dynamicsWorld->removeRigidBody(groundRigidBody);
        delete groundRigidBody->getMotionState();
        delete groundRigidBody;
 
 
        delete fallShape;
 
        delete groundShape;
 
 
        delete dynamicsWorld;
        delete solver;
        delete collisionConfiguration;
        delete dispatcher;
        delete broadphase;
}

void PhysicsManager::Initialize() {
    //teste();
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

	btContactSolverInfo& info = world_->getSolverInfo();
	info.m_splitImpulse = 1; //enable split impulse feature
}

void PhysicsManager::Update(double dt) {
    world_->stepSimulation(dt, 5);
}

void PhysicsManager::AddBody(btRigidBody* body) {
    world_->addRigidBody(body);
}

void PhysicsManager::RemoveBody(btRigidBody* body) {
    world_->removeRigidBody(body);
}


}
