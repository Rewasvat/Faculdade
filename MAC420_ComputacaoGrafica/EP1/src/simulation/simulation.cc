#include <engine/engine.h>
#include <simulation/simulation.h>
#include <simulation/vectorfield.h>
#include <simulation/objects/fieldobject.h>
#include <simulation/objects/cylinder.h>
#include <simulation/objects/sphere.h>
#include <GL/glut.h>
#include <cstdio>

using namespace engine;

namespace simulation {

Simulation::Simulation(VectorField* field) : Scene(), EventHandler(), use_perspective_proj_(true), paused_(false), show_bounding_sphere_(false), field_(field) {
	field_object_ = new objects::FieldObject();
	field_object_->ReparentTo(this);

	int i, j, k;
	for (i=0; i < field_->nX(); i++) {
		for (j=0; j < field_->nY(); j++) {
			for (k=0; k < field_->nZ(); k++) {
				Vector3D pos = field_->GetBaseVectorInGrid(i,j,k).first;
				Vector3D dir = field_->GetBaseVectorInGrid(i,j,k).second;
				objects::Cylinder* c = new objects::Cylinder( pos, dir );
				c->ReparentTo(field_object_);

                objects::Sphere* sp = new objects::Sphere(pos);
                sp->ReparentTo(this);
			}
		}
	}

	// Specifying default camera position and direction
	// NOTE: we maintain the camera position in a sphere covering the vector field cubic volume
	center_ = field_->GetFieldCenterPos();

	double r = center_.Length();
	camera_distance_ = r * 2.0;

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
	if (!paused_)
		Scene::Update(dt);
}

void Simulation::Render() {
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

    if (show_bounding_sphere_) {
	    glPushMatrix();
	    glTranslated(center_.x, center_.y, center_.z);
	    glColor3d(0.0,0.0,0.0);
	    glutWireSphere(center_.Length(), 30, 15);
	    glPopMatrix();
    }

    Scene::Render();
    
}

void Simulation::End() {
    Scene::End();
}

void Simulation::SetProjectionMode() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double r = center_.Length();
	double near_d = (camera_distance_) * 0.01;
    double far_d = camera_distance_ + r;

	if (use_perspective_proj_) {
		/*void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
			viewing volume is defined by the parameters: (left, bottom, -near) and (right, top, -near) specify the (x, y, z)
			coordinates of the lower-left and upper-right corners of the near clipping plane; near and far give the distances
			from the viewpoint to the near and far clipping planes. They should always be positive.*/

        gluPerspective(60.0, 1.0, near_d, far_d);
	}
	else {
		/*void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
		(left, bottom, -near) and (right, top, -near) are points on the near clipping plane that are mapped to the lower-left
		and upper-right corners of the viewport window, respectively. (left, bottom, -far) and (right, top, -far)
		are points on the far clipping plane that are mapped to the same respective corners of the viewport. 
		Both near and far can be positive or negative.*/

        glOrtho(-r, r, -r, r, near_d, far_d);
	}

    glMatrixMode(GL_MODELVIEW);
}

void Simulation::MouseHandler(int btn, int state, int x, int y) {
}

void Simulation::MouseMotionHandler(int btn, int dx, int dy) {
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

void Simulation::KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // 27 = '\e' = ESCAPE  (for some magical reason, '\e' does not exist in Windows >_>)
	case 'q':
		Finish();
		break;
	case 'p':
		if (!paused_) {
			use_perspective_proj_ = !use_perspective_proj_;
			this->SetProjectionMode();
		}
		break;
	case ' ':
		paused_ = !paused_;
		break;
    case 'b':
        show_bounding_sphere_ = !show_bounding_sphere_;
        break;
    case 'r':
        Simulation* new_sim = new Simulation(field_);
        this->Finish();
        Engine::reference()->PushScene(new_sim);
		printf("Simulation has been restarted...\n");
		break;
	}
}


}
