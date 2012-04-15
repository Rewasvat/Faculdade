
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
	virtual ~ObjectContainer() {
		ChildList to_delete;
		for(ChildList::iterator it = childs_.begin(); it != childs_.end(); ++it)
			to_delete.push_front(*it);

		for(ChildList::iterator it = to_delete.begin(); it != to_delete.end(); ++it) {
			delete (*it);
			childs_.remove(*it);
		}

		childs_.clear();
	}

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

	/* If needed, this should return the Position of this entity in the 3D space,
	   as given to the childs.*/
	virtual Vector3D parent_position() { return Vector3D(); }

protected:
	ChildList childs_;

	void AddChild(Object* obj) { childs_.push_back(obj); }
 	void RemoveChild(Object* obj) { childs_.remove(obj); }

	friend class Object;
};

}
#endif
