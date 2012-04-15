#include <engine/object.h>
#include <engine/objectcontainer.h>
#include <cstdlib>

namespace engine {

void Object::ReparentTo(ObjectContainer* new_parent) {
	if (parent_ != NULL) {
		parent_->RemoveChild(this);
	}

	parent_ = new_parent;
	if (parent_ != NULL) {
		parent_->AddChild(this);
	}
}

}
