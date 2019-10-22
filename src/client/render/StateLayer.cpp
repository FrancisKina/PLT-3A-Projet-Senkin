#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "StateLayer.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <unistd.h>


using namespace render;
using namespace std;
using namespace state;

/*Creation de la liste contenant chaque type de tuile avec la texture associée*/
StateLayer::StateLayer(){

	TileSet tilesetField(FROG,LAYERFIELD);
	tilesets.push_back(tilesetField);
/*	
	TileSet tilesetField(FROG,LAYERFIELD);
	tilesets.push_back(tilesetField);
	
	TileSet tilesetPlayer(LAYERPLAYER);
	tilesets.push_back(layerPlayer));
	
	TileSet tilesetCursor(LAYERCURSOR);
	tilesets.push_back(layerCursor);

	TileSet tilesetInfos(INFOSTILESET);
	std::unique_ptr<TileSet> ptr_tilesetInfos (new TileSet(tilesetInfos));
	tilesets.push_back(move(ptr_tilesetInfos));
*/	
	
}

/*Creation de la liste contenant tous les layers texturées*/
void StateLayer::initLayers(state::State& state){	
	Layer layerField;
	Layer layerPersonnage;
	Layer layerCurseur;
	Layer layerInfos;
	
	layerField.loadField(state, tilesets[0].getTexture(), sf::Vector2u(tilesets[0].getCellWidth(), tilesets[0].getCellHeight()), state.getGrid()[0].size(), state.getGrid().size());

	//surfPersonnage.loadPersonnage(state, tilesets[1]->getTexture(), sf::Vector2u(tilesets[1]->getCellWidth(), tilesets[1]->getCellHeight()), state.getPlayers().size(), 1);

	//surfCurseur.loadCurseur(state, tilesets[2]->getTexture(), sf::Vector2u(tilesets[2]->getCellWidth(), tilesets[2]->getCellHeight()), 1, 1);
	
	//surfInfos.loadInfos(state, tilesets[3]->getTexture(), sf::Vector2u(tilesets[3]->getCellWidth(), tilesets[3]->getCellHeight()), 1, 1);
	//surfInfos.loadInfos(state, tilesets[3]->getTexture(), sf::Vector2u(tilesets[3]->getCellWidth(), tilesets[3]->getCellHeight()), state.getPlayers().size(), 1);
	
	//std::unique_ptr<Layer> ptr_surfField (new Layer(surfField));
	//std::unique_ptr<Layer> ptr_surfPersonnage (new Layer(surfPersonnage));
	//std::unique_ptr<Layer> ptr_surfCurseur (new Layer(surfCurseur));
	//std::unique_ptr<Layer> ptr_surfInfos (new Layer(surfInfos));
	
	/*Vider layers*/
	if(layers.size()!=0){
		while(layers.size()!=0){
			layers.pop_back();
		}
	}
	
	layers.push_back(layerField);
	//layers.push_back(move(ptr_surfPersonnage));
	//layers.push_back(move(ptr_surfCurseur));
	//layers.push_back(move(ptr_surfInfos));
	
}

std::vector<TileSet>& StateLayer::getTilesets (){
	std::vector<TileSet>& ref_tilesets = tilesets;
	return ref_tilesets;
}

std::vector<Layer>& StateLayer::getLayers (){
	std::vector<Layer>& ref_layers = layers;
	return ref_layers;
}
