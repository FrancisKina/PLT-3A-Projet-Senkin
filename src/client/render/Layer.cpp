#include "Layer.h"
#include <state.h>

using namespace render;
using namespace state;


bool Layer::loadField(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
	
		//std::vector<std::vector<std::pair<state::FieldTypeId, std::pair<FieldStatusId,int>>>>& grid = state.getGrid();
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
		
		// création de tous les quads
		for(size_t i=0; i<state.getGrid().size(); i++){
			for(size_t j=0; j<state.getGrid()[0].size(); j++){
				sf::Vertex* quad = &quads[(i * state.getGrid()[0].size()+ j) * 4];
				
				//Position des 4 coins du quad
				quad[0].position = sf::Vector2f(tileSize * j, tileSize * i);
				quad[1].position = sf::Vector2f(tileSize * (j + 1), tileSize * i);
				quad[2].position = sf::Vector2f(tileSize * (j + 1), tileSize * (i + 1));
				quad[3].position = sf::Vector2f(tileSize * j, tileSize * (i + 1));
				
				//Position de la texture
				quad[0].texCoords = sf::Vector2f(textSize.x * (state.getGrid()[i][j].first - 1), 0);
				quad[1].texCoords = sf::Vector2f(textSize.x * state.getGrid()[i][j].first, 0);
				quad[2].texCoords = sf::Vector2f(textSize.x * state.getGrid()[i][j].first, textSize.y);
				quad[3].texCoords = sf::Vector2f(textSize.x * (state.getGrid()[i][j].first - 1), textSize.y);
			
			}
		}			
		return true;				
}


bool Layer::loadPlayer(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
    		
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
       	
        // on remplit le tableau de vertex, avec un quad par tuile
        for (size_t i = 0; i < width; ++i){
              	sf::Vertex* quad = &quads[i * 4];
				
				//Position des 4 coins du quad (x, y)
				quad[0].position = sf::Vector2f(tileSize * state.getPlayers()[i]->getX() + tileSize/6.66, tileSize * state.getPlayers()[i]->getY() + tileSize/tileSize); //Haut gauche
				quad[1].position = sf::Vector2f(tileSize * (state.getPlayers()[i]->getX() + 1) - tileSize/6.66, tileSize * state.getPlayers()[i]->getY() + tileSize/tileSize); //Haut droite
				quad[2].position = sf::Vector2f(tileSize * (state.getPlayers()[i]->getX() + 1) - tileSize/6.66, tileSize * (state.getPlayers()[i]->getY() + 1) - tileSize/10); //Bas droite
				quad[3].position = sf::Vector2f(tileSize * state.getPlayers()[i]->getX() + tileSize/6.66, tileSize * (state.getPlayers()[i]->getY() + 1) - tileSize/10); //Bas gauche
				
				//Position de la texture;			
				quad[0].texCoords = sf::Vector2f(textSize.x * (2 - 1), (3 - 1) * textSize.y);
				quad[1].texCoords = sf::Vector2f(textSize.x * 2, (3 - 1) * textSize.y);
				quad[2].texCoords = sf::Vector2f(textSize.x * 2, 3 * textSize.y);
				quad[3].texCoords = sf::Vector2f(textSize.x * (2 - 1), 3 * textSize.y);
		}

		return true;
}
/*
bool Layer::loadCurseur(state::State& stateLayer, sf::Texture& textureTileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){
    			
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
		
	  	// on récupère le numéro de tuile courant
		int tileNumber=stateLayer.getCurseur()->getCodeTuile();
		
	    // on en déduit sa position dans la texture du tileset
	    int tu = tileNumber % (texture.getSize().x / tileSize.x);
	    int tv = tileNumber / (texture.getSize().x / tileSize.x);

	    // on récupère un pointeur vers le quad à définir dans le tableau de vertex
	    sf::Vertex* quad = &quads[0];
		
		// on définit ses quatre coins
		quad[0].position = sf::Vector2f(stateLayer.getCurseur()->getPosition().getX() * tileSize.x, stateLayer.getCurseur()->getPosition().getY() * tileSize.y);
		quad[1].position = sf::Vector2f((stateLayer.getCurseur()->getPosition().getX()+ 1) * tileSize.x, stateLayer.getCurseur()->getPosition().getY() * tileSize.y);
		quad[2].position = sf::Vector2f((stateLayer.getCurseur()->getPosition().getX() + 1) * tileSize.x, (stateLayer.getCurseur()->getPosition().getY() + 1) * tileSize.y);
		quad[3].position = sf::Vector2f(stateLayer.getCurseur()->getPosition().getX() * tileSize.x, (stateLayer.getCurseur()->getPosition().getY() + 1) * tileSize.y);
		
		// on définit ses quatre coordonnées de texture
		quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
		quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
		quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
		quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		

		return true;
}

bool Layer::loadInfos(state::State& stateLayer, sf::Texture& textureTileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){

	texture = textureTileset;
		
  	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
   	quads.setPrimitiveType(sf::Quads);
   	quads.resize(width * height * 4);
    // on remplit le tableau de vertex, avec un quad par tuile
    for (unsigned int i = 0; i < width; ++i){
    	if(stateLayer.getPersonnages()[i]->getStatut()==state::SELECTIONNE || stateLayer.getPersonnages()[i]->getStatut()==state::CIBLE){
          	// on récupère le numéro de tuile courant
			int tileNumber=stateLayer.getPersonnages()[i]->getCodeTuile();
			
            // on en déduit sa position dans la texture du tileset
            int tu = tileNumber % (texture.getSize().x / tileSize.x);
            int tv = tileNumber / (texture.getSize().x / tileSize.x);

            // on récupère un pointeur vers le quad à définir dans le tableau de vertex
            sf::Vertex* quad = &quads[i * 4];
			
			if (stateLayer.getPersonnages()[i]->getCamp()==true){
				quad[0].position = sf::Vector2f(5+64,440);
				quad[1].position = sf::Vector2f(0+5,440);
				quad[3].position = sf::Vector2f(5+64,440+64);
				quad[2].position = sf::Vector2f(0+5,440+64);
			}
				
			// Personnage de l'armee rouge affiché à droite
			else{
				quad[0].position = sf::Vector2f(400-5-64,440);
				quad[1].position = sf::Vector2f(400-5,440);
				quad[2].position = sf::Vector2f(400-5,440+64);
				quad[3].position = sf::Vector2f(400-5-64,440+64);		
			}
			
// on définit ses quatre coordonnées de texture
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}
	}

	return true;
}
*/
void Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // on applique la transformation
        states.transform *= getTransform();

        // on applique la texture du tileset
        states.texture = &texture;

        // et on dessine enfin le tableau de vertex
        target.draw(quads, states);
}

sf::VertexArray Layer::getQuads(){
	return quads;
}

