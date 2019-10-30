#include <string>
#include "TileSet.h"

using namespace render;

TileSet::TileSet(LayerId newIdLayer){
	/*Fonction instanciant la tuile associé au sprite d'un élément du jeu*/
	LayerId id=newIdLayer;
	
	if (id==LAYERFIELD){
		//TODO: adjust value and path
		cellWidth=20;
		cellHeight=20;
		imageFile="res/Map/texturetest3.png";
	} else if (id==LAYERPLAYER){
		cellWidth=24;
		cellHeight=32;
		imageFile="res/Char/Characters.png";
	} else if (id==LAYERCURSOR){
		//todo: adjust value
		cellWidth=500;
		cellHeight=500;
		imageFile="res/curseur.png";
	}
	
	textureTileset.loadFromFile(imageFile);
}

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


