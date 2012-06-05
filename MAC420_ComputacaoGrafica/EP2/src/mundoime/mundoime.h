#ifndef MUNDOIME_H_
#define MUNDOIME_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>
#include <engine/vector3D.h>

#include <engine/objload/objload.h>
#include <vector>

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
	typedef std::vector<Obj::Surface> VertexBufferList;
	VertexBufferList models_;
};

}
#endif
