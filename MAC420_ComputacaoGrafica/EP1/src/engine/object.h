
#ifndef OBJECT_H_
#define OBJECT_H_

#include <list>

namespace engine {

class ObjectContainer;

class Object {
public:
    Object() {}
    virtual ~Object() {}

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;

	void ReparentTo(ObjectContainer* new_parent);

protected:
	ObjectContainer* parent_;
};

}
#endif
