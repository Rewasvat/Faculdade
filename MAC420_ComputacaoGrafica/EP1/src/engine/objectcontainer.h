
#ifndef OBJECTCONTAINER_H_
#define OBJECTCONTAINER_H_

#include <list>
#include <engine/object.h>
#include <engine/vector3D.h>

namespace engine {

typedef std::list<Object*> ChildList;

class ObjectContainer {
public:
	ObjectContainer() {}
	virtual ~ObjectContainer() {}

	void UpdateChilds(double dt) {
		ChildList::iterator it;
        for (it = childs_.begin(); it != childs_.end(); ++it) {
            Object* obj = (*it);
            obj->Update(dt);
        }
	}
	void RenderChilds() {
		ChildList::iterator it;
        for (it = childs_.begin(); it != childs_.end(); ++it) {
            Object* obj = (*it);
            obj->Render();
        }
	}

	virtual Vector3D parent_position() { return Vector3D(); }

protected:
	ChildList childs_;

	void AddChild(Object* obj) { childs_.push_back(obj); }
 	void RemoveChild(Object* obj) { childs_.remove(obj); }

	friend class Object;
};

}
#endif
