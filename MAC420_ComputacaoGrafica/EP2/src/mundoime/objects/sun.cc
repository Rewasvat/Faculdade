#include <mundoime/objects/sun.h>
#include <engine/light.h>
#include <engine/color.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <math.h>

#define PI 3.14159265358979323846
#define E  2.71828182845904523536

using namespace engine;

namespace mundoime {
namespace objects {

Sun::Sun(double radius, double distance) : Light(GL_LIGHT0, DIRECTIONAL) {
	radius_ = radius;
	distance_ = distance;
	angle_ = PI/6;
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

    glColor4f(sun_color_[0], sun_color_[1], sun_color_[2], sun_color_[3]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_color_);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sun_color_);
	glTranslated(position_.x, position_.y, position_.z);
    glutSolidSphere(radius_, 6, 6);

	float zero[] = {0.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zero);
}

void Sun::IncreateTimeRate() {
	time_rate_ += PI/100.0;
	if (time_rate_ > PI)  time_rate_ = PI;
}

void Sun::DecreaseTimeRate() {
	time_rate_ -= PI/100.0;
	if (time_rate_ < -PI)  time_rate_ = -PI;
}

void Sun::GetCurrentTimeStr(char* str) {
    int hours = 0, minutes = 0;
    minutes = 6*60 + abs(angle_ * 24 * 60 / (2*PI));
    while (minutes > 60) {
        minutes -= 60;
        hours++;
        if (hours > 23) hours = 0;
    }
    sprintf(str, "Time of Day: %d:%d", hours, minutes);
}

bool Sun::IsDaytime() {
    return (angle_ > 0.0 && angle_ <= PI);
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
    dir.Scale(-1.0); /*for some bizarre reason, in OpenGL, if we keep changing direction in each frame the direction vector gets inverted...*/
    set_direction( dir );
}

double normalDistribution(double x, double mean, double variance) {
	double exponent = -pow((x - mean), 2) / (2*pow(variance, 2));
	return (pow(E, exponent)) / (sqrt(2*PI*pow(variance,2)));
}

void Sun::updateColors() {
	if (IsDaytime()) {
		double mean = PI/2.0;
		double variance = 0.95;
		double max_f = normalDistribution(PI/2.0, mean, variance);
		double offset = 1.0 - max_f;
		time_factor_ = normalDistribution(angle_, mean, variance) + offset;

		SetAmbientColor(Color(0.2*time_factor_, 0.2*time_factor_, 0.2*time_factor_, 0.5*time_factor_));
		SetDiffuseColor(Color(1.0, time_factor_, time_factor_, time_factor_));
		SetSpecularColor(Color(1.0, 1.0, 1.0, time_factor_));
		sun_color_[0] = 1.0;
		sun_color_[1] = time_factor_;
		sun_color_[2] = 0.7*time_factor_;
		sun_color_[3] = 1.0;
		casts_shadow_ = true;
	}
	else {
		SetAmbientColor(Color(0.004, 0.004, 0.004, 1.0));
		SetDiffuseColor(Color(0.0, 0.0, 0.0, 0.0));
		SetSpecularColor(Color(0.0, 0.0, 0.0, 0.0));
		sun_color_[0] = 0.0;
		sun_color_[1] = 0.0;
		sun_color_[2] = 0.0;
		sun_color_[3] = 0.0;
		casts_shadow_ = false;
	}
}

}
}

