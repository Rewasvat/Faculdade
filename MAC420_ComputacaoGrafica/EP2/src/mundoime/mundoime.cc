#include <engine/engine.h>
#include <mundoime/mundoime.h>
#include <engine/objload/objload.h>
#include <GL/glut.h>
#include <cstdio>
#include <btBulletDynamicsCommon.h>
#include <mundoime/objects/model.h>
#include <mundoime/objects/player.h>
#include <mundoime/physicsmanager.h>

#include <engine/light.h>
#include <engine/color.h>

using namespace engine;

namespace mundoime {

MundoIME::MundoIME() : Scene(), EventHandler(), paused_(false) {

    Light* light = new Light(GL_LIGHT0, Light::DIRECTIONAL);
    light->SetSpotlightParameters(10.0);
    light->SetAmbientColor(Color(1.0, 1.0, 1.0, 0.7));
    light->SetDiffuseColor(Color(1.0, 1.0, 1.0, 1.0));
    light->SetSpecularColor(Color(1.0, 1.0, 1.0, 1.0));
    Vector3D pos(0.0, 0.0, 0.0);
    light->set_position( pos );
    Vector3D dir(-1.0, -1.0, -1.0);
    light->set_direction( dir );
    light->Activate();
    light->ReparentTo(this);


	ime_file_ = new Obj::File();
	if (ime_file_->Load("Models/teste.obj")) {
	
	    std::vector<Obj::VertexBuffer> meshes;

        std::vector<Obj::Surface> surfaces;
	
		ime_file_->GroupsToVertexArrays(meshes);
        ime_file_->GroupsToSurfaces(surfaces);
        printf("Number of models: %d\n", meshes.size());
        
        std::vector<Obj::VertexBuffer>::iterator it;
        std::vector<Obj::Surface>::iterator its = surfaces.begin();
        for (it = meshes.begin(); it != meshes.end(); ++it, ++its) {
            Vector3D mpos (0.0, 0.0, 0.0);
            Vector3D mdir (0.0, 0.0, 0.0);
            btCollisionShape* mshape = NULL;
            //btCollisionShape* mshape = new btStaticPlaneShape(btVector3(0,1,0),1);
            /******
            btTriangleMesh* bttm = new btTriangleMesh(true, false);
            int i;
            printf("Going to build triangle mesh collision shape [%d]\n", its->m_Triangles.size());
            for (i=0; i<its->m_Triangles.size(); i++) {
                printf("Building Triangle Mesh i=%d/%d\n", i, its->m_Triangles.size());
                btVector3 v0 (its->m_Vertices[ its->m_Triangles[i].v[0] ].x, its->m_Vertices[ its->m_Triangles[i].v[0] ].y, its->m_Vertices[ its->m_Triangles[i].v[0] ].z);
                btVector3 v1 (its->m_Vertices[ its->m_Triangles[i].v[1] ].x, its->m_Vertices[ its->m_Triangles[i].v[1] ].y, its->m_Vertices[ its->m_Triangles[i].v[1] ].z);
                btVector3 v2 (its->m_Vertices[ its->m_Triangles[i].v[2] ].x, its->m_Vertices[ its->m_Triangles[i].v[2] ].y, its->m_Vertices[ its->m_Triangles[i].v[2] ].z);
                bttm->addTriangle (v0, v1, v2, false);
            }

            bool useQuantizedAabbCompression = true;
	        mshape = new btBvhTriangleMeshShape(bttm,useQuantizedAabbCompression);
            /******/
			printf("#Indexes=%d || #Triangles=%d\n", it->m_Indices.size(), its->m_Triangles.size());
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

	/*Apply the viewing transformation of the scene*/
	//gluLookAt(camera_pos_.x, camera_pos_.y, camera_pos_.z, center_.x, center_.y, center_.z, camera_up_.x, camera_up_.y, camera_up_.z);
	//gluLookAt(camera_pos_.x, camera_pos_.y, camera_pos_.z, 0.0,0.0,-1.0  , 0.0, 1.0, 0.0);

	Vector3D& eye = player_->eye_position();
	Vector3D& dir = player_->direction();
	Vector3D& up = player_->up();
	gluLookAt(eye.x, eye.y, eye.z, eye.x+dir.x, eye.y+dir.y, eye.z+dir.z, up.x, up.y, up.z);

	//trying to position camera in polar coordinates around the origin
    
	/*glRotated(up.x, 1.0, 0.0, 0.0); //angle of rotation of the camera in the y-z plane, measured from the positive z-axis
	glRotated(up.y, 0.0, 1.0, 0.0); //angle of rotation of the camera about the object in the x-y plane, measured from the positive y-axis
	glTranslated(eye.x, eye.y, eye.z);*/

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
