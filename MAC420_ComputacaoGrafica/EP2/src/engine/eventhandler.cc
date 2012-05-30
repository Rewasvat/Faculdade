#include "eventhandler.h"
#include <engine/engine.h>
#include <engine/inputmanager.h>

namespace engine {

EventHandler::EventHandler() : handles_keyboard_(true), handles_mouse_(true), is_removed_(false) {
	Engine::reference()->input_manager()->AddHandler(this);
}

EventHandler::~EventHandler() {
	if (!is_removed_) {
		is_removed_ = true;
		Engine::reference()->input_manager()->RemoveHandler(this);
	}
}

}
