
#ifndef CYLINDER_H_
#define CYLINDER_H_

#include <engine/object.h>
#include <GL/glut.h>

namespace simulation {
namespace objects {

class Cylinder : public engine::Object {
public:
    Cylinder(engine::Vector3D& pos, double base_radius, double top_radius, double height);
    ~Cylinder();

	void Update(double dt);
	void Render();

protected:
	double base_radius_;
	double top_radius_;
	double height_;
	double color_[3];
	GLUquadricObj* quadric_;
};

}
}
#endif
