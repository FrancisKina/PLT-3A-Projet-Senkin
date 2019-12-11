#include "Command.h"

using namespace engine;

CommandID Command::getCommandID(){
	return id;
}

int Command::getN(){
	return n;
}

std::pair<int,int> Command::getPos(){
	return pos;
}
