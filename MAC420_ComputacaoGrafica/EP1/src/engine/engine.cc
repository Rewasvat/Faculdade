#include <cstdlib>
#include <cstdio>
#include "engine.h"
#include <GL/glut.h>
#include <engine/engineconfig.h>
#include <engine/inputmanager.h>
#include <stdlib.h>
#include <engine/scene.h>

namespace engine {

Engine* Engine::reference_ = NULL;
ExitCallbackFunc Engine::custom_exit_callback_ = NULL;

Engine::Engine() : width_(0), height_(0), config_(NULL) {
	input_manager_ = new InputManager;
}

Engine::~Engine() {
	SceneList::iterator it;
	for (it = scenes_.begin(); it != scenes_.end(); ++it)
		delete *it;

	delete input_manager_;
	delete config_;
}


void Engine::Initialize(int argc, char* argv[]) {
	if (config_ == NULL)
		config_ = new EngineConfig;

	atexit(Engine::exitCallback);
	glutInit(&argc, argv);
	glutInitDisplayMode( config_->display_mode() );
	glutInitWindowSize( config_->width() , config_->height() );
	glutCreateWindow( config_->window_name().c_str() );

	glutDisplayFunc(Engine::renderCallback);
	glutReshapeFunc(Engine::reshapeCallback);
	glutVisibilityFunc(Engine::visibilityCallback);
	glutIdleFunc(Engine::idleCallback);
    glClearColor(0.0, 0.0, 0.0, 0.0); //transparent, I hope =P
    glEnable(GL_DEPTH_TEST);
    
	input_manager_->Initialize();
}

void Engine::Run() {
	clock_.Start();
	glutMainLoop();
}

void Engine::Update() {
	clock_.Stop();
	if (window_state_ == GLUT_VISIBLE) {
		double dt = clock_.elapsed_time_secs();
		
		DeleteFinishedScenes();

		input_manager_->Update();

		if (scenes_.size() > 0) {
		    scenes_.back()->Update(dt);
			glutPostRedisplay();
	    }
	    else {
	        printf("[Engine] No Scenes to display, exiting...\n");
	        /*TODO: check onexit(...) function to register exit callbacks to free memory and stuff*/
	        exit(0);
	    }
	}
	clock_.Start();
}

void Engine::Render() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
	SceneList::iterator it;
	for (it = scenes_.begin(); it != scenes_.end(); ++it)
		(*it)->Render();

	glutSwapBuffers();
}

void Engine::WindowReshape(int w, int h) {
	width_ = w;
	height_ = h;
	glViewport(0, 0, (GLint)w, (GLint)h); 
}

void Engine::PushScene(Scene* scene) {
	scenes_.push_back(scene);
	scene->Start();
}

void Engine::RemoveScene(Scene* scene) {
    scenes_.remove(scene);
}

void Engine::PopScene() {
	if(!scenes_.empty()) scenes_.pop_back();
}

void Engine::DeleteFinishedScenes() {
    SceneList to_delete;
    SceneList::iterator it;
    Scene* scene;
    for(it = scenes_.begin(); it != scenes_.end(); ++it) {
        scene = (*it);
        if(scene->finished())
            to_delete.push_front(scene);
    }

    for(it = to_delete.begin(); it != to_delete.end(); ++it) {
        scene = (*it);
        delete scene;
        scenes_.remove(scene);
    }
}


}
