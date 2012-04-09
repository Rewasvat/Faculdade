#ifndef ENGINE_H_
#define ENGINE_H_

#include <string>
#include <list>
#include <engine/timer.h>

namespace engine {

class Scene;
class EngineConfig;
class InputManager;

typedef std::list<Scene*> SceneList;

class Engine {
public:
	static Engine* reference() { return reference_ ? reference_ : reference_ = new Engine; }

	Engine();
	~Engine();

	void Configure(EngineConfig* config) { config_ = config; }

	void Initialize(int argc, char* argv[]);
	void Run();

	void Update();
	void Render();

	void PushScene(Scene* scene);
	void RemoveScene(Scene* scene);
	void PopScene();

	InputManager* input_manager() { return input_manager_; }

private:
	static Engine* reference_;

	EngineConfig* config_;
	SceneList scenes_;
	InputManager* input_manager_;

	int window_state_;
	Timer clock_;
	
	void DeleteFinishedScenes();

	static void visibilityCallback(int state) { reference_->window_state_ = state; }
	static void idleCallback() { reference_->Update(); }
	static void renderCallback() { reference_->Render(); }
};

}
#endif
