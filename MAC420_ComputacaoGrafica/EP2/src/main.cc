/*******************************************************************/
/**   MAC 420 - Introdução à Computação Gráfica                   **/
/**   IME-USP - Primeiro Semestre de 2012                         **/
/**   BCC2009 - Marcel P. Jackowski                               **/
/**                                                               **/
/**   Segundo Exercício-Programa                                  **/
/**   Arquivo: main.cc                                            **/
/**                                                               **/
/**   Fernando Omar Aluani             #USP: 6797226              **/
/**                                                               **/
/**   Entregado em 03/07/2012                                     **/
/*******************************************************************/
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <engine/engine.h>
#include <engine/engineconfig.h>
#include <mundoime/mundoime.h>
#include <mundoime/physicsmanager.h>

void mundoIME_exitCallback() {
    mundoime::PhysicsManager* physics = mundoime::PhysicsManager::reference();
    delete physics;
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
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
