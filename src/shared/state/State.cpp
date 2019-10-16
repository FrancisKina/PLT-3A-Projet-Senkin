#include "State.h"
#include <string>

using namespace state;

std::vector<std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>>> State::getGrid(){
	return grid;
}

std::vector<Player> State::getPlayers(){
	return players;
}

int State::getRound(){
	return round;
}

void State::setRound(int newRound){
	round = newRound;
}

void State::initCursor(){
}

void initPlayers(int nbPlayers){
}

void initGrid(std::string map){
}
