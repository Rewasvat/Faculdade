#include <cstdlib>
#include <cstdio>
#include <engine/engine.h>
#include <engine/engineconfig.h>
#include <mundoime/mundoime.h>
#include <mundoime/physicsmanager.h>

void mundoIME_exitCallback() {
    mundoime::PhysicsManager* physics = mundoime::PhysicsManager::reference();
    delete physics;
}

int main(int argc, char* argv[]) {

	engine::Engine* e = engine::Engine::reference();
	e->RegisterCustomExitCallback(mundoIME_exitCallback);
	engine::EngineConfig* config = new engine::EngineConfig();
	config->set_window_name("MAC420 EP2 - Mundo IME");
	e->Configure(config);
	e->Initialize(argc, argv);

    mundoime::PhysicsManager::reference()->Initialize();
	
	mundoime::MundoIME* scene = new mundoime::MundoIME( );

    e->PushScene(scene);
    
    e->Run();
    //from here on down, things never will be executed...
    return 0;
}
