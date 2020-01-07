#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <ctime>
#include "../../extern/jsoncpp-1.8.0/jsoncpp.cpp"

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
#include "client.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;
using namespace client;


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
			state.getGrid()[10][10]->updateFieldStatus({BURNING,999});
			state.getGrid()[10][10]->updateFieldStatus({POISON,999});
			state.getGrid()[10][10]->updateFieldStatus({TENT,999});
			
			engine.startGame(state);
			/*
			state.sortPlayers(); // Trier les personnages par initiative podowsize * (state.getGrid()[0].size() + 8), windowsize * state.getGrid().size()),ur l'ordre d'action et donner le tour d'action au premier joueur
			
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
			commandList.push_back(new Move({11,14}));
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
						engine.executeCommand(commandList[0]);
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
			state.getPlayers()[3]->setIa(false);
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
			
			state.getGrid()[12][11]->updateFieldStatus({GRAVITY,999});
			state.getGrid()[10][10]->updateFieldStatus({BURNING,999});
			state.getGrid()[10][10]->updateFieldStatus({POISON,999});
			//state.getGrid()[10][10]->updateFieldStatus({SNOW,999});
			state.getGrid()[11][11]->updateFieldStatus({TENT,999});
			state.getGrid()[13][11]->updateFieldStatus({FORT,999});
			state.getGrid()[13][13]->updateFieldStatus({TOWER,999});
			
				//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state);
			statelayer.initWindow(state);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			sf::RenderWindow& window = statelayer.getWindow(); 
			window.setKeyRepeatEnabled(false);
			
			RandomIA ai;
			
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
				if(state.getPlayers().size()>1){
					ai.run(engine);
				}
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
			StateLayer statelayer;
			statelayer.initLayers(state);
			statelayer.initWindow(state);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			sf::RenderWindow& window = statelayer.getWindow(); 
			window.setKeyRepeatEnabled(false);
			
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
				if(state.getPlayers().size()>1){
					heuristicAi.run(engine);
				}
				window.display();
			}
		}
		/*rollback: retour arrière*/
		else if(strcmp(argv[1],"rollback")==0){
			
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
			window.setKeyRepeatEnabled(false);
			
			RandomIA ai;
			clock_t begin = clock();
			clock_t end = clock();
			bool stoprollback=false;
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
				if((end - begin)/ CLOCKS_PER_SEC<60){
					ai.run(engine);
					end = clock();
					cout<<endl<<endl<<"---------------------------------------------------"<<endl;
					cout<<"         temps clock = "<<(end - begin)/ CLOCKS_PER_SEC<<endl<<"-----------------------------------------"<<endl<<endl;
				}else if(!stoprollback){
					stoprollback=engine.rollBack();
					clock_t start_time = clock();
					while(clock()<start_time+1000000);
					//sleep(1);
				}
				window.display();
			}
		}
		
		else if(strcmp(argv[1],"deep_ai")==0){
			
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
			
			
			state.getGrid()[11][11]->updateFieldStatus({TENT,999});
			state.getGrid()[13][11]->updateFieldStatus({FORT,999});
			state.getGrid()[13][13]->updateFieldStatus({TOWER,999});
			
			//debut du jeu
			engine.startGame(state);
			
				//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state);
			statelayer.initWindow(state);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			sf::RenderWindow& window = statelayer.getWindow(); 
			
			DeepIA deepAi;
			
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
				if (state.getPlayers().size()>1){
					deepAi.run(engine);
				}
				window.display();
			}
		}
		/*record: enregistre les commandes*/
		else if(strcmp(argv[1],"record")==0){
			
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
			state.getPlayers()[0]->setIa(false);
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
			engine.setEnableRecord(true);
			engine.startGame(state);
			
				//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state);
			statelayer.initWindow(state);
			state.registerObserver(&statelayer);
				//Creation puis affichage de la fenêtre
			sf::RenderWindow& window = statelayer.getWindow(); 
			window.setKeyRepeatEnabled(false);
			
			
			Json::StyledWriter styledWriter;
			std::ofstream file_id;
			file_id.open("res/replay.txt");
			
			RandomIA ai;
			clock_t begin = clock();
			clock_t end = clock();
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
				if((end - begin)/ CLOCKS_PER_SEC<60){
					engine.getRecord();
					ai.run(engine);
					end = clock();
					cout<<endl<<endl<<"---------------------------------------------------"<<endl;
					cout<<"         temps clock = "<<(end - begin)/ CLOCKS_PER_SEC<<endl<<"-----------------------------------------"<<endl<<endl;
				}else{
					window.close();
				}
				engine.getState().notifyObservers(engine.getState());
				window.display();
			}
			file_id << styledWriter.write(engine.getRecord());

			file_id.close();
		}
		
		/*play : rejoue la partie enregistrée dans replay*/
		else if(strcmp(argv[1],"play")==0){
			
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
			state.getPlayers()[0]->setIa(false);
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
			window.setKeyRepeatEnabled(false);
			
			
			std::string fichier_commandes = "res/replay.txt";
			std::ifstream file_id(fichier_commandes);
			//file_id.open("res/replay.txt");
			
			bool partie_rejouee = false;
			sf::Event event;
			while (window.isOpen()){				
				// Au premier appui sur P, on ouvre le fichier et on execute les commandes
				if(partie_rejouee == false){// && sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
					
					cout << "--> Debut de la lecture <--" << endl;
					
					//Ouverture du fichier en lecture
					//std::ifstream commandes_txt(fichier_commandes);
					if (file_id){//commandes_txt){
						Json::Value root;
						Json::Reader reader;
						if(!reader.parse(file_id,root)){//commandes_txt, root)){
							cout 	<< "Failed to parse commandes\n"
									<< reader.getFormattedErrorMessages();
							return 0;
						}
						// Fermeture du fichier en lecture
						//commandes_txt.close();
						file_id.close();
						
						cout << "Taille du tableau de commandes de "<< fichier_commandes << " : " << root["tabCmd"].size() << endl;
											
						std::pair<int,int> pos;
						int n;
						std::string player;
						bool good_player=false;
						// Conversion en commandes
						for (unsigned int i = 0; i < root["tabCmd"].size(); i++){
							cout<<"test joueur"<<endl;
							// Recherche du joueur
							player=root["tabCmd"][i]["joueur"].asString();
							cout<<"test joueur"<<endl<<"joueur = "<<state.getPlaying()->getName()<<endl<<"commade = "<<player<<endl;
							if(root["tabCmd"][i]["joueur"].asString() == state.getPlaying()->getName()){
								good_player=true;
							}
						}
						if(good_player){
							for (unsigned int i = 0; i < root["tabCmd"].size(); i++){
								// Cas du deplacement
								if(root["tabCmd"][i]["id"].asUInt() == 1){
									cout<<"deplacement"<<endl;
								
									pos.first=root["tabCmd"][i]["xDestination"].asUInt();
									pos.second=root["tabCmd"][i]["yDestination"].asUInt();
									
									Move* move = new Move(pos);
									
									engine.executeCommand(move);
								}
								// Cas de l'attaque
								else if(root["tabCmd"][i]["id"].asUInt() == 2){
									cout<<"attaquz"<<endl;
								
									pos.first=root["tabCmd"][i]["xDestination"].asUInt();
									pos.second=root["tabCmd"][i]["yDestination"].asUInt();
									n=root["tabCmd"][i]["attaque"].asUInt();
									
									Attack* attack=new Attack(pos , n);
									
									engine.executeCommand(attack);
									
								}
								// Cas de la fin d'actions
								else if(root["tabCmd"][i]["id"].asUInt() == 3){
									cout<<"fin de tour"<<endl;
									
									EndActions* endactions = new EndActions();
									
									engine.executeCommand(endactions);
								}
								else{
									cout << "La commande " << i << " est inconnue" << endl;
								}
								engine.getState().notifyObservers(engine.getState());						
							}								
						}else{
							cout<<"pas le bon joueur:"<<endl<<"ordre de "<<state.getPlaying()->getName()<<" sur "<<good_player<<endl;
						}
					}
					
					else{
						cerr << "Impossible d'ouvrir le fichier des commandes enregistrées (lecture)." << endl;
						return 0;
					}
										
					cout << "--> Lecture Terminée <--" << endl;
					partie_rejouee = true;
				}
					
				
				while (window.pollEvent(event)){
					// Fermeture de la fenetre
					if (event.type == sf::Event::Closed){
						window.close();
					}
				}					
			}
				
		
		}
		
		else if(strcmp(argv[1],"thread")==0){
			Client client;
			client.run();
		}
		
    cout << argv[1] << endl;
	}
    return 0;
}
