#include <simulation/objects/cylinder.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265358979323846

namespace simulation {
namespace objects {

Cylinder::Cylinder(engine::Vector3D& pos, engine::Vector3D& direction) {
	position_ = pos;

	base_radius_ = 0.2;
	top_radius_ = 0.07;
	height_ = direction.Length();
	direction_ = direction;

	color_[0] = (double)rand()/(double)RAND_MAX;
	color_[1] = (double)rand()/(double)RAND_MAX;
	color_[2] = (double)rand()/(double)RAND_MAX;
	quadric_ = gluNewQuadric();
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

	rotateDirectionTowards( 1.0, 0.0, 0.0 );
	rotateDirectionTowards( 0.0, 1.0, 0.0 );
	rotateDirectionTowards( 0.0, 0.0, 1.0 );

	gluCylinder(quadric_, base_radius_, top_radius_, height_, 30, 15);

	/*glPopMatrix removes the top matrix of the stack*/
	glPopMatrix();
}

void Cylinder::rotateDirectionTowards(double x_axis, double y_axis, double z_axis) {
    engine::Vector3D axis(x_axis, y_axis, z_axis);
	double angle_rad = direction_ * axis;
	double angle_deg = (180.0 * angle_rad) / PI;
	glRotated(angle_deg, axis.x, axis.y, axis.z);
}

}
}
