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

#include <state.h>

using namespace std;
using namespace state;

int main(int argc,char* argv[])
{
    if (argc>1){
		
		/*	render : Affichage du rendu de la map */
		if(strcmp(argv[1],"render")==0){
			//Initialisation de la grille
			State state_init;
			state_init.initGrid("res/maptest.txt");
			std::vector<std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>>>& grid = state_init.getGrid();
			std::string tileset = "res/Map/texturetest.png";
			
			sf::VertexArray quads(sf::Quads, 4 * grid.size() * grid[0].size());
			int tilesize = 50;
			int textsize = 20;
			
			for(size_t i=0; i<grid.size(); i++){
				for(size_t j=0; j<grid[0].size(); j++){
					sf::Vertex* quad = &quads[(i * grid[0].size()+ j) * 4];
					
					
					//Position des 4 coins du quad
					quad[0].position = sf::Vector2f(tilesize * j, tilesize * i);
					quad[1].position = sf::Vector2f(tilesize * (j + 1), tilesize * i);
					quad[2].position = sf::Vector2f(tilesize * (j + 1), tilesize * (i + 1));
					quad[3].position = sf::Vector2f(tilesize * j, tilesize * (i + 1));
					
					//Position de la texture
					quad[0].texCoords = sf::Vector2f(textsize * (grid[i][j].first - 1), 0);
					quad[1].texCoords = sf::Vector2f(textsize * grid[i][j].first, 0);
					quad[2].texCoords = sf::Vector2f(textsize * grid[i][j].first, textsize);
					quad[3].texCoords = sf::Vector2f(textsize * (grid[i][j].first - 1), textsize);
			
				}
			}
			
			sf::RenderWindow window(sf::VideoMode(tilesize * grid[0].size(), tilesize * grid.size()), "Test");
			cout << grid[0].size() << "x" << grid.size() << "\n";
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
				
				sf::RenderStates states;
				sf::Texture m_tileset;
				
				states.texture = &m_tileset;
				m_tileset.loadFromFile(tileset);
				window.setFramerateLimit(60);
				window.draw(quads,states);
				window.display();
			}
		}
    cout << argv[1] << endl;
	}
    return 0;
}
