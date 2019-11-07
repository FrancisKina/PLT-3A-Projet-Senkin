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
	
	int x = player->getX(), y = player->getY();
	int dx = destination.first, dy = destination.second;
	int distance = abs(x-dx) + abs(y-dy);
	
	if (player->getMovement() - distance < 0){
		cout << "Pas assez de PM pour aller en (" << dx << "," << dy << ") : " << distance << " necessaires, " << player->getMovement() << " restants" << endl;
		return;
	}
	
	//Generation de l'ensemble des coordonnees du chemin a prendre a partir de la position de depart et d'arrivee
	//TODO : AMELIORER PATHFINDING
	std::vector<std::pair<int,int>> path;
	/*while((x!=dx) || (y!=dy)){ //Recherche de chemin utilisant la plus grande distance
		if(abs(x-dx)>abs(y-dy)){
			x = x-sgn(x-dx);
		}
		else{
			y = y-sgn(y-dy);
		}
		path.push_back({x,y});
	}
	path.push_back({dx,dy});
	
	
	//Avancee pas a pas avec path
	for(size_t i=0; i<path.size();i++){ //Parcourir la liste des terrains du chemin
		state::Field* field = state.getGrid()[path[i].first][path[i].second]; 
		for(size_t j=0; j < field->getFieldStatus().size(); j++){ //Parcourir la liste des statuts du terrain
			if(field->getFieldStatus()[j].first == POISON && field->getFieldStatus()[j].second > 0){
				cout << "Terrain : Joueur empoisonne" << endl;
			}
			if(field->getFieldStatus()[j].first == BURNING && field->getFieldStatus()[j].second > 0){
				cout << "Terrain : Joueur brule" << endl;
			}
		}
	}
	*/
	//Nouvelle position du joueur
	player->setX(dx);
	player->setY(dy);
	cout << "Joueur se déplace en (" << dx << "," << dy << ")" << endl;
	
	//Maj mouvement
	player->setMovement(player->getMovement() - distance);
	cout << "Point(s) de mouvement restant : " << player->getMovement() << endl;
}
