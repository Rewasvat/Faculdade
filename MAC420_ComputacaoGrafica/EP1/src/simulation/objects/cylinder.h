
#ifndef CYLINDER_H_
#define CYLINDER_H_

#include <engine/object.h>
#include <GL/glut.h>

namespace simulation {
namespace objects {

class Cylinder : public engine::Object {
public:
    Cylinder(engine::Vector3D& pos, engine::Vector3D& direction);
    ~Cylinder();

	void Update(double dt);
	void Render();

protected:
	double base_radius_;
	double top_radius_;
	double height_;
	engine::Vector3D direction_;
	double color_[3];
	GLuint render_list_;
	GLUquadricObj* quadric_;

    void buildRenderList();
};

}
}
#endif
