/*******************************************************************/
/**   MAC 420 - Introdu��o � Computa��o Gr�fica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exerc�cio-Programa                                  **/
/**   Arquivo: mundoime.cc                                        **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#include <engine/engine.h>
#include <mundoime/mundoime.h>
#include <engine/objload/objload.h>
#include <GL/glut.h>
#include <cstdio>
#include <math.h>
#include <btBulletDynamicsCommon.h>
#include <mundoime/physicsmanager.h>
#include <mundoime/objects/model.h>
#include <mundoime/objects/player.h>
#include <mundoime/objects/skybox.h>
#include <mundoime/objects/sun.h>
#include <mundoime/objects/rain.h>

#define PI 3.14159265358979323846
#define E  2.71828182845904523536

using namespace engine;

namespace mundoime {

MundoIME::MundoIME() : Scene(), EventHandler() {

	objects::Skybox* skybox = new objects::Skybox("Models/skybox", 50);
	skybox->ReparentTo(this);

	sun_ = new objects::Sun(1.0, 50.0);
    sun_->ReparentTo(this);
	skybox->set_related_sun(sun_);

	objects::Rain* rain = new objects::Rain();
	rain->ReparentTo(this);
	sun_->set_related_rain(rain);

	ime_file_ = new Obj::File();
	if (ime_file_->Load("Models/IME.obj")) {
	
	    std::vector<Obj::VertexBuffer> meshes;

		ime_file_->GroupsToVertexArrays(meshes);
        printf("Number of models: %d\n", meshes.size());
        
        std::vector<Obj::VertexBuffer>::iterator it;
        for (it = meshes.begin(); it != meshes.end(); ++it) {
            Vector3D mpos (0.0, 0.0, 0.0);
            Vector3D mdir (0.0, 0.0, 0.0);
            btCollisionShape* mshape = NULL;
            /******/
            objects::Model* model = new objects::Model(mpos, mdir, (*it), 0, mshape);
            model->ReparentTo(this);
            PhysicsManager::reference()->AddBody( model->body() );
			if ( (*it).name.find("Barrier") != std::string::npos) {
				model->set_mesh_visible(false);
				model->set_casts_shadow(false);
			}
        }
	}

    player_file_ = new Obj::File();
	if (player_file_->Load("Models/player.obj")) {
	
	    std::vector<Obj::VertexBuffer> pmeshes;
	
		player_file_->GroupsToVertexArrays(pmeshes);
        printf("Number of player models: %d\n", pmeshes.size());
        
        Vector3D ppos (0.0, 2.0, 0.0);
        Vector3D pdir (0.0, 0.0, 1.0);
		double model_height = 0.35;
        btCollisionShape* pshape = new btCapsuleShape(model_height/4, model_height/2);
        player_ = new objects::Player(ppos, pdir, pmeshes[0], 68.0, model_height, pshape);
        player_->ReparentTo(this);
		//player_->set_casts_shadow(false);
        PhysicsManager::reference()->AddBody( player_->body() );
	}
}

MundoIME::~MundoIME() {
    delete ime_file_;
    delete player_file_;
}

void MundoIME::Start() {
	setPerspective((double)Engine::reference()->window_width(), (double)Engine::reference()->window_height());

	glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);

	glutWarpPointer(Engine::reference()->window_width()/2, Engine::reference()->window_height()/2);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void MundoIME::setPerspective(double w, double h) {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	double aspect_ratio = w / h;
    double fov = 60.0;
	gluPerspective(fov, aspect_ratio, 0.05, 3000);

	/*Trying to create a infinite-perspective projection matrix (far-plane at infinite distance)
	  Some sources say it is a requirement for depth-fail shadow volume algorithms. 
	  STILL NOT WORKING */
	/*double n = 0.05;
    double rad_fov = PI * (fov) / 180.0;
	double t = n * tan(rad_fov);
	double b = -t;    
	double r = t * aspect_ratio;
	double l = -r;
    printf("Perspective: AR=%lf (n=%lf|t=%lf|b=%lf|r=%lf|l=%lf)\n", aspect_ratio, n, t, b, r, l);
    double persp[16] = { 2*n/(r-l),     0.0,            0.0,        0.0,
						0.0,            2*n*(t-b),      0.0,        0.0,
						(r+l)/(r-l),    (t+b)/(t-b),    -1.0,       -1.0,
						0.0,            0.0,            -2*n,       0.0 };
	glLoadMatrixd(persp);*/

    glMatrixMode(GL_MODELVIEW);
}

void MundoIME::Update(double dt) {
    PhysicsManager::reference()->Update(dt);
	Scene::Update(dt);
}

static void clearCharBuffer(char* buffer, int size) {
	for (int i =0; i<size; i++)
		buffer[i] = 0;
}

void MundoIME::Render() {
	//TODO: Still haven't decided on a unified transformation system for the framework, so lets do this hardcoded for now
	glLoadIdentity(); /*Resets transformation state*/

	Vector3D eye = player_->eye_position();
	Vector3D dir = player_->direction();
	Vector3D up = player_->up();
	gluLookAt(eye.x, eye.y, eye.z, eye.x+dir.x, eye.y+dir.y, eye.z+dir.z, up.x, up.y, up.z);
	
    Scene::Render();

    //char draw_str[64];
	clearCharBuffer(draw_str, 64);
    sprintf(draw_str, "FPS: %4.2lf", Engine::reference()->FPS());
    Engine::reference()->DrawString(5.0, 20.0, &draw_str[0], RED);

	clearCharBuffer(draw_str, 64);
    sprintf(draw_str, "Player Speed: %2.3lf", player_->speed());
    Engine::reference()->DrawString(5.0, 40.0, &draw_str[0], RED);

	clearCharBuffer(draw_str, 64);
    sun_->GetCurrentTimeStr(draw_str);
    Engine::reference()->DrawString(5.0, 60.0, &draw_str[0], RED);
}

void MundoIME::RenderShadows(Light* light) { 
	if (!is_visible_) return;

	glPushAttrib(GL_ENABLE_BIT);
 	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LESS);

	glColorMask(0, 0, 0, 0);
	glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glEnable(GL_CULL_FACE);
	/*******
	// Depth-Pass method: for shadow volumes that the camera is not inside
	glCullFace(GL_BACK);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	RenderShadowChilds(light);

	// second pass, stencil operation increases stencil value
	glCullFace(GL_FRONT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	RenderShadowChilds(light);
	/*******/
	// Depth-Fail method: for shadow volumes with the camera inside
	glCullFace(GL_FRONT);
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	RenderShadowChilds(light);

	glCullFace(GL_BACK);
	glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
	RenderShadowChilds(light);
	/*******/

	//glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glColorMask(1, 1, 1, 1);

	//draw a shadowing rectangle covering the entire screen
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glStencilFunc(GL_NOEQUAL, 0, 0xffffffff);
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f,-0.10f);
		glVertex3f(-0.1f,-0.1f,-0.10f);
		glVertex3f( 0.1f, 0.1f,-0.10f);
		glVertex3f( 0.1f,-0.1f,-0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glShadeModel(GL_SMOOTH);

	glPopAttrib();
}

void MundoIME::End() {
    Scene::End();
}

void MundoIME::WindowResize(int w, int h) {
	glViewport(0, 0, (GLint)w, (GLint)h);
	setPerspective((double)w, (double)h);
}

void MundoIME::MouseHandler(int btn, int state, int x, int y) {
}

void MundoIME::MouseMotionHandler(int btn, int dx, int dy) {
}

void MundoIME::KeyboardHandler(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // 27 = '\e' = ESCAPE  (for some magical reason, '\e' does not exist in Windows >_>)
	case 'q':
		Finish();
		break;
	case '+':
		sun_->IncreateTimeRate();
		break;
	case '-':
		sun_->DecreaseTimeRate();
		break;
    case 'F':
        Engine::reference()->ToggleFullscreen();
	/*case 'r':
		player_->set_position(Vector3D(0.0, 3.0, 0.0));
		break;*/
	}
}


double NormalDistribution(double x, double mean, double variance) {
	double exponent = -pow((x - mean), 2) / (2*pow(variance, 2));
	return (pow(E, exponent)) / (sqrt(2*PI*pow(variance,2)));
}

}
