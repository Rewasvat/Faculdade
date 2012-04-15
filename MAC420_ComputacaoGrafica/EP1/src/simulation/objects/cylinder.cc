#include <simulation/objects/cylinder.h>
#include <GL/glut.h>

namespace simulation {
namespace objects {

Cylinder::Cylinder(engine::Vector3D& pos, double base_radius, double top_radius, double height)
	: base_radius_(base_radius), top_radius_(top_radius), height_(height) {
	position_ = pos;
	quadric_ = gluNewQuadric();
	color_[0] = (double)rand()/(double)RAND_MAX;
	color_[1] = (double)rand()/(double)RAND_MAX;
	color_[2] = (double)rand()/(double)RAND_MAX;
}

Cylinder::~Cylinder() {
	gluDeleteQuadric(quadric_);
}

void Cylinder::Update(double dt) {
}

void Cylinder::Render() {
	gluQuadricDrawStyle(quadric_, GLU_FILL); /* flat shaded */
	gluQuadricNormals(quadric_, GLU_SMOOTH);
	
	/*glPushMatrix copies the current matrix (top of the matrix stack), and places the copy at the top*/
	glPushMatrix(); 


	glColor3d(color_[0], color_[1], color_[2]);
	glTranslated(position_.x, position_.y, position_.z);
	//glRotated(45.0, 1.0, 0.0, 0.0);
	gluCylinder(quadric_, base_radius_, top_radius_, height_, 30, 15);

	/*glPopMatrix removes the top matrix of the stack*/
	glPopMatrix();
}

}
}