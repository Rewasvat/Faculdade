
#ifndef SPHERE_H_
#define SPHERE_H_

#include <engine/object.h>
#include <engine/eventhandler.h>
#include <GL/glut.h>

namespace simulation {

class VectorField;

namespace objects {

class Sphere : public engine::Object, public engine::EventHandler {
public:
    Sphere(engine::Vector3D& pos, VectorField* field);
    ~Sphere();

	void Update(double dt);
	void Render();

	void MouseHandler(int btn, int state, int x, int y) {}
	void MouseMotionHandler(int btn, int dx, int dy) {}
	void KeyboardHandler(unsigned char key, int x, int y);

protected:
	double radius_;
	VectorField* field_;
	double color_[3];
    bool active_;
	GLuint render_list_;
	GLUquadricObj* quadric_;

    void buildRenderList();
};

}
}
#endif
