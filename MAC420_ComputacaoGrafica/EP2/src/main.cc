#include <cstdlib>
#include <cstdio>
#include <engine/engine.h>
#include <engine/engineconfig.h>
#include <mundoime/mundoime.h>
#include <mundoime/loaderutil.h>

void mundoIME_exitCallback() {
	mundoime::LoaderUtil* loader = mundoime::LoaderUtil::reference();
	delete loader;
}

int main(int argc, char* argv[]) {
	mundoime::LoaderUtil* loader = mundoime::LoaderUtil::reference();

	loader->Load(argc, argv);

	if (!loader->GetVectorField()) {
		loader->PrintUsage();
		return 0;
	}

	engine::Engine* e = engine::Engine::reference();
	e->RegisterCustomExitCallback(mundoIME_exitCallback);
	engine::EngineConfig* config = loader->GetEngineConfig();
	config->set_window_name("MAC420 EP2 - Mundo IME");
	e->Configure(config);
	e->Initialize(argc, argv);
	
	mundoime::MundoIME* scene = new mundoime::MundoIME( );

    e->PushScene(scene);
    
    e->Run();
    //from here on down, things never will be executed...
    return 0;
}
