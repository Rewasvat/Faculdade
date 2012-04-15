#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <engine/scene.h>
#include <engine/eventhandler.h>
#include <engine/vector3D.h>

namespace simulation {

class VectorField;

namespace objects {
	class FieldObject;
}

class Simulation : public engine::Scene, public engine::EventHandler {
public:
    Simulation(VectorField* field);
    ~Simulation();
    
    virtual void Start();
	virtual void Update(double dt);
	virtual void Render();
	virtual void End();
	
	void SetProjectionMode();

    virtual void MouseHandler(int btn, int state, int x, int y);
	virtual void MouseMotionHandler(int btn, int dx, int dy);
	virtual void KeyboardHandler(unsigned char key, int x, int y);

protected:
	bool use_perspective_proj_;
	VectorField* field_;
	objects::FieldObject* field_object_;

	double azimuth_;
	double elevation_;
	double zoom_;
	double camera_distance_;
	engine::Vector3D center_;
};

}
#endif
