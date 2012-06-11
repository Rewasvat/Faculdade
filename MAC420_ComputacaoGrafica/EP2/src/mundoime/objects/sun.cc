#include <mundoime/objects/sun.h>
#include <engine/light.h>
#include <engine/color.h>
#include <GL/glut.h>
#include <cstdlib>
#include <math.h>

#define PI 3.14159265358979323846

using namespace engine;

namespace mundoime {
namespace objects {

Sun::Sun(double radius, double distance) : Light(GL_LIGHT0, DIRECTIONAL) {
	radius_ = radius;
	distance_ = distance;
	angle_ = PI/2;
	time_rate_ = 2*PI/300.0; //default is a day in 300 secs (5 minutes)
	position_ = Vector3D();
    updatePosAndDir();
	updateColors();
    Activate();
}

Sun::~Sun() {

}

void Sun::Update(double dt) {
	angle_ += time_rate_ * dt;
	if (angle_ > 2*PI)
		angle_ -= 2*PI;
	updatePosAndDir();
	updateColors();
}

void Sun::Render() {
	Light::Render();

    glColor4d(sun_color_[0], sun_color_[1], sun_color_[2], sun_color_[3]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_color_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sun_color_);
	glTranslated(position_.x, position_.y, position_.z);
    glutSolidSphere(radius_, 6, 6);
}

void Sun::IncreateTimeRate() {
	time_rate_ += PI/100.0;
}

void Sun::DecreaseTimeRate() {
	time_rate_ -= PI/100.0;
}

void Sun::updatePosAndDir() {
	/*rotate around X -> angle_
			y' = y*cos q - z*sin q
			z' = y*sin q + z*cos q
			x' = x
	  rotate around Y -> tilt to the side
			z' = z*cos q - x*sin q
			x' = z*sin q + x*cos q
			y' = y
*/
	position_.Scale(0.0);

	double nx, ny, nz;
	position_.z = -distance_;
	ny = position_.y*cos(angle_) - position_.z*sin(angle_);
	nz = position_.y*sin(angle_) + position_.z*cos(angle_);
	position_.y = ny;
	position_.z = nz;

	double tilt = PI/10;
	nx = position_.z*sin(tilt) + position_.x*cos(tilt);
	nz = position_.z*cos(tilt) - position_.x*sin(tilt);
	position_.x = nx;
	position_.z = nz;			

    Vector3D dir = -position_; //direction pointing towards origin
    set_direction( dir );
}

void Sun::updateColors() {
	if (angle_ > 0.0 && angle_ <= PI) {
		double factor = fabs( (PI/2.0) - angle_ ) / (PI/2.0); /*this is wrong =( */
		//printf("SUN ANGLE = %lf [%lf]\n", angle_, factor);
		SetAmbientColor(Color(0.3*factor, 0.3*factor, 0.3*factor, 0.5*factor));
		SetDiffuseColor(Color(1.0, 1.0, 1.0, factor));
		SetSpecularColor(Color(1.0, 1.0, 1.0, factor));
		sun_color_[0] = 1.0;
		sun_color_[1] = 1.0;
		sun_color_[2] = 0.8;
		sun_color_[3] = factor;
	}
	else {
		SetAmbientColor(Color(0.1, 0.1, 0.1, 0.05));
		SetDiffuseColor(Color(0.0, 0.0, 0.0, 0.0));
		SetSpecularColor(Color(0.0, 0.0, 0.0, 0.0));
		sun_color_[0] = 0.0;
		sun_color_[1] = 0.0;
		sun_color_[2] = 0.0;
		sun_color_[3] = 0.0;
	}
}

}
}

