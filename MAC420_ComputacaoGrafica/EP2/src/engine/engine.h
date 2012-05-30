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
typedef void (*ExitCallbackFunc)(void);

class Engine {
public:
	static Engine* reference() { return reference_ ? reference_ : reference_ = new Engine; }

	~Engine();

	void Configure(EngineConfig* config) { config_ = config; }

	void Initialize(int argc, char* argv[]);
	void Run();

	void Update();
	void Render();
	
	void WindowReshape(int w, int h);

	void PushScene(Scene* scene);
	void RemoveScene(Scene* scene);
	void PopScene();

	InputManager* input_manager() { return input_manager_; }

	int window_width() { return width_; }
	int window_height() { return height_; }

	static void RegisterCustomExitCallback( ExitCallbackFunc callback) { custom_exit_callback_ = callback; }

private:
	static Engine* reference_;
	static ExitCallbackFunc custom_exit_callback_;

	int width_;
	int height_;
	EngineConfig* config_;
	SceneList scenes_;
	InputManager* input_manager_;

	int window_state_;
	Timer clock_;
	
	Engine();

	void DeleteFinishedScenes();

	static void visibilityCallback(int state) { reference_->window_state_ = state; }
	static void idleCallback() { reference_->Update(); }
	static void renderCallback() { reference_->Render(); }
	static void reshapeCallback(int w, int h) { reference_->WindowReshape(w,h); }
	static void exitCallback() { 
		if (Engine::custom_exit_callback_ != 0)
			custom_exit_callback_();
		delete reference_;
		reference_ = 0;
	}
};

}
#endif
