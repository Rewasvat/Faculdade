
#ifndef OBJECT_H_
#define OBJECT_H_

#include <list>
#include <engine/vector3D.h>

namespace engine {

class ObjectContainer;
class Light;

class Object {
public:
    Object() : parent_(NULL), casts_shadow_(false) {}
    virtual ~Object() {}

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void RenderShadow(Light* light) { }

	void ReparentTo(ObjectContainer* new_parent);

	bool casts_shadow() { return casts_shadow_; }
	void set_casts_shadow(bool casts_shadow) { casts_shadow_ = casts_shadow; }

	virtual Vector3D position() { return position_; }
	virtual void set_position(Vector3D& pos) { position_ = pos; }

protected:
	ObjectContainer* parent_;
	bool casts_shadow_;
	Vector3D position_;
};

}
#endif
