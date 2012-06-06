#ifndef PHYSICSMANAGER_H_
#define PHYSICSMANAGER_H_


class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btRigidBody;

namespace mundoime {

class PhysicsManager {
public:
	static PhysicsManager* reference() { return reference_ ? reference_ : reference_ = new PhysicsManager; }

	~PhysicsManager();

    void Initialize();

    void AddBody(btRigidBody* body);
    void RemoveBody(btRigidBody* body);

private:
	static PhysicsManager* reference_;

	PhysicsManager();

    btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* config_;
    btCollisionDispatcher* dispatcher_;
    btSequentialImpulseConstraintSolver* solver_;
    btDiscreteDynamicsWorld* world_;
};

}
#endif
