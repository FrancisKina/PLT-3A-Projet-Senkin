#include "Engine.h"
#include <iostream>
#include <unistd.h>
#include "engine.h"

using namespace state;
using namespace engine;
using namespace std;

Engine::Engine(){

}

void Engine::executeCommand(Command* command, sf::RenderWindow& window){
	command->execute(currentState);
	
	//Placement du curseur le joueur qui joue
	currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
	currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
	
	//Verification joueur mort
	std::vector<state::Player*> players = currentState.getPlayers();
	for(size_t i=0; i<players.size(); i++) {
		if(players[i]->getHp() <= 0){
			players[i]->setX(-1);
			players[i]->setY(-1);
			//cout << endl << players[i]->getName() <<  " est mort." << endl;
			players.erase(players.begin()+i);
		}
	}
	
	currentState.notifyObservers(currentState, window);
}

state::State& Engine::getState(){
	return currentState;
}

sf::RenderWindow& Engine::getWindow(){
	//currentWindow = new sf::RenderWindow(sf::VideoMode(tilesize * state.getGrid()[0].size(), tilesize * state.getGrid().size()), "Test");
	return currentWindow;
}

void Engine::startGame (state::State& state){
		//Initialisation du curseur
	currentState.initCursor();
	currentState.setCommandMode(FIELD);

	cout<<"Lancement du jeu"<<endl;
	currentState.setRound(1);
	currentState.sortPlayers();// Trier les personnages par initiative pour l'ordre d'action et donner le tour d'action au premier joueur
	cout << "Ordre de jeu : ";
	for (Player* p : currentState.getPlayers()){
		cout << p->getName() << " ";
	}
	cout << endl;
	
	//Placement du curseur le joueur qui joue
	currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
	currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
	
	//------------------------changment aléatoire d'état du terrain-----------------------

	cout << "Evenements aléatoires" << endl;
	int nzone = rand()%3+1;
	
	for(int i=0; i<nzone; i++){
		unsigned int x = rand()%state.getGrid()[0].size();
		unsigned int y = rand()%state.getGrid().size();
		unsigned int ntour = state.getPlayers().size() * (rand()%2+1);
		
		int chanceRain, chanceMist, chanceSnow, chanceBurn;
		FieldTypeId type = state.getGrid()[y][x]->getFieldType();
		if(type == MOUNTAIN) chanceRain=25, chanceMist=35, chanceSnow=40, chanceBurn = 0 ;
		else if (type == SAND) chanceRain=10, chanceMist=5, chanceSnow=5, chanceBurn = 80;
		else chanceRain=60, chanceMist=30, chanceSnow=10;
		
		int reffect = rand()%100+1;
		FieldStatusId effect;
		if(reffect<chanceRain) effect = RAIN;
		else if(reffect<chanceRain+chanceMist) effect = MIST;
		else if(reffect<chanceRain+chanceMist+chanceSnow) effect = SNOW;
		else if(reffect<chanceRain+chanceMist+chanceSnow+chanceBurn) effect = BURNING;
		
		int proba = 100;
		while(proba>0){
			if (rand()%100<proba){
				state.getGrid()[y][x]->updateFieldStatus(std::make_pair(effect, ntour));
				x += rand()%3-1;
				y += rand()%3-1;
				if (x<0) x=0;
				if (y<0) y=0;	
				if (x>=state.getGrid()[0].size()) x=state.getGrid()[0].size()-1;
				if (y>=state.getGrid().size()) y=state.getGrid().size()-1;	
			}
			proba-=5;
		}
	}
	
	cout << endl << "[ Début de round "<< currentState.getRound() <<"]" << endl;
	cout << endl << "---------- Tour du joueur " << currentState.getPlaying()->getName() << " ----------" << endl;
}

void Engine::keyCommand (sf::Event event, sf::RenderWindow& window){
	
	if (currentState.getPlaying()->getIa() == true) return;

	Cursor* cursor = currentState.getCursor();
	Cursor* cursorinfo = currentState.getCursorInfo();
	
	switch (event.key.code){
		//FLECHE GAUCHE
		case sf::Keyboard::Left:
			if (currentState.getCommandMode() == FIELD) {
				if (cursor->getCursorX() > 0) cursor->setCursorX(cursor->getCursorX() - 1);
				else cursor->setCursorX(0);	
			}
			break;
		//FLECHE HAUT
		case sf::Keyboard::Up:
			if (currentState.getCommandMode() == FIELD) {
				if (cursor->getCursorY() > 0) cursor->setCursorY(cursor->getCursorY() - 1);
				else cursor->setCursorY(0);	
			}
			else if (currentState.getCommandMode() == COMMAND) {
				if (cursorinfo->getCursorY() > 20) cursorinfo->setCursorY(cursorinfo->getCursorY() - 1);
				else cursorinfo->setCursorY(20);	
			}
			break;
		//FLECHE DROITE
		case sf::Keyboard::Right:
			if (currentState.getCommandMode() == FIELD) {
				if (cursor->getCursorX() < (int)currentState.getGrid()[0].size()-1) cursor->setCursorX(cursor->getCursorX() + 1);
				else cursor->setCursorX(currentState.getGrid()[0].size()-1);	
			}
			break;
		//FLECHE BAS	
		case sf::Keyboard::Down:
			if (currentState.getCommandMode() == FIELD) {
				if (cursor->getCursorY() < (int)currentState.getGrid().size()-1) cursor->setCursorY(cursor->getCursorY() + 1);
				else cursor->setCursorY(currentState.getGrid().size()-1);
			}
			else if (currentState.getCommandMode() == COMMAND) {
				if (cursorinfo->getCursorY() < 22) cursorinfo->setCursorY(cursorinfo->getCursorY() + 1);
				else cursorinfo->setCursorY(22);	
			}
			break;
			
		//ESPACE OU ENTREE
		case sf::Keyboard::Space:	
		case sf::Keyboard::Return:
			if (currentState.getCommandMode() == FIELD) {
				currentState.setCommandMode(COMMAND);
				cursorinfo->setCursorY(20);
				break;
			}
			else if (currentState.getCommandMode() == COMMAND) {
				//EndActions* endactions = new EndActions();
				if (cursorinfo->getCursorY() == 20){
					currentState.setCommandMode(MOVEMENT);
				}
				else if (cursorinfo->getCursorY() == 21){ 
					currentState.setCommandMode(SKILL);
				}
				else if (cursorinfo->getCursorY() == 22) {
					currentState.setCommandMode(FIELD);
					executeCommand(new EndActions(), currentWindow);
				}
				break;
			}
			
			/*
				for (Player* player: currentState.getPlayers()){
					if (cursor->getCursorX() == player->getX() && cursor->getCursorY() == player->getY())
				}

				break;
			*/
				
		default:
			break;
	}
	
	currentState.notifyObservers(currentState, window);

}
