#include <engine/engine.h>
#include <mundoime/mundoime.h>
#include <engine/objload/objload.h>
#include <GL/glut.h>
#include <cstdio>
#include <btBulletDynamicsCommon.h>

#include <engine/light.h>
#include <engine/color.h>

using namespace engine;

namespace mundoime {

MundoIME::MundoIME() : Scene(), EventHandler(), paused_(false) {
	azimuth_ = 0.0;
	elevation_ = 0.0;
	zoom_ = 10.0;
	camera_distance_ = 30.0;
	center_ = Vector3D();

	file_ = new Obj::File();
	if (file_->Load("Models/teste.obj")) {
		file_->GroupsToVertexArrays(models_);
        printf("Number of models: %d\n", models_.size());
	}
	delete file_;

    Light* light = new Light(GL_LIGHT0, Light::SPOTLIGHT);
    light->SetSpotlightParameters(10.0);
    light->SetAmbientColor(Color(0.5, 0.5, 0.5, 1.0));
    light->SetDiffuseColor(Color(1.0, 1.0, 1.0, 1.0));
    light->SetSpecularColor(Color(1.0, 1.0, 1.0, 1.0));
    Vector3D pos(0.0, 0.0, 100.0);
    light->set_position( pos );
    Vector3D dir(0.0, 0.0, 1.0);
    light->set_direction( dir );
    light->Activate();

    light->ReparentTo(this);


    /****************/
int i;

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-10,0));

	///create a few basic rigid bodies
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-56,0));

	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}


	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

			startTransform.setOrigin(btVector3(2,10,0));
		
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(body);
	}



/// Do some simulation



	for (i=0;i<100;i++)
	{
		dynamicsWorld->stepSimulation(1.f/60.f,10);
		
		//print positions of all objects
		for (int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);
				printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
			}
		}
	}


	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	for (i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<collisionShapes.size();j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
    /****************/
}

MundoIME::~MundoIME() {

}

void MundoIME::Start() {
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double r = center_.Length();
	double near_d = (camera_distance_) * 0.01;
    double far_d = camera_distance_ + r;
	gluPerspective(60.0, 1.0, 1, far_d*10);
    glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);

    /*float light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    float light_diffuse[] = { 1.0, 1.0, 1.0, 0.8 };
    float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float light_position[] = { 2.0, 2.0, -50.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);*/
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

	if (models_.size() > 0) {
		VertexBufferList::iterator it;
		for (it = models_.begin(); it != models_.end(); ++it) {
			Obj::VertexBuffer vb = (*it);
			vb.gl();
		}
	}

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
