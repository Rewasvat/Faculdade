#ifndef MUNDOIME_H_
#define MUNDOIME_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>
#include <engine/vector3D.h>

namespace Obj {
class File;
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
	virtual void End();

    virtual void MouseHandler(int btn, int state, int x, int y);
	virtual void MouseMotionHandler(int btn, int dx, int dy);
	virtual void KeyboardHandler(unsigned char key, int x, int y);

protected:
	bool paused_;
	
	Obj::File* ime_file_;
	Obj::File* player_file_;
	objects::Player* player_;
	objects::Sun* sun_;
};

}
#endif
