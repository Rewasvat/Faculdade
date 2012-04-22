#include <cstdlib>
#include <cstdio>
#include <engine/engine.h>
#include <engine/engineconfig.h>
#include <simulation/simulation.h>
#include <simulation/loaderutil.h>

void ep1_exitCallback() {
	simulation::LoaderUtil* loader = simulation::LoaderUtil::reference();
	delete loader;
}

int main(int argc, char* argv[]) {
	simulation::LoaderUtil* loader = simulation::LoaderUtil::reference();

	loader->Load(argc, argv);

	if (!loader->GetVectorField()) {
		loader->PrintUsage();
		return 0;
	}

	engine::Engine* e = engine::Engine::reference();
	e->RegisterCustomExitCallback(ep1_exitCallback);
	engine::EngineConfig* config = loader->GetEngineConfig();
	config->set_window_name("MAC420 EP1 - Simulacao de Particulas em um Campo de Forcas");
	e->Configure(config);
	e->Initialize(argc, argv);
	
	simulation::Simulation* scene = new simulation::Simulation( loader->GetVectorField() );

    e->PushScene(scene);
    
    e->Run();
    
	delete e;

    return 0;
}
