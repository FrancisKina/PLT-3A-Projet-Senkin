#include <string>
#include "TileSet.h"

using namespace render;

TileSet::TileSet(LayerId newIdLayer){
	/*Fonction instanciant la tuile associé au sprite d'un élément du jeu*/
	LayerId id=newIdLayer;
	
	if (id==LAYERFIELD){
		cellWidth=20;
		cellHeight=20;
		imageFile="res/Map/texturetest3.png";
	} else if (id==LAYERFIELDEFFECT){
		cellWidth=20;
		cellHeight=20;
		imageFile="res/Map/textureeffect.png";
	} else if (id==LAYERPLAYER){
		cellWidth=24;
		cellHeight=32;
		imageFile="res/Char/Characters.png";
	} else if (id==LAYERPLAYEREFFECT){
		cellWidth=20;
		cellHeight=20;
		imageFile="res/Char/iconeffect.png";
	} else if (id==LAYERCURSOR){
		cellWidth=20;
		cellHeight=20;
		imageFile="res/curseur3.png";
	} else if (id==LAYERINFOS){
		cellWidth=21;
		cellHeight=24;
		imageFile="res/font.png";
	} else if (id==LAYERINFOSBACK){
		cellWidth=40;
		cellHeight=40;
		imageFile="res/wood.png";
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


