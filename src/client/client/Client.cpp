#include "Client.h"
#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <thread>
#include "state.h"
#include "ai.h"


#include "../render/StateLayer.h"
namespace render {
  class StateLayer;
};


using namespace state;
using namespace engine;
using namespace render;
using namespace std;
using namespace client;
using namespace ai;

render::StateLayer statelayer;
bool updated = true;

void threadEngine(void* ptr1){
	Engine* ptr_engine=(Engine*)ptr1;
	while(1){
		ptr_engine->update();
		if(!updated){
			updated = true;
		}
	}
}

Client::Client(){
	srand(time(NULL));
	//Initialisation générale
		//Initialisation de la grille par le moteur
	State& state = engine.getState();
	state.initGrid("res/maptest.txt");
	
		//Initialisation des personnages
	CharacterFactory cf;
	state.initPlayers(4);
	
			//Definition en IA
	state.getPlayers()[0]->setIa(true);
	state.getPlayers()[1]->setIa(true);
	state.getPlayers()[2]->setIa(true);
	state.getPlayers()[3]->setIa(true);
			//Classes
	state.getPlayers()[0]->setCharacter(cf.createCharacter(KNIGHT));
	state.getPlayers()[1]->setCharacter(cf.createCharacter(FROG));
	state.getPlayers()[2]->setCharacter(cf.createCharacter(ARCHER));
	state.getPlayers()[3]->setCharacter(cf.createCharacter(DWARF));
			//Positions
	state.getPlayers()[0]->setX(2);
	state.getPlayers()[0]->setY(2);
	state.getPlayers()[1]->setX(2);
	state.getPlayers()[1]->setY(21);
	state.getPlayers()[2]->setX(21);
	state.getPlayers()[2]->setY(21);
	state.getPlayers()[3]->setX(21);
	state.getPlayers()[3]->setY(2);
		//Initialisation des skills
	SkillFactory sf;
	state.getPlayers()[0]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(PIETINEMENT)});
	state.getPlayers()[1]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(SOIN_LEGER)});
	state.getPlayers()[2]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(ARC)});
	state.getPlayers()[3]->setSkills({sf.createSkill(SOIN), sf.createSkill(FEU_D_ENFER)});
	
	state.getGrid()[7][6]->updateFieldStatus({TENT,999});
	state.getGrid()[17][6]->updateFieldStatus({TOWER,999});
	state.getGrid()[7][17]->updateFieldStatus({TOWER,999});
	state.getGrid()[17][17]->updateFieldStatus({TENT,999});	
	state.getGrid()[12][16]->updateFieldStatus({FORT,999});
	state.getGrid()[12][7]->updateFieldStatus({FORT,999});
	
	//Debut du jeu
	engine.startGame(state);
	
		//Initialisation de la liste des différents elements pour l'affichage
	//StateLayer statelayer;
	statelayer.initLayers(state);
	statelayer.initWindow(state);
	state.registerObserver(&statelayer);
	
	ia = new DeepIA;
}

void Client::run(){
		//Creation puis affichage de la fenêtre
	sf::RenderWindow& window = statelayer.getWindow(); 
	
	std::thread th(threadEngine, &engine);
	
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				engine.setNextKeyCommand(event);
			}
		}
		if (updated){
			if (engine.getState().getPlayers().size()>1){
				ia->run(engine);
				updated = false;
			}
			engine.getState().notifyObservers(engine.getState());
		}
		
		window.display();
	}
	
	th.join();
}
