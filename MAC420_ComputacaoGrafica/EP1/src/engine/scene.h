#ifndef SCENE_H_
#define SCENE_H_

#include <list>

namespace engine {

typedef std::list<Object*> ObjectList;

class Scene {
public:
	Scene();
	~Scene();

	void Update(double dt);
	void Render();

	void AddObject(Object* obj);
	void RemoveObject(Object* obj);

	bool is_visible() { return is_visible_; }
	void set_visible(bool visible) { is_visible_ = visible; }

protected:
	ObjectList objects_;
	bool is_visible_;
};

}
#endif