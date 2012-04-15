#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>

namespace simulation {

class VectorField;

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
	
	void SetVectorField(VectorField* field);

protected:
    int n;
	VectorField* field_;
};

}
#endif
