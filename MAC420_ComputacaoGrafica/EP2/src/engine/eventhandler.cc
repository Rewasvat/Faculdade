/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: eventhandler.cc                                    **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
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
