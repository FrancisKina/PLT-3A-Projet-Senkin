/*
#include "engine.h"
#include "render.h"
#include "state.h"
#include "client.h"
#include "ai.h"
#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <thread>
*/

#include "Client.h"
#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <thread>

using namespace state;
using namespace engine;
using namespace render;
using namespace std;
using namespace client;
using namespace ai;


Client::Client(){
}

void Client::run(){
	
	srand(time(NULL));
	//Initialisation générale
		//Initialisation de la grille par le moteur
	Engine engine;
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
	state.getPlayers()[0]->setX(10);
	state.getPlayers()[0]->setY(10);
	state.getPlayers()[1]->setX(10);
	state.getPlayers()[1]->setY(14);
	state.getPlayers()[2]->setX(14);
	state.getPlayers()[2]->setY(14);
	state.getPlayers()[3]->setX(14);
	state.getPlayers()[3]->setY(10);
		//Initialisation des skills
	SkillFactory sf;
	state.getPlayers()[0]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(PIETINEMENT)});
	state.getPlayers()[1]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(SOIN_LEGER)});
	state.getPlayers()[2]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(ARC)});
	state.getPlayers()[3]->setSkills({sf.createSkill(SOIN), sf.createSkill(FEU_D_ENFER)});
	
	//debut du jeu
	engine.startGame(state);
	
		//Initialisation de la liste des différents layers avec texture
	StateLayer statelayer;
	statelayer.initLayers(state);
	statelayer.initWindow(state);
	state.registerObserver(&statelayer);
		//Creation puis affichage de la fenêtre
	sf::RenderWindow& window = statelayer.getWindow(); 
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				engine.keyCommand(event);
			}
		}

		window.display();
	}
	
}
