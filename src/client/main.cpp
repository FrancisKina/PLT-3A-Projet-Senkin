#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <time.h>
#include <unistd.h>


// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;


int main(int argc,char* argv[])
{
    if (argc>1){
		
		/*	render : Affichage du rendu de la map */
		if(strcmp(argv[1],"render")==0){
			
			//Initialisation de la grille
			State state_init;
			state_init.initGrid("res/maptest.txt");
			
			//Initialisation des personnages
			CharacterFactory cf;
			state_init.initPlayers(4);
			
				//Classes
			state_init.getPlayers()[0]->setCharacter(cf.createCharacter(KNIGHT));
			state_init.getPlayers()[1]->setCharacter(cf.createCharacter(FROG));
			state_init.getPlayers()[2]->setCharacter(cf.createCharacter(ARCHER));
			state_init.getPlayers()[3]->setCharacter(cf.createCharacter(DWARF));
				//Directions
			state_init.getPlayers()[0]->setDirection(NORTH);
			state_init.getPlayers()[1]->setDirection(EAST);
			state_init.getPlayers()[2]->setDirection(SOUTH);
			state_init.getPlayers()[3]->setDirection(WEST);
				//Positions
			srand(time(0));
			for(size_t i = 0; i<state_init.getPlayers().size(); i++){
				state_init.getPlayers()[i]->setX(rand()%state_init.getGrid()[0].size());
				state_init.getPlayers()[i]->setY(rand()%state_init.getGrid().size());
			}
			
			//Initialisation du curseur
			state_init.initCursor();
			state_init.getCursor()->setCursorX(rand()%state_init.getGrid()[0].size());
			state_init.getCursor()->setCursorY(rand()%state_init.getGrid().size());
			
			//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state_init, 35);
			
			//Creation de la fenêtre puis affichage du rendu
			statelayer.displayLayers(state_init);
		}
		
		/*engine : changements d'état*/
		else if(strcmp(argv[1],"engine")==0){
			
			srand(time(NULL));
			//Initialisation générale
				//Initialisation de la grille par le moteur
			Engine engine;
			State& state = engine.getState();
			state.initGrid("res/maptest.txt");
			
				//Initialisation des personnages
			CharacterFactory cf;
			state.initPlayers(4);
			
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
			
			//test gravité
			state.getGrid()[12][11]->updateFieldStatus({GRAVITY,999});
			
			engine.startGame(state);
			/*
			state.sortPlayers(); // Trier les personnages par initiative pour l'ordre d'action et donner le tour d'action au premier joueur
			
				//Initialisation du curseur
			state.initCursor();
			state.getCursor()->setCursorX(rand()%state.getGrid()[0].size());
			state.getCursor()->setCursorY(rand()%state.getGrid().size());
			*/
			
				//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state, 35);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			int tilesize = statelayer.getLayers()[0].getQuads()[1].position.x - statelayer.getLayers()[0].getQuads()[0].position.x;
			sf::RenderWindow window(sf::VideoMode(tilesize * state.getGrid()[0].size(), tilesize * state.getGrid().size()), "Test");
			statelayer.draw(window);
			
			
				//Suite de commandes
				//j2
			std::vector<Command*> commandList;
			commandList.push_back(new Move({11,11}));
			commandList.push_back(new Move({11,13}));
			commandList.push_back(new Move({11,11}));
			EndActions* endactions = new EndActions();
			commandList.push_back(endactions);
			
			//j1
			commandList.push_back(new Move({11,13}));
			commandList.push_back(new Move({19,12}));
			commandList.push_back(endactions);
			
			//j3
			commandList.push_back(new Move({11,11}));
			commandList.push_back(new Move({11,12}));
			commandList.push_back(new Attack(std::make_pair (11,13) , 0));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(endactions);
			
			//j4
			commandList.push_back(new Move({12,13}));
			commandList.push_back(new Attack(std::make_pair (11,13) , 0));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(endactions);
			
			//j2
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(endactions);
			
			//j1
			commandList.push_back(new Move({23,12}));
			commandList.push_back(new Move({11,14}));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(endactions);
			
			//j3
			commandList.push_back(new Move({11,11}));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			commandList.push_back(endactions);
			
			//j4
			commandList.push_back(new Move({14,13}));
			commandList.push_back(new Attack(std::make_pair (11,13) , 1));
			
			
			
			bool finCommande = false;
			bool go = false;
			
			while (window.isOpen())
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if(event.type == sf::Event::Closed){
						window.close();
					}
					//Sortie de pause
					else if(event.type==sf::Event::KeyPressed && go == false){
						go = true;
						cout << endl;
					}
				}
				
				if(go){
					sleep(1);
					if (commandList.size() > 0){
						engine.executeCommand(commandList[0],window);
						go = !(commandList[0] == endactions); //Mettre en pause si fin d'action
						commandList.erase(commandList.begin());
					}
					else if(finCommande == false){
						cout << endl << "---------- Fin de commande ----------" << endl;
						finCommande = true;
					}
				}
				else{
					cout << "\rAPPUYEZ SUR UNE TOUCHE POUR CONTINUER     ";
				}
				window.display();
			}
		}
		
		/*ai : partie joué par des ia aléatoires*/
		else if(strcmp(argv[1],"ai")==0){
			
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
			int SIZE = 35;	
			
			state.getGrid()[12][11]->updateFieldStatus({TOWER,999});
			state.getGrid()[12][13]->updateFieldStatus({TENT,999});
			state.getGrid()[12][15]->updateFieldStatus({FORT,999});
				
			StateLayer statelayer;
			statelayer.initLayers(state, SIZE);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			int tilesize = statelayer.getLayers()[0].getQuads()[1].position.x - statelayer.getLayers()[0].getQuads()[0].position.x;
			sf::RenderWindow window(sf::VideoMode(tilesize * (state.getGrid()[0].size() + 8), tilesize * state.getGrid().size()), "Test");
			statelayer.draw(window);
			
			RandomIA ai;
			
			
			while (window.isOpen())
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if(event.type == sf::Event::Closed){
						window.close();
					}
				}
				ai.run(engine,window);
				window.display();
			}
		}
		
		/*heuristic_ai : partie joué par des ia a l'inteligence basic*/
		else if(strcmp(argv[1],"heuristic_ai")==0){
			
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
			int SIZE = 30;	
				
			StateLayer statelayer;
			statelayer.initLayers(state, SIZE);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			int tilesize = statelayer.getLayers()[0].getQuads()[1].position.x - statelayer.getLayers()[0].getQuads()[0].position.x;
			sf::RenderWindow window(sf::VideoMode(tilesize * (state.getGrid()[0].size() + 8), tilesize * state.getGrid().size()), "Test");
			statelayer.draw(window);
			
			HeuristicIA heuristicAi;
			
			while (window.isOpen())
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if(event.type == sf::Event::Closed){
						window.close();
					}
				}
				heuristicAi.run(engine,window);
				window.display();
			}
		}
    cout << argv[1] << endl;
	}
    return 0;
}
