
#ifndef SPHERE_H_
#define SPHERE_H_

#include <engine/object.h>
#include <GL/glut.h>

namespace simulation {

class VectorField;

namespace objects {

class Sphere : public engine::Object {
public:
    Sphere(engine::Vector3D& pos, VectorField* field);
    ~Sphere();

	void Update(double dt);
	void Render();

protected:
	double radius_;
	VectorField* field_;
	double color_[3];
	GLuint render_list_;
	GLUquadricObj* quadric_;

    void buildRenderList();
};

}
}
#endif
