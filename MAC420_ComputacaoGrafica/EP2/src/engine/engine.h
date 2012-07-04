/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: engine.h                                           **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#ifndef ENGINE_H_
#define ENGINE_H_

#include <string>
#include <list>
#include <engine/timer.h>
#include <engine/color.h>

namespace engine {

class Scene;
class Light;
class EngineConfig;
class InputManager;

typedef std::list<Scene*> SceneList;
typedef std::list<Light*> LightList;
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

	void AddLight(Light* light) { lights_.push_back(light); }
	void RemoveLight(Light* light) { lights_.remove(light); }

    void DrawString(double x, double y, const char* str, const Color& color);

    bool fullscreen() { return fullscreen_; }
    void ToggleFullscreen();

	InputManager* input_manager() { return input_manager_; }

	int window_width() { return width_; }
	int window_height() { return height_; }
    double FPS() { return fps_; }

	static void RegisterCustomExitCallback( ExitCallbackFunc callback) { custom_exit_callback_ = callback; }

private:
	static Engine* reference_;
	static ExitCallbackFunc custom_exit_callback_;

	int width_;
	int height_;
    bool fullscreen_;
	EngineConfig* config_;
	SceneList scenes_;
	LightList lights_;
	InputManager* input_manager_;

	int window_state_;
	Timer clock_;
	
	Engine();

    double fps_;
    unsigned frames_;
    unsigned previous_;
    void calculateFPS();

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
