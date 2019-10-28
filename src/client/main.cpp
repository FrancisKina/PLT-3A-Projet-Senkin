#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <time.h>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"
#include "render.h"

using namespace std;
using namespace state;
using namespace render;


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
    cout << argv[1] << endl;
	}
    return 0;
}
