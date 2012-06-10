#include <engine/engine.h>
#include <mundoime/mundoime.h>
#include <engine/objload/objload.h>
#include <GL/glut.h>
#include <cstdio>
#include <btBulletDynamicsCommon.h>
#include <mundoime/objects/model.h>
#include <mundoime/objects/player.h>
#include <mundoime/objects/skybox.h>
#include <mundoime/physicsmanager.h>

#include <engine/light.h>
#include <engine/color.h>

using namespace engine;

namespace mundoime {

MundoIME::MundoIME() : Scene(), EventHandler(), paused_(false) {

	objects::Skybox* skybox = new objects::Skybox("Models/skybox", 50);
	skybox->ReparentTo(this);

	light_ = new Light(GL_LIGHT0, Light::DIRECTIONAL);
    light_->SetSpotlightParameters(10.0);
    light_->SetAmbientColor(Color(1.0, 1.0, 1.0, 0.7));
    light_->SetDiffuseColor(Color(1.0, 1.0, 1.0, 1.0));
    light_->SetSpecularColor(Color(1.0, 1.0, 1.0, 1.0));
    Vector3D pos(0.0, 2.0, 0.0);
    light_->set_position( pos );
    Vector3D dir(-1.0, -1.0, -1.0);
    light_->set_direction( dir );
    light_->Activate();
    light_->ReparentTo(this);

	Light::SetGlobalAmbientLight( Color(1.0, 1.0, 1.0, 1.0) );

	ime_file_ = new Obj::File();
	if (ime_file_->Load("Models/IMEblocoB.obj")) {
	
	    std::vector<Obj::VertexBuffer> meshes;

		ime_file_->GroupsToVertexArrays(meshes);
        printf("Number of models: %d\n", meshes.size());
        
        std::vector<Obj::VertexBuffer>::iterator it;
        for (it = meshes.begin(); it != meshes.end(); ++it) {
            Vector3D mpos (0.0, 0.0, 0.0);
            Vector3D mdir (0.0, 0.0, 0.0);
            btCollisionShape* mshape = NULL;
            //btCollisionShape* mshape = new btStaticPlaneShape(btVector3(0,1,0),1);
            /******/
            objects::Model* model = new objects::Model(mpos, mdir, (*it), 0, mshape);
            model->ReparentTo(this);
            PhysicsManager::reference()->AddBody( model->body() );
        }
	}

    player_file_ = new Obj::File();
	if (player_file_->Load("Models/player.obj")) {
	
	    std::vector<Obj::VertexBuffer> pmeshes;
	
		player_file_->GroupsToVertexArrays(pmeshes);
        printf("Number of player models: %d\n", pmeshes.size());
        
        Vector3D ppos (0.0, 7.0, 0.0);
        Vector3D pdir (0.0, 0.0, 1.0);
        btCollisionShape* pshape = new btCapsuleShape(0.25, 0.5);
        player_ = new objects::Player(ppos, pdir, pmeshes[0], 68.0, pshape);
        player_->ReparentTo(this);
        PhysicsManager::reference()->AddBody( player_->body() );
	}
}

MundoIME::~MundoIME() {
    delete ime_file_;
    delete player_file_;
}

void MundoIME::Start() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective(60.0, 1.0, 0.05, 300);
    glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);

	glutWarpPointer(Engine::reference()->window_width()/2, Engine::reference()->window_height()/2);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void MundoIME::Update(double dt) {
	if (!paused_) {
        PhysicsManager::reference()->Update(dt);
		Scene::Update(dt);
    }
}

void MundoIME::Render() {
	//TODO: Still haven't decided on a unified transformation system for the framework, so lets do this hardcoded for now
	glLoadIdentity(); /*Resets transformation state*/

	Vector3D eye = player_->eye_position();
	Vector3D dir = player_->direction();
	Vector3D up = player_->up();
	gluLookAt(eye.x, eye.y, eye.z, eye.x+dir.x, eye.y+dir.y, eye.z+dir.z, up.x, up.y, up.z);
	

    Scene::Render();
}

void MundoIME::End() {
    Scene::End();
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
	case ' ':
		paused_ = !paused_;
		break;
	}
}


}
