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
using namespace sf;

/*Creation de la liste contenant chaque type de tuile avec la texture associée*/
StateLayer::StateLayer(){
	
	TileSet tilesetField(LAYERFIELD);
	tilesets.push_back(tilesetField);
	
	TileSet tilesetFieldEffect(LAYERFIELDEFFECT);
	tilesets.push_back(tilesetFieldEffect);

	TileSet tilesetPlayer(LAYERPLAYER);
	tilesets.push_back(tilesetPlayer);
	
	TileSet tilesetPlayerEffect(LAYERPLAYEREFFECT);
	tilesets.push_back(tilesetPlayerEffect);
	
	TileSet tilesetCursor(LAYERCURSOR);
	tilesets.push_back(tilesetCursor);

	TileSet tilesetInfos(LAYERINFOS);
	tilesets.push_back(tilesetInfos);
	
}

/*Creation de la liste contenant tous les layers texturées*/
void StateLayer::initLayers(state::State& state, int newTileSize){
	tileSize = newTileSize;
	
	Layer layerField;
	Layer layerFieldEffect;
	Layer layerPlayer;
	Layer layerPlayerEffect;
	Layer layerFieldMeteo;
	Layer layerCursor;
	Layer layerInfos;
	Layer layerBuilding;
	
	layerField.loadField(state, tilesets[0].getTexture(), sf::Vector2u(tilesets[0].getCellWidth(), tilesets[0].getCellHeight()), state.getGrid()[0].size(), state.getGrid().size(), tileSize);
	
	layerFieldEffect.loadFieldEffect(state, tilesets[1].getTexture(), sf::Vector2u(tilesets[1].getCellWidth(), tilesets[1].getCellHeight()), state.getGrid()[0].size(), state.getGrid().size(), tileSize, 0);

	layerPlayer.loadPlayer(state, tilesets[2].getTexture(), sf::Vector2u(tilesets[2].getCellWidth(), tilesets[2].getCellHeight()), state.getPlayers().size(), 1, tileSize);
	
	layerPlayerEffect.loadPlayerEffect(state, tilesets[3].getTexture(), sf::Vector2u(tilesets[3].getCellWidth(), tilesets[3].getCellHeight()), state.getPlayers().size(), 1, tileSize);
	
	layerBuilding.loadFieldEffect(state, tilesets[1].getTexture(), sf::Vector2u(tilesets[1].getCellWidth(), tilesets[1].getCellHeight()), state.getGrid()[0].size(), state.getGrid().size(), tileSize, 2);
	
	layerFieldMeteo.loadFieldEffect(state, tilesets[1].getTexture(), sf::Vector2u(tilesets[1].getCellWidth(), tilesets[1].getCellHeight()), state.getGrid()[0].size(), state.getGrid().size(), tileSize, 1);
	
	layerCursor.loadCursor(state, tilesets[4].getTexture(), sf::Vector2u(tilesets[4].getCellWidth(), tilesets[4].getCellHeight()), 2, 1, tileSize);
	
	layerInfos.loadInfos(state, tilesets[5].getTexture(), sf::Vector2u(tilesets[5].getCellWidth(), tilesets[5].getCellHeight()), 1, 1, tileSize);

	
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
	layers.push_back(layerFieldEffect);
	layers.push_back(layerPlayer);
	layers.push_back(layerPlayerEffect);
	layers.push_back(layerBuilding);
	layers.push_back(layerFieldMeteo);
	layers.push_back(layerCursor);
	layers.push_back(layerInfos);
	
}

std::vector<TileSet>& StateLayer::getTilesets (){
	std::vector<TileSet>& ref_tilesets = tilesets;
	return ref_tilesets;
}

std::vector<Layer>& StateLayer::getLayers (){
	std::vector<Layer>& ref_layers = layers;
	return ref_layers;
}

void StateLayer::displayLayers(state::State& state){
	//Creation puis affichage de la fenêtre
	int tilesize = getLayers()[0].getQuads()[1].position.x - getLayers()[0].getQuads()[0].position.x;
	sf::RenderWindow window(sf::VideoMode(tilesize * state.getGrid()[0].size(), tilesize * state.getGrid().size()), "Test");

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
		
		window.draw(getLayers()[0]);//Affichage terrain
		window.draw(getLayers()[1]);//Affichage personnages
		window.draw(getLayers()[2]);//Affichage curseur

		window.display();
	}
}

void StateLayer::stateChanged (state::State& state, sf::RenderWindow& window){
	initLayers(state,tileSize);
	draw(window);
}

void StateLayer::draw (sf::RenderWindow& window){
	window.clear();
	for(size_t i=0; i<layers.size(); i++){
		window.draw(getLayers()[i]);
	}
	window.display();
}
/*
void StateLayer::setWindow (sf::RenderWindow& newWindow){
	window = newWindow;
}
*/
