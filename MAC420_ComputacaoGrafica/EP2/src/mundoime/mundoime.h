#ifndef MUNDOIME_H_
#define MUNDOIME_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>
#include <engine/vector3D.h>

namespace Obj {
class File;
}

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace mundoime {


class MundoIME : public engine::Scene, public engine::EventHandler {
public:
    MundoIME();
    ~MundoIME();
    
    virtual void Start();
	virtual void Update(double dt);
	virtual void Render();
	virtual void End();

    virtual void MouseHandler(int btn, int state, int x, int y);
	virtual void MouseMotionHandler(int btn, int dx, int dy);
	virtual void KeyboardHandler(unsigned char key, int x, int y);

protected:
	bool paused_;

	double azimuth_;
	double elevation_;
	double zoom_;
	double camera_distance_;
	engine::Vector3D center_;

	Obj::File* file_;
	
	btBroadphaseInterface* broadphase_;
    btDefaultCollisionConfiguration* config_;
    btCollisionDispatcher* dispatcher_;
    btSequentialImpulseConstraintSolver* solver_;
    btDiscreteDynamicsWorld* world_;
};

}
#endif
