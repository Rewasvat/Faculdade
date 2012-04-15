#include <engine/engine.h>
#include <simulation/simulation.h>
#include <simulation/vectorfield.h>
#include <simulation/objects/fieldobject.h>
#include <simulation/objects/cylinder.h>
#include <GL/glut.h>

using namespace engine;

namespace simulation {

Simulation::Simulation(VectorField* field) : Scene(), EventHandler(), use_perspective_proj_(true), field_(field) {
	field_object_ = new objects::FieldObject();
	field_object_->ReparentTo(this);

	int i, j, k;
	for (i=0; i < field_->nX(); i++) {
		for (j=0; j < field_->nY(); j++) {
			for (k=0; k < field_->nZ(); k++) {
				Vector3D pos = field_->GetBaseVectorInGrid(i,j,k).first;
				double height = field_->GetBaseVectorInGrid(i,j,k).second.Length();
				objects::Cylinder* c = new objects::Cylinder( pos, 0.25, 0.25, height );
				c->ReparentTo(field_object_);
			}
		}
	}

	// Specifying default camera position and direction
	// NOTE: we maintain the camera position in a sphere covering the vector field cubic volume
	center_ = field_->GetFieldCenterPos();

	double r = center_.Length();
	camera_distance_ = r * 15.0;

	azimuth_ = 0.0;
	elevation_ = 0.0;
	zoom_ = 0.0;
}

Simulation::~Simulation() {
}

void Simulation::Start() {
	this->SetProjectionMode();

	glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);
}

void Simulation::Update(double dt) {
    Scene::Update(dt);
}

void Simulation::Render() {
	//TODO: Still haven't decided on a unified transformation system for the framework, so lets do this hardcoded for now
	glLoadIdentity(); /*Resets transformation state*/

	/*Apply the viewing transformation of the scene*/
	//gluLookAt(camera_pos_.x, camera_pos_.y, camera_pos_.z, center_.x, center_.y, center_.z, camera_up_.x, camera_up_.y, camera_up_.z);
	//gluLookAt(camera_pos_.x, camera_pos_.y, camera_pos_.z, 0.0,0.0,-1.0  , 0.0, 1.0, 0.0);

	//glTranslated(-center_.x, -center_.y, -center_.z);
	//trying to position camera in polar coordinates around the origin
	//double radius = dist.Length() - center_.Length();
	//glTranslated(0.0, 0.0, -(radius + zoom_));
	glTranslated(center_.x, center_.y, center_.z -(camera_distance_ + zoom_));

	glRotated(elevation_, 1.0, 0.0, 0.0); //angle of rotation of the camera in the y-z plane, measured from the positive z-axis
	glRotated(azimuth_, 0.0, 0.0, 1.0); //angle of rotation of the camera about the object in the x-y plane, measured from the positive y-axis
	
	
	glTranslated(-center_.x, -center_.y, -center_.z);

	glPushMatrix();
	glTranslated(center_.x, center_.y, center_.z);
	glColor3d(0.0,0.0,0.0);
	glutWireSphere(center_.Length(), 30, 15);
	glPopMatrix();

    Scene::Render();
    
}

void Simulation::End() {
    Scene::End();
}

void Simulation::SetProjectionMode() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	double near_d = camera_distance_ + zoom_ - center_.Length()*5;

	if (use_perspective_proj_) {
		/*void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
			viewing volume is defined by the parameters: (left, bottom, -near) and (right, top, -near) specify the (x, y, z)
			coordinates of the lower-left and upper-right corners of the near clipping plane; near and far give the distances
			from the viewpoint to the near and far clipping planes. They should always be positive.*/
		//glFrustum(0.0, center_.x*2, 0.0, center_.y*2, near_d, near_d + center_.Length()*10);
		glFrustum(-center_.x*0.5, center_.x*2.5, -center_.y*0.5, center_.y*2.5, near_d, near_d + center_.Length()*10);
	}
	else {
		/*void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
		(left, bottom, -near) and (right, top, -near) are points on the near clipping plane that are mapped to the lower-left
		and upper-right corners of the viewport window, respectively. (left, bottom, -far) and (right, top, -far)
		are points on the far clipping plane that are mapped to the same respective corners of the viewport. 
		Both near and far can be positive or negative.*/
		//glOrtho(0.0, center_.x*2, 0.0, center_.y*2, near_d, near_d + center_.Length()*10);
		glOrtho(-center_.x*0.5, center_.x*2.5, -center_.y*0.5, center_.y*2.5, near_d, near_d + center_.Length()*10);

		//glOrtho(-2.5*w/h, 2.5*w/h, -2.5, 2.5, -10.0, 10.0);
	}


    glMatrixMode(GL_MODELVIEW);
}

void Simulation::MouseHandler(int btn, int state, int x, int y) {
}

void Simulation::MouseMotionHandler(int btn, int dx, int dy) {
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

void Simulation::KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // 27 = '\e' = ESCAPE  (for some magical reason, '\e' does not exist in Windows >_>)
	case 'q':
		Finish();
		break;
	case 'p':
		use_perspective_proj_ = !use_perspective_proj_;
		this->SetProjectionMode();
		break;
	}
}


}
