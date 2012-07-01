#ifndef MUNDOIME_H_
#define MUNDOIME_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>
#include <engine/vector3D.h>

namespace Obj {
class File;
}

namespace engine {
class Light;
}

namespace mundoime {

namespace objects {
class Player;
class Sun;
}

class MundoIME : public engine::Scene, public engine::EventHandler {
public:
    MundoIME();
    ~MundoIME();
    
    virtual void Start();
	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderShadows(engine::Light* light);
	virtual void End();
	virtual void WindowResize(int w, int h);

    virtual void MouseHandler(int btn, int state, int x, int y);
	virtual void MouseMotionHandler(int btn, int dx, int dy);
	virtual void KeyboardHandler(unsigned char key, int x, int y);

    objects::Sun* sun () { return sun_; }

protected:
	Obj::File* ime_file_;
	Obj::File* player_file_;
	objects::Player* player_;
	objects::Sun* sun_;
	char draw_str[64];

	void setPerspective(double w, double h);
};


double NormalDistribution(double x, double mean, double variance);

}
#endif
