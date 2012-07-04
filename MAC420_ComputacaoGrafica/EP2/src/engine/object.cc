/*******************************************************************/
/**   MAC 420 - Introdu��o � Computa��o Gr�fica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exerc�cio-Programa                                  **/
/**   Arquivo: object.cc                                          **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#include <engine/object.h>
#include <engine/objectcontainer.h>
#include <cstdlib>

namespace engine {

void Object::ReparentTo(ObjectContainer* new_parent) {
	if (parent_ != NULL) {
		parent_->RemoveChild(this);
	}

	parent_ = new_parent;
	if (parent_ != NULL) {
		parent_->AddChild(this);
	}
}

}
