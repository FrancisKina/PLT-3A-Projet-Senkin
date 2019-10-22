#include <string>
#include "TileSet.h"

using namespace render;

TileSet::TileSet(state::ClassId newIdClass, LayerId newIdLayer){
	/**Fonction instantiant la tuile associé au sprite d'un élément du jeu
	 * **/
	LayerId id=newIdLayer;
	
	if (id==LAYERFIELD){
		//TODO: adjust value and path
		cellWidth=16;
		cellHeight=16;
		imageFile="res/terrain_tileset.png";
	} else if (id==LAYERPLAYER){
		if (newIdClass==state::KNIGHT){
			imageFile="res/Char/Chevalier-Kiza.png";
		} else 
		if (newIdClass==state::FROG){;
			imageFile="res/DreamingFrog.png";
		} else 
		if (newIdClass==state::ARCHER){
			imageFile="res/Hope_Arc.png";
		} else 
		if (newIdClass==state::DWARF){
			imageFile="res/NainDesMonts.png";
		}
		cellWidth=16;
		cellHeight=16;
	} else if (id==LAYERCURSOR){
		//todo: adjust value
		cellWidth=16;
		cellHeight=16;
		imageFile="res/curseur_tileset.png";
	}
	
	textureTileset.loadFromFile(imageFile);
}
/*
const int TileSet::getTileSetID(){
	return id;
}
*/
const int TileSet::getCellWidth(){
	return cellWidth;
}

const int TileSet::getCellHeight(){
	return cellHeight;
}

const std::string  TileSet::getImageFile (){
	return imageFile;
}

sf::Texture& TileSet::getTexture(){
	sf::Texture & refTexture = textureTileset;
	return refTexture ;
}


