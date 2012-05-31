#include <engine/engine.h>
#include <simulation/simulation.h>
#include <simulation/vectorfield.h>
#include <simulation/objects/fieldobject.h>
#include <simulation/objects/cylinder.h>
#include <simulation/objects/sphere.h>
#include <GL/glut.h>
#include <cstdio>

using namespace engine;

namespace mundoime {

MundoIME::MundoIME() : Scene(), EventHandler(), paused_(false) {

}

MundoIME::~MundoIME() {
}

void MundoIME::Start() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double r = center_.Length();
	double near_d = (camera_distance_) * 0.01;
    double far_d = camera_distance_ + r;
	gluPerspective(60.0, 1.0, near_d, far_d);
    glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);
}

void MundoIME::Update(double dt) {
	if (!paused_)
		Scene::Update(dt);
}

void MundoIME::Render() {
	//TODO: Still haven't decided on a unified transformation system for the framework, so lets do this hardcoded for now
	glLoadIdentity(); /*Resets transformation state*/

	/*Apply the viewing transformation of the scene*/
	//gluLookAt(camera_pos_.x, camera_pos_.y, camera_pos_.z, center_.x, center_.y, center_.z, camera_up_.x, camera_up_.y, camera_up_.z);
	//gluLookAt(camera_pos_.x, camera_pos_.y, camera_pos_.z, 0.0,0.0,-1.0  , 0.0, 1.0, 0.0);

	//trying to position camera in polar coordinates around the origin
    glTranslated(0.0, 0.0,  -(camera_distance_ + zoom_));
	glRotated(elevation_, 1.0, 0.0, 0.0); //angle of rotation of the camera in the y-z plane, measured from the positive z-axis
	glRotated(azimuth_, 0.0, 0.0, 1.0); //angle of rotation of the camera about the object in the x-y plane, measured from the positive y-axis
	
    //moving the entire scene (the cube) so that its center is in the origin - that helps with the camera and the projection (mainly perspective)
	glTranslated(-center_.x, -center_.y, -center_.z);


    Scene::Render();
}

void MundoIME::End() {
    Scene::End();
}

void MundoIME::MouseHandler(int btn, int state, int x, int y) {
}

void MundoIME::MouseMotionHandler(int btn, int dx, int dy) {
	if (paused_)	return;

	if (btn==GLUT_LEFT_BUTTON) {
		elevation_ += (double)dy / 10.0;
		/*If elevation if outside [-180,0] degrees the cube will "flip", and thus the azimuth movimentation
		  will also change direction */
		if (elevation_ > 0) elevation_ = 0.0;
		if (elevation_ < -180.0) elevation_ = -180.0;

		azimuth_ += (double)dx / 10.0;
	}
	else if (btn==GLUT_RIGHT_BUTTON) {
		zoom_ += (double)dy/10.0;
	}
}

void MundoIME::KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // 27 = '\e' = ESCAPE  (for some magical reason, '\e' does not exist in Windows >_>)
	case 'q':
		Finish();
		break;
	case ' ':
		paused_ = !paused_;
		break;
	}
}


}
