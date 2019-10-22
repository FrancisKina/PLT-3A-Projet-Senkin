#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>

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
			
			//Initialisation de la liste des différents layers avec texture
			StateLayer statelayer;
			statelayer.initLayers(state_init);
			
			//Paramétrage puis affichage de la fenêtre
			int tilesize = 50;
			sf::RenderWindow window(sf::VideoMode(tilesize * state_init.getGrid()[0].size(), tilesize * state_init.getGrid().size()), "Test");
			
			// on fait tourner la boucle principale
			while (window.isOpen())
			{
				// on gère les évènements
				sf::Event event;
				while (window.pollEvent(event))
				{
					if(event.type == sf::Event::Closed)
						window.close();
				}

				// on dessine le niveau
				window.clear();
				
				window.draw(statelayer.getLayers()[0]);//Affichage terrain
				window.display();
			}
		}
    cout << argv[1] << endl;
	}
    return 0;
}
