#include "Layer.h"

using namespace render;

/*
bool Layer::loadField(state::State& stateLayer, sf::Texture& textureTileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){
    			
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);

        // on remplit le tableau de vertex, avec un quad par tuile
        for (unsigned int i = 0; i < width; i++){
            for (unsigned int j = 0; j < height; j++){

            	// on récupère le numéro de tuile courant
				int tileNumber=i+j*tileSize.x;
				
                // on en déduit sa position dans la texture du tileset
                int tu = tileNumber % (texture.getSize().x / tileSize.x);
                int tv = tileNumber / (texture.getSize().x / tileSize.x);

                // on récupère un pointeur vers le quad à définir dans le tableau de vertex
                sf::Vertex* quad = &quads[(i + j * width) * 4];
				
				// on définit ses quatre coins
				quad[0].position = sf::Vector2f(stateLayer.getField()[j][i]->getPosition().getX() * tileSize.x, stateLayer.getField()[j][i]->getPosition().getY() * tileSize.y);
				quad[1].position = sf::Vector2f((stateLayer.getField()[j][i]->getPosition().getX()+ 1) * tileSize.x, stateLayer.getField()[j][i]->getPosition().getY() * tileSize.y);
				quad[2].position = sf::Vector2f((stateLayer.getField()[j][i]->getPosition().getX() + 1) * tileSize.x, (stateLayer.getField()[j][i]->getPosition().getY() + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(stateLayer.getField()[j][i]->getPosition().getX() * tileSize.x, (stateLayer.getField()[j][i]->getPosition().getY() + 1) * tileSize.y);
				
// on définit ses quatre coordonnées de texture
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
							
           }
		}
		return true;				
}


bool Surface::loadPersonnage(state::State& stateLayer, sf::Texture& textureTileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){
    		
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
        // on remplit le tableau de vertex, avec un quad par tuile
        for (unsigned int i = 0; i < width; ++i){
        		//On met a jour les tuiles des personnages en fonction de leur statut
        		updateTilesPersonnages(stateLayer);
              	// on récupère le numéro de tuile courant
				int tileNumber=stateLayer.getPersonnages()[i]->getCodeTuile();
				
                // on en déduit sa position dans la texture du tileset
                int tu = tileNumber % (texture.getSize().x / tileSize.x);
                int tv = tileNumber / (texture.getSize().x / tileSize.x);

                // on récupère un pointeur vers le quad à définir dans le tableau de vertex
                sf::Vertex* quad = &quads[i * 4];
				
				// on définit ses quatre coins
				quad[0].position = sf::Vector2f(stateLayer.getPersonnages()[i]->getPosition().getX() * tileSize.x, stateLayer.getPersonnages()[i]->getPosition().getY() * tileSize.y);
				quad[1].position = sf::Vector2f((stateLayer.getPersonnages()[i]->getPosition().getX()+ 1) * tileSize.x, stateLayer.getPersonnages()[i]->getPosition().getY() * tileSize.y);
				quad[2].position = sf::Vector2f((stateLayer.getPersonnages()[i]->getPosition().getX() + 1) * tileSize.x, (stateLayer.getPersonnages()[i]->getPosition().getY() + 1) * tileSize.y);
				quad[3].position = sf::Vector2f(stateLayer.getPersonnages()[i]->getPosition().getX() * tileSize.x, (stateLayer.getPersonnages()[i]->getPosition().getY() + 1) * tileSize.y);
				
// on définit ses quatre coordonnées de texture
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}

		return true;
}

bool Surface::loadCurseur(state::State& stateLayer, sf::Texture& textureTileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){
    			
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

bool Surface::loadInfos(state::State& stateLayer, sf::Texture& textureTileset, sf::Vector2u tileSize, unsigned int width, unsigned int height){

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

void Surface::draw(sf::RenderTarget& target, sf::RenderStates states) const {/*
        // on applique la transformation
        states.transform *= getTransform();

        // on applique la texture du tileset
        states.texture = &texture;

        // et on dessine enfin le tableau de vertex
        target.draw(quads, states);
}
*/

