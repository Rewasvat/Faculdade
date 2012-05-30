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
