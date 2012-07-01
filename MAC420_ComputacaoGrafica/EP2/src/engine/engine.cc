#include <cstdlib>
#include <cstdio>
#include "engine.h"
#include <GL/glut.h>
#include <engine/engineconfig.h>
#include <engine/inputmanager.h>
#include <stdlib.h>
#include <engine/scene.h>
#include <engine/light.h>
#include <engine/texture/texture.h>

namespace engine {

Engine* Engine::reference_ = NULL;
ExitCallbackFunc Engine::custom_exit_callback_ = NULL;

Engine::Engine() : width_(0), height_(0), config_(NULL) {
	input_manager_ = new InputManager;
    frames_ = previous_ = 0;
    fullscreen_ = false;
}

Engine::~Engine() {
	SceneList::iterator it;
	for (it = scenes_.begin(); it != scenes_.end(); ++it)
		delete *it;

	delete input_manager_;
	delete config_;

	texture::DeleteAllTextures();
}


void Engine::Initialize(int argc, char* argv[]) {
	if (config_ == NULL)
		config_ = new EngineConfig;

	atexit(Engine::exitCallback);
	glutInit(&argc, argv);
	glutInitDisplayMode( config_->display_mode() );
	glutInitWindowSize( config_->width() , config_->height() );
	glutCreateWindow( config_->window_name().c_str() );

	width_ = config_->width();
	height_ = config_->height();

	glutDisplayFunc(Engine::renderCallback);
	glutReshapeFunc(Engine::reshapeCallback);
	glutVisibilityFunc(Engine::visibilityCallback);
	glutIdleFunc(Engine::idleCallback);
    glClearColor(0.0, 0.0, 0.0, 0.0); //transparent, I hope =P
    
	if (config_->do_multisampling())
		glEnable(GLUT_MULTISAMPLE);
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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

        calculateFPS();
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
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
	SceneList::iterator it;
	for (it = scenes_.begin(); it != scenes_.end(); ++it)
		(*it)->Render();

	if (scenes_.size() > 0) {
		LightList::iterator lit;
		for (lit = lights_.begin(); lit != lights_.end(); ++lit) {
			if ( (*lit)->casts_shadow() ) {
				glClear(GL_STENCIL_BUFFER_BIT);
				scenes_.back()->RenderShadows( (*lit) );
			}
		}
	}

	glutSwapBuffers();
}

void Engine::WindowReshape(int w, int h) {
	if (scenes_.size() > 0) {
		scenes_.back()->WindowResize(w, h);
		glutPostRedisplay();
	}
	width_ = w;
	height_ = h;
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

void Engine::DrawString(double x, double y, const char* str, const Color& color) {
    /*Firstly, go to ortho projection to draw text at the screen in 2D*/
    glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, static_cast<double>(width_), 0.0, static_cast<double>(height_));
	glScalef(1, -1, 1);
	glTranslatef(0.0, -static_cast<float>(height_), 0.0);
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);

    /*render the string*/
    glColor4fv(color.val);
    const char *c;
    glRasterPos2d(x, y);
    for (c=str; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

    /*restore previous projection*/
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Engine::ToggleFullscreen() {
    if (fullscreen_) {
        glutReshapeWindow(800, 600);
        glutReshapeWindow(width_, height_);
        glutPositionWindow(50,50);
    }
    else {
        glutFullScreen();
    }
    fullscreen_ = !fullscreen_;
}

void Engine::calculateFPS() {
    frames_++;
	unsigned time = glutGet(GLUT_ELAPSED_TIME);
	if (time - previous_ > 1000) {
		fps_ = frames_*1000.0/(time-previous_);
		previous_ = time;
		frames_ = 0;
	}
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
