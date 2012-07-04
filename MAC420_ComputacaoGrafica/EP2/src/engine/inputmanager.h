/*******************************************************************/
/**   MAC 420 - Introdu��o � Computa��o Gr�fica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exerc�cio-Programa                                  **/
/**   Arquivo: inputmanager.h                                     **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include <list>

namespace engine {

class EventHandler;

typedef std::list<EventHandler*> HandlerList;

class InputManager {
public:
	InputManager();
	~InputManager();

	void Initialize();

	void AddHandler(EventHandler* handler);
	void RemoveHandler(EventHandler* handler);

	void Update();

	void CallMouseHandlers(int btn, int state, int x, int y);
	void CallMouseMotionHandlers(int x, int y);
	void CallKeyboardHandlers(unsigned char key, int x, int y);

private:
	HandlerList handlers_;
	HandlerList new_additions_;
	HandlerList to_be_removed_;

	bool buttons_[3];
	int buttons_map_[3];
	int last_mouse_coords_[2];
};

}
#endif