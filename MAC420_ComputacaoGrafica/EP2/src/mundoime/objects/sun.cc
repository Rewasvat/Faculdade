#include <mundoime/objects/sun.h>
#include <mundoime/objects/rain.h>
#include <engine/light.h>
#include <engine/color.h>
#include <mundoime/mundoime.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <math.h>

#define PI 3.14159265358979323846

#define MAX_TIME_RATE PI/60.0
#define MIN_TIME_RATE -MAX_TIME_RATE

using namespace engine;

namespace mundoime {
namespace objects {

Sun::Sun(double radius, double distance) : Light(GL_LIGHT0, DIRECTIONAL) {
	radius_ = radius;
	distance_ = distance;
	angle_ = PI/6;
	time_rate_ = 2*PI/300.0; //default is a day in 300 secs (5 minutes)
	position_ = Vector3D();
	factor_mean_ = PI/2.0;
	factor_variance_ = 0.95;
	max_factor_ = NormalDistribution(factor_mean_, factor_mean_, factor_variance_);
	sun_color_ = engine::Color(0.0,0.0,0.0,0.0);
	related_rain_ = NULL;
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

	glColor4fv(sun_color_.val);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_color_.val);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sun_color_.val);
	glTranslated(position_.x, position_.y, position_.z);
    glutSolidSphere(radius_, 10, 10);

	float zero[] = {0.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zero);
}

void Sun::IncreateTimeRate() {
	time_rate_ += PI/180.0;
	if (time_rate_ > MAX_TIME_RATE)  time_rate_ = MAX_TIME_RATE;
}

void Sun::DecreaseTimeRate() {
	time_rate_ -= PI/180.0;
	if (time_rate_ < MIN_TIME_RATE)  time_rate_ = MIN_TIME_RATE;
}

void Sun::GetCurrentTimeStr(char* str) {
    int hours = 0, minutes = 0;
    minutes = 6*60 + static_cast<int>(abs(angle_ * 24 * 60 / (2*PI)));
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

static double interpolateTwo(double value1, double factor1, double value2, double factor2) {
	return (value1*factor1 + value2*factor2) / (factor1 + factor2);
}

void Sun::updateColors() {
	if (IsDaytime()) {
		double offset = 1.0 - max_factor_;
		time_factor_ = NormalDistribution(angle_, factor_mean_, factor_variance_) + offset;

		double r = 1.0;
		double g = time_factor_;
		double b = time_factor_;
		double ambient_color = 0.2 * time_factor_;
		double ambient_alpha = 0.5 * time_factor_;
		if (related_rain_ != NULL && related_rain_->raining()) {
			r = interpolateTwo(r, time_factor_, 0.9*time_factor_, related_rain_->intensity_factor());
			g = interpolateTwo(g, time_factor_, 0.8*time_factor_, related_rain_->intensity_factor());
			b = interpolateTwo(b, time_factor_, 0.8*time_factor_, related_rain_->intensity_factor());
			ambient_color = interpolateTwo(ambient_color, time_factor_, 0.1 * time_factor_, related_rain_->intensity_factor());
			ambient_alpha = interpolateTwo(ambient_alpha, time_factor_, 0.75 * time_factor_, related_rain_->intensity_factor());

			time_factor_ = interpolateTwo(1.0, time_factor_, 1.0, related_rain_->intensity_factor());
		}
		SetAmbientColor(Color(ambient_color, ambient_color, ambient_color, ambient_alpha));
		SetDiffuseColor(Color(r, g, b, time_factor_));
		SetSpecularColor(Color(r, r, r, time_factor_));
		sun_color_.r = r;
		sun_color_.g = g;
		sun_color_.b = b;
		sun_color_.a = 1.0;
		
		casts_shadow_ = true;
	}
	else {
		SetAmbientColor(Color(0.004, 0.004, 0.004, 1.0));
		SetDiffuseColor(Color(0.0, 0.0, 0.0, 0.0));
		SetSpecularColor(Color(0.0, 0.0, 0.0, 0.0));
		sun_color_.r = 0.0;
		sun_color_.g = 0.0;
		sun_color_.b = 0.0;
		sun_color_.a = 0.0;
		casts_shadow_ = false;
	}
}

}
}

