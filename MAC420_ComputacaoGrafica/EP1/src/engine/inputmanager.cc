#include "inputmanager.h"
#include <GL/glut.h>
#include <engine/engine.h>
#include <engine/eventhandler.h>

namespace engine {

InputManager::InputManager() {
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

void InputManager::Initialize() {
	glutMouseFunc(globalMouseHandler);
	glutKeyboardFunc(globalKeyboardHandler);
}

void InputManager::AddHandler(EventHandler* handler) {
	handlers_.push_back(handler);
}

void InputManager::RemoveHandler(EventHandler* handler) {
	if (!handlers_.empty())
		handlers_.remove(handler);
}

void InputManager::CallMouseHandlers(int btn, int state, int x, int y) {
	HandlerList::iterator it;
	for (it = handlers_.begin(); it != handlers_.end(); ++it) {
		if ( (*it)->handles_mouse() )
			(*it)->MouseHandler(btn, state, x, y);
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