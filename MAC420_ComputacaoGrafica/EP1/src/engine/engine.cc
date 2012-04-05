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
    if (w > h) {
        x = (w - h) / 2;
        w = h;
    } else if (w < h) {
        y = (h - w) / 2; 
        h = w;
    }

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

		SceneList::iterator it;
		for (it = scenes_.begin(); it != scenes_.end(); ++it)
			(*it)->Update(dt);
	}
	clock_.Start();
}

void Engine::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
    
	SceneList::iterator it;
	for (it = scenes_.begin(); it != scenes_.end(); ++it)
		(*it)->Render();

	glutSwapBuffers();
}


void Engine::PushScene(Scene* scene) {
	scenes_.push_back(scene);
}

void Engine::PopScene() {
	if(!scenes_.empty()) scenes_.pop_back();
}

}
