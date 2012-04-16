
#ifndef OBJECT_H_
#define OBJECT_H_

#include <list>
#include <engine/vector3D.h>

namespace engine {

class ObjectContainer;

class Object {
public:
    Object() : parent_(NULL) {}
    virtual ~Object() {}

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;

	void ReparentTo(ObjectContainer* new_parent);

	Vector3D position() { return position_; }
	void set_position(Vector3D& pos) { position_ = pos; }

protected:
	ObjectContainer* parent_;
	Vector3D position_;
};

}
#endif
