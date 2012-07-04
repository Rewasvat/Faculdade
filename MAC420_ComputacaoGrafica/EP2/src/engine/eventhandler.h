/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: eventhandler.h                                     **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

namespace engine {

class EventHandler {
public:
	EventHandler();
	~EventHandler();

	bool handles_mouse() { return handles_mouse_; }
	bool handles_keyboard() { return handles_keyboard_; }

	virtual void MouseHandler(int btn, int state, int x, int y) = 0;
	virtual void MouseMotionHandler(int btn, int dx, int dy) = 0;
	virtual void KeyboardHandler(unsigned char key, int x, int y) = 0;

protected:
	bool handles_keyboard_;
	bool handles_mouse_;
	bool is_removed_;
};

}
#endif
