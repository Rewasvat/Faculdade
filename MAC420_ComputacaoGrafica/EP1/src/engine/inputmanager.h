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

	void CallMouseHandlers(int btn, int state, int x, int y);
	void CallKeyboardHandlers(unsigned char key, int x, int y);

private:
	HandlerList handlers_;
};

}
#endif