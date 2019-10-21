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
/*

StateLayer::StateLayer(state::State& state){
/*
	TileSet tilesetField(FIELDTILESET);
	std::unique_ptr<TileSet> ptr_tilesetField (new TileSet(tilesetField));
	tilesets.push_back(move(ptr_tilesetField));
	
	TileSet tilesetPersonnages(PERSONNAGETILESET);
	std::unique_ptr<TileSet> ptr_tilesetPersonnages (new TileSet(tilesetPersonnages));
	tilesets.push_back(move(ptr_tilesetPersonnages));
	
	TileSet tilesetCurseur(CURSEURTILESET);
	std::unique_ptr<TileSet> ptr_tilesetCurseur (new TileSet(tilesetCurseur));
	tilesets.push_back(move(ptr_tilesetCurseur));

	TileSet tilesetInfos(INFOSTILESET);
	std::unique_ptr<TileSet> ptr_tilesetInfos (new TileSet(tilesetInfos));
	tilesets.push_back(move(ptr_tilesetInfos));
	
	
}

void StateLayer::initLayers(state::State& state){	
	Layer surfField;
	Layer surfPersonnage;
	Layer surfCurseur;
	Layer surfInfos;
	
	surfField.loadField(state, tilesets[0]->getTexture(), sf::Vector2u(tilesets[0]->getCellWidth(), tilesets[0]->getCellHeight()), state.getField().size(), state.getField()[0].size());

	surfPersonnage.loadPersonnage(state, tilesets[1]->getTexture(), sf::Vector2u(tilesets[1]->getCellWidth(), tilesets[1]->getCellHeight()), state.getPlayers().size(), 1);

	surfCurseur.loadCurseur(state, tilesets[2]->getTexture(), sf::Vector2u(tilesets[2]->getCellWidth(), tilesets[2]->getCellHeight()), 1, 1);
	
	//surfInfos.loadInfos(state, tilesets[3]->getTexture(), sf::Vector2u(tilesets[3]->getCellWidth(), tilesets[3]->getCellHeight()), 1, 1);
	surfInfos.loadInfos(state, tilesets[3]->getTexture(), sf::Vector2u(tilesets[3]->getCellWidth(), tilesets[3]->getCellHeight()), state.getPlayers().size(), 1);
	
	std::unique_ptr<Layer> ptr_surfField (new Layer(surfField));
	std::unique_ptr<Layer> ptr_surfPersonnage (new Layer(surfPersonnage));
	std::unique_ptr<Layer> ptr_surfCurseur (new Layer(surfCurseur));
	std::unique_ptr<Layer> ptr_surfInfos (new Layer(surfInfos));
	
	if(layers.size()!=0){
		while(layers.size()!=0){
			layers.pop_back();
		}
	}
	
	layers.push_back(move(ptr_surfField));
	layers.push_back(move(ptr_surfPersonnage));
	layers.push_back(move(ptr_surfCurseur));
	layers.push_back(move(ptr_surfInfos));
	
}

std::vector<std::unique_ptr<TileSet>>& StateLayer::getTilesets (){
	std::vector<std::unique_ptr<TileSet>>& ref_tilesets = tilesets;
	return ref_tilesets;
}

std::vector<std::unique_ptr<Layer>>& StateLayer::getLayers (){
	std::vector<std::unique_ptr<Layer>>& ref_layers = layers;
	return ref_layers;
}*/
