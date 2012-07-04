/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: physicsmanager.cc                                  **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
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
    void Update(double dt);

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
