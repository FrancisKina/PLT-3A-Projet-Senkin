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

using namespace std;
using namespace state;
using namespace render;
using namespace engine;


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
			state.getPlayers()[2]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(ARC)});
			
			state.sortPlayers(); // Trier les personnages par initiative pour l'ordre d'action et donner le tour d'action au premier
				//Initialisation du curseur
			state.initCursor();
			state.getCursor()->setCursorX(rand()%state.getGrid()[0].size());
			state.getCursor()->setCursorY(rand()%state.getGrid().size());
			
				//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state, 35);
			state.registerObserver(&statelayer);
			
				//Creation puis affichage de la fenêtre
			int tilesize = statelayer.getLayers()[0].getQuads()[1].position.x - statelayer.getLayers()[0].getQuads()[0].position.x;
			sf::RenderWindow window(sf::VideoMode(tilesize * state.getGrid()[0].size(), tilesize * state.getGrid().size()), "Test");
			window.draw(statelayer.getLayers()[0]);//Affichage terrain
			window.draw(statelayer.getLayers()[1]);//Affichage personnages
			window.draw(statelayer.getLayers()[2]);//Affichage curseur
			window.display();
			
				//Suite de commandes
			std::vector<Command*> commandList;
			Move* move1 = new Move({11,11});
			commandList.push_back(move1);
			Move* move2 = new Move({11,13});
			commandList.push_back(move2);
			commandList.push_back(move1);
			EndActions* endactions = new EndActions();
			commandList.push_back(endactions);
			
			Move* move3 = new Move({11,13});
			commandList.push_back(move3);
			Move* move4 = new Move({19,12});
			commandList.push_back(move4);
			commandList.push_back(endactions);
			
			Move* move5 = new Move({11,11});
			commandList.push_back(move5);
			Move* move6 = new Move({11,12});
			commandList.push_back(move6);
			//std::pair<int,int> p = {11,13};
			//Attack* attack1 = new Attack(p , *state.getPlaying()->getSkills()[0]);
			//commandList.push_back(attack1);
			
			bool finCommande = false;
			
			while (window.isOpen())
			{
				// on gère les évènements
				sf::Event event;
				while (window.pollEvent(event))
				{
					if(event.type == sf::Event::Closed)
						window.close();
				}
				
				sleep(2);
				if (commandList.size() > 0){
					engine.executeCommand(commandList[0],window);
					commandList.erase(commandList.begin());
				}
				else if(finCommande == false){
					cout << "Fin de commande" << endl;
					finCommande = true;
				}
				window.display();
				
			}
		}
    cout << argv[1] << endl;
	}
    return 0;
}
