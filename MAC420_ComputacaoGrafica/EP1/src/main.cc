#include <cstdlib>
#include <cstdio>
#include <engine/engine.h>
#include <engine/engineconfig.h>
#include <simulation/simulation.h>
#include <simulation/loaderutil.h>


int main(int argc, char* argv[]) {
	simulation::LoaderUtil loader(argc, argv);

	if (!loader.GetVectorField()) {
		loader.PrintUsage();
		return 0;
	}

	engine::Engine* e = engine::Engine::reference();
	
	engine::EngineConfig* config = loader.GetEngineConfig();
	config->set_window_name("MAC420 EP1 - Simulacao de Particulas em um Campo de Forcas");
	e->Configure(config);
	e->Initialize(argc, argv);
	
	simulation::Simulation* scene = new simulation::Simulation();
    e->PushScene(scene);
    
    e->Run();
    
	delete e;

    return 0;
}
