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
	
	cout << endl;
	
	int x = player->getX(), y = player->getY();
	int dx = destination.first, dy = destination.second;
	int distance = abs(x-dx) + abs(y-dy);
	
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
