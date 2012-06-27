#include <mundoime/objects/rain.h>
#include <mundoime/mundoime.h>
#include <math.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>

#define PI 3.14159265358979323846
#define E  2.71828182845904523536

namespace mundoime {
namespace objects {

/******************** DROP *************************************/
Drop::Drop(engine::Vector3D& pos, engine::Color& start_color, engine::Color& end_color, float line_width, engine::Vector3D& fall_speed) {
	position_ = pos;
	start_color_ = start_color;
	end_color_ = end_color;
	line_width_ = line_width;
	fall_speed_ = fall_speed;
	finished_ = false;
}

Drop::~Drop() {
}

void Drop::Update(double dt) {
	engine::Vector3D deltaV = fall_speed_;
	deltaV.Scale(dt);

	position_ = position_ + deltaV;
	if (position_.y < -1.0) {
		finished_ = true;
	}
}

void Drop::Render() {
	glLineWidth(line_width_);
	glBegin(GL_LINES);

	glColor4fv(start_color_.val);
	engine::Vector3D p = fall_speed_;
	p.Normalize();
	p.Scale(-0.1);
	p = p + position_;
	glVertex3dv(p.val);

	glColor4fv(end_color_.val);
	glVertex3dv(position_.val);

	glEnd();
}

/******************** RAIN *************************************/
Rain::Rain() {
	/*	position -14.884317 -0.834598 -11.474524
		position 13.882018 0.100503 -11.474616
		position 13.881560 -0.063596 17.402067
		position -14.884803 -0.846410 17.402056*/
	min_coords_ = engine::Vector3D(-15.0, -1.0, -12.0);
	max_coords_ = engine::Vector3D(14.0, 10.0, 18.0);
	waitForRain();
}

Rain::~Rain() {
}

void Rain::startRain() {
	actual_intensity_ = intensity_= 5.0 + 145.0*Random();
	rain_time_ = 30.0 + 270*Random();
	rain_direction_ = engine::Vector3D(0.5-Random(), -2.0, 0.5-Random());

	time_elapsed_ = 0.0;
	factor_mean_ = rain_time_/2;
	factor_variance_ = 1.0; //1.5;
	max_factor_ = NormalDistribution(factor_mean_, factor_mean_, factor_variance_);
	drop_time_ = RandExpTime();
	raining_ = true;

	glEnable(GL_FOG);
    float fcf = Random()*0.7;
	GLfloat fogColor[4] = {fcf, fcf, fcf, 1.0};
	glFogi (GL_FOG_MODE, GL_EXP); //GL_EXP2, GL_LINEAR
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_START, 0.7+Random());
	glFogf (GL_FOG_END, 4.2+Random()*4.0);
}

void Rain::waitForRain() {
	raining_ = false;
	actual_intensity_ = intensity_ = 0.01;
	rain_time_ = RandExpTime();
	time_elapsed_ = 0.0;
    glDisable(GL_FOG);
}

void Rain::Update(double dt) {
	if (raining_) {
		//update actual intensity value
		double yoffset = 1.0 - max_factor_;
        double xoffset = (time_elapsed_ * 6.0 / rain_time_) - 3.0;
        actual_intensity_ = intensity_ * ( NormalDistribution(xoffset, 0.0, factor_variance_) + yoffset );

        glFogf (GL_FOG_DENSITY, 0.25f*(1+(intensity_factor()-0.5)));

		//check for rain drop creating loop
		drop_time_ -= dt;
		if (drop_time_ < 0.0) {
			drop_time_ = RandExpTime();
			for (int i=0; i < actual_intensity_; i++) {
				double xf = Random();
				double zf = Random();

				double x = min_coords_.x + ((fabs(min_coords_.x)+fabs(max_coords_.x))*xf);
				double z = min_coords_.z + ((fabs(min_coords_.z)+fabs(max_coords_.z))*zf);
				engine::Vector3D pos (x, max_coords_.y, z);

				float lw = 0.5 + Random();
				double speed = 9.0 + 7.0*Random() + actual_intensity_/10.0;
				engine::Vector3D dir_speed = rain_direction_;
				dir_speed.Normalize();
				dir_speed.Scale(speed);

				double scf = Random();
				double ecf = Random();
				engine::Color sc (0.35,0.35,0.8,0.4);
				engine::Color ec (0.1,0.1,0.8,0.8);

				Drop* drop = new Drop(pos, sc, ec, lw, dir_speed);
				drop->ReparentTo(this);
			}
		}

		//check for rain ending
		time_elapsed_ += dt;
		if (time_elapsed_ > rain_time_) {
			waitForRain();
		}
	}
	else {
		//check for rain start
		time_elapsed_ += dt;
		if (time_elapsed_ > rain_time_) {
			startRain();
		}
	}
	printf("Rain update (%d) [I=%3.3lf/%3.3lf][T=%3.3lf/%3.3lf]\n", (int)raining_, actual_intensity_, intensity_, time_elapsed_, rain_time_);

	//update rain drops
	UpdateChilds(dt);
	
	//removing old rain drops
	engine::ChildList::iterator it;
	std::list<Drop*> to_remove;
    for (it = childs_.begin(); it != childs_.end(); ++it) {
        Object* obj = (*it);
        Drop* drop = dynamic_cast<Drop*>(obj);
		if (drop->finished())
			to_remove.push_back(drop);
    }

	for (std::list<Drop*>::iterator rit = to_remove.begin(); rit != to_remove.end(); ++rit) {
		(*rit)->ReparentTo(NULL);
		delete (*rit);
	}
}

void Rain::Render() {
	glDisable(GL_LIGHTING);
	RenderChilds();
	glEnable(GL_LIGHTING);
}

// Returns ~exp(param)  time value
double Rain::RandExpTime() {
	double t = (1.0*-log(Random())/actual_intensity_);
    return t;
}

double Rain::Random() {
	return 1.0*rand()/RAND_MAX;
}

void Rain::KeyboardHandler(unsigned char key, int x, int y) {
	if (key == 'i') {
		intensity_ += 0.1;
		if (intensity_ > 120.0)	intensity_ = 100.0;
	}
	if (key == 'k') {
		intensity_ -= 0.1;
		if (intensity_ <= 5.0)	intensity_ = 3.0;
	}
	if (key == 'R')
		time_elapsed_ = rain_time_;
	
}

}
}
