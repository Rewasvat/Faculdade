#include <simulation/objects/cylinder.h>
#include <GL/glut.h>

namespace simulation {
namespace objects {

Cylinder::Cylinder() {
	GLUquadricObj* qobj = gluNewQuadric();
}

Cylinder::~Cylinder() {
}

void Cylinder::Update(double dt) {
}

void Cylinder::Render() {
}

}
}