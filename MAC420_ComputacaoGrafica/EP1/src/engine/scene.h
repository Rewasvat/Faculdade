#ifndef SCENE_H_
#define SCENE_H_

namespace engine {

class Scene {
public:
	Scene();
	~Scene();

	void Update(double dt);
	void Render();
};

}
#endif