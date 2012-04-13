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

Engine::Engine() : config_(NULL) {
	input_manager_ = new InputManager;
}

Engine::~Engine() {
	SceneList::iterator it;
	for (it = scenes_.begin(); it != scenes_.end(); ++it)
		delete *it;

	delete input_manager_;
}

void reshape(int w, int h)
{
    int x = 0, y = 0;
    /*if (w > h) {
        x = (w - h) / 2;
        w = h;
    } else if (w < h) {
        y = (h - w) / 2; 
        h = w;
    }*/

	glViewport((GLint)x, (GLint)y, (GLint)w, (GLint)h); 
}

void Engine::Initialize(int argc, char* argv[]) {
	if (config_ == NULL)
		config_ = new EngineConfig;

	glutInit(&argc, argv);
	glutInitDisplayMode( config_->display_mode() );
	glutInitWindowSize( config_->width() , config_->height() );
	glutCreateWindow( config_->window_name().c_str() );

	glutDisplayFunc(Engine::renderCallback);
    glutReshapeFunc(reshape);
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

		if (scenes_.size() > 0) {
		    scenes_.back()->Update(dt);
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
