#include "fieldobject.h"

namespace simulation {
namespace objects {

FieldObject::FieldObject() {
	handles_mouse_ = false;
}

FieldObject::~FieldObject() {
}

void FieldObject::Update(double dt) {
	if (visible_) {
		this->UpdateChilds(dt);
	}
}

void FieldObject::Render() {
	if (visible_) {
		this->RenderChilds();
	}
}

void FieldObject::KeyboardHandler(unsigned char key, int x, int y) {
	if (key == 'v')
		visible_ = !visible_;
}

}
}