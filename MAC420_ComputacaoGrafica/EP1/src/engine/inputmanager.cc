#include "inputmanager.h"
#include <GL/glut.h>
#include <engine/engine.h>
#include <engine/eventhandler.h>

namespace engine {

InputManager::InputManager() {
	buttons_[0] = buttons_[1] = buttons_[2] = false;

	buttons_map_[0] = GLUT_LEFT_BUTTON;
	buttons_map_[1] = GLUT_MIDDLE_BUTTON;
	buttons_map_[2] = GLUT_RIGHT_BUTTON;
}


InputManager::~InputManager() {
	HandlerList::iterator it;
	for (it = handlers_.begin(); it != handlers_.end(); ++it)
		delete *it;
}

void globalMouseHandler(int btn, int state, int x, int y){
	Engine::reference()->input_manager()->CallMouseHandlers(btn, state, x, y);
}
void globalKeyboardHandler(unsigned char key, int x, int y) {
	Engine::reference()->input_manager()->CallKeyboardHandlers(key, x, y);
}

void globalMouseMoveFunc(int x, int y) {
	Engine::reference()->input_manager()->CallMouseMotionHandlers(x, y);
}

void InputManager::Initialize() {
	glutMouseFunc(globalMouseHandler);
	glutKeyboardFunc(globalKeyboardHandler);
	glutMotionFunc(globalMouseMoveFunc);
	glutPassiveMotionFunc(globalMouseMoveFunc);
}

void InputManager::AddHandler(EventHandler* handler) {
	handlers_.push_back(handler);
}

void InputManager::RemoveHandler(EventHandler* handler) {
	if (!handlers_.empty())
		handlers_.remove(handler);
}

void InputManager::CallMouseHandlers(int btn, int state, int x, int y) {
	buttons_[0] = (btn==buttons_map_[0] && state==GLUT_DOWN);
	buttons_[1] = (btn==buttons_map_[1] && state==GLUT_DOWN);
	buttons_[2] = (btn==buttons_map_[2] && state==GLUT_DOWN);
	last_mouse_coords_[0] = x;
	last_mouse_coords_[1] = y;

	// call registered handlers
	HandlerList::iterator it;
	for (it = handlers_.begin(); it != handlers_.end(); ++it) {
		if ( (*it)->handles_mouse() )
			(*it)->MouseHandler(btn, state, x, y);
	}
}

void InputManager::CallMouseMotionHandlers(int x, int y) {
	int dx = (x - last_mouse_coords_[0]);
	int dy = (y - last_mouse_coords_[1]);
	last_mouse_coords_[0] = x;
	last_mouse_coords_[1] = y;

	int i;
	for (i=0; i<3; i++) {
		if (buttons_[i]) {
			HandlerList::iterator it;
			for (it = handlers_.begin(); it != handlers_.end(); ++it) {
				if ( (*it)->handles_mouse() )
					(*it)->MouseMotionHandler(buttons_map_[i], dx, dy);
			}
		}
	}
}

void InputManager::CallKeyboardHandlers(unsigned char key, int x, int y) {
	HandlerList::iterator it;
	for (it = handlers_.begin(); it != handlers_.end(); ++it) {
		if ( (*it)->handles_keyboard() )
			(*it)->KeyboardHandler(key, x, y);
	}
}

}