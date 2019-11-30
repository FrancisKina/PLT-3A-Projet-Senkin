#include "Move.h"
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

int sgn(int val){
	return (val>=0) - (val<=0);
}


Move::Move(std::pair<int,int> newDestination){
	id = MOVE;
	destination = newDestination;
}

void Move::execute (state::State& state){
	player = state.getPlaying();
	
	cout << endl << "| Deplacement |" << endl;
	
	int x = player->getX(), y = player->getY();
	int dx = destination.first, dy = destination.second;
	
	//Cout du deplacement en fonction du type de mouvement et de la destination
	FieldTypeId destinationtype = state.getGrid()[dy][dx]->getFieldType();
	MovementId movementtype = player->getCharacter()->getMovementType();
	int distance;
	switch (movementtype){
		case NORMAL:
			distance = 1 + 2 * (destinationtype == SWAMP) + 2 * (destinationtype == MOUNTAIN) + (destinationtype == FOREST) + (destinationtype == SAND);
			break;
		case AGILE:
			distance = 1 + 2 * (destinationtype == SWAMP) + 2 * (destinationtype == SAND) + (destinationtype == MOUNTAIN);
			break;
		case NATURE:	
			distance = 1 + 2 * (destinationtype == SAND) + 2 * (destinationtype == CITY) + (destinationtype == SWAMP);
			break;
		case RIDING:	
			distance = 1 + 2 * (destinationtype == SWAMP);
	}
	
	//Test PM suffisant
	if (player->getMovement() - distance < 0){
		cout << "Deplacement impossible en (" << dx << "," << dy << ") : " << distance << " PM requis, " << player->getMovement() << " restants" << endl;
		return;
	}
	
	//Test case libre
	for(state::Player* player : state.getPlayers()){
		if (player->getX() == dx && player->getY() == dy){
			cout << "Deplacement impossible en (" << dx << "," << dy << ") : " << "Joueur present sur la case" << endl;
			return;
		}
	}
	
	//Test case bloquee
	if (state.getGrid()[dy][dx]->getFieldStatus()[11].second > 0){
		cout << "Deplacement impossible en (" << dx << "," << dy << ") : " << "Destination bloquee" << endl;
		return;
	}

	//Direction du joueur
	if (dx-x<0) player->setDirection(WEST);
	else if (dx-x>0) player->setDirection(EAST);
	else if (dy-y<0) player->setDirection(NORTH);
	else if (dy-y>0) player->setDirection(SOUTH);
	
	//Deplacement du joueur
	cout << player->getName() << " se deplace de (" << x << "," << y << ") a (" << dx << "," << dy << ") : " << distance << " PM utilises, ";
	player->setX(dx);
	player->setY(dy);
	
	//Maj points de mouvement
	player->setMovement(player->getMovement() - distance);
	cout << player->getMovement() << " PM restants" << endl;
	
	//Effets de terrain
	state::Field* field = state.getGrid()[dy][dx];
	int testEffect;
	for (size_t j=0; j<field->getFieldStatus().size(); j++){
		if(field->getFieldStatus()[j].first == POISON && field->getFieldStatus()[j].second > 0){
			testEffect = rand()%100 + 1;
			if (testEffect > 70){
				cout << "Terrain : le joueur est empoisonne" << endl;
				player->updateStatus({POISONED,2});
			}
		}
		if(field->getFieldStatus()[j].first == BURNING && field->getFieldStatus()[j].second > 0){
			testEffect = rand()%100 + 1;
			if (testEffect > 50){
				cout << "Terrain : le joueur brule" << endl;
				player->updateStatus({BURNED,2});
			}
		}
	}
}
