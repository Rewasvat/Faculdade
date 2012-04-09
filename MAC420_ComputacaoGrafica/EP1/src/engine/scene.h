#ifndef SCENE_H_
#define SCENE_H_

#include <list>

namespace engine {

class Object;

typedef std::list<Object*> ObjectList;

class Scene {
public:
	Scene();
	~Scene();

    virtual void Start() {}
	virtual void Update(double dt);
	virtual void Render();
	virtual void End() {};
	
	void Finish() { End(); finished_ = true; }

	virtual void AddObject(Object* obj) { objects_.push_back(obj); }
 	virtual void RemoveObject(Object* obj) { objects_.remove(obj); }

	bool is_visible() { return is_visible_; }
	void set_visible(bool visible) { is_visible_ = visible; }
	bool finished() { return finished_; }

protected:
	ObjectList objects_;
	bool is_visible_;
	bool finished_;
};

}
#endif
