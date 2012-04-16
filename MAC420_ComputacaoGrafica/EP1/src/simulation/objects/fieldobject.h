#ifndef SPHERE_H_
#define SPHERE_H_

#include <engine/object.h>
#include <engine/objectcontainer.h>
#include <engine/eventhandler.h>

namespace simulation {
namespace objects {

class FieldObject : public engine::Object, public engine::ObjectContainer, public engine::EventHandler {
public:
	FieldObject();
	~FieldObject();

	void Update(double dt);
	void Render();

	void MouseHandler(int btn, int state, int x, int y) {}
	void MouseMotionHandler(int btn, int dx, int dy) {}
	void KeyboardHandler(unsigned char key, int x, int y);

protected:
	bool visible_;
};

}
}
#endif
