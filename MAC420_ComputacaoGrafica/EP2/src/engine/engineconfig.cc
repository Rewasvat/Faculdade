/*******************************************************************/
/**   MAC 420 - Introdu��o � Computa��o Gr�fica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exerc�cio-Programa                                  **/
/**   Arquivo: engineconfig.cc                                    **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#include "engineconfig.h"
#include <GL/glut.h>

namespace engine {

EngineConfig::EngineConfig() {
	window_name_ = "MAC420 Engine Window";
	do_multisampling_ = true;
	width_ = 1024;
	height_ = 768;
}

EngineConfig::~EngineConfig() {
}

unsigned int EngineConfig::display_mode() const {
	unsigned int mode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
	if (do_multisampling_)
		mode |= GLUT_MULTISAMPLE;
	return mode;
}

}
