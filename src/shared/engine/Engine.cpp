#include "Engine.h"
#include <iostream>
#include <unistd.h>

using namespace state;
using namespace engine;
using namespace std;

Engine::Engine(){
}

void Engine::executeCommand(Command* command, sf::RenderWindow& window){
	command->execute(currentState);
	
	std::vector<state::Player*> players = currentState.getPlayers();
	for(size_t i=0; i<players.size(); i++) {
		if(players[i]->getHp() <= 0){
			players[i]->setX(-1);
			players[i]->setY(-1);
			cout << endl << players[i]->getName() <<  " est mort." << endl;
			players.erase(players.begin()+i);
		}
	}
	
	currentState.notifyObservers(currentState, window);
}

state::State& Engine::getState(){
	state::State& refState=currentState;
	return refState;
}

