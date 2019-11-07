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
	currentState.notifyObservers(currentState, window);
}

state::State& Engine::getState(){
	state::State& refState=currentState;
	return refState;
}

