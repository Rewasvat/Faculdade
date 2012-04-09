#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>

namespace simulation {

class Simulation : public engine::Scene, public engine::EventHandler {
public:
    Simulation();
    ~Simulation();
    
    virtual void Start();
	virtual void Update(double dt);
	virtual void Render();
	virtual void End();
	
    virtual void MouseHandler(int btn, int state, int x, int y);
	virtual void KeyboardHandler(unsigned char key, int x, int y);
	
protected:
    int n;
};

}
#endif
