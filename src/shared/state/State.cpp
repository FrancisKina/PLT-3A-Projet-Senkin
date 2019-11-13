#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string.h>
#include <cstdlib>
#include "State.h"

using namespace state;
using namespace std;
//std::vector<std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>>> grid;

std::vector<std::vector<Field*>>& State::getGrid(){
	std::vector<std::vector<Field*>>& refGrid = grid;
	return refGrid;
}

std::vector<Player*> State::getPlayers(){
	return players;
}

int State::getRound(){
	return round;
}

void State::setRound(int newRound){
	round = newRound;
}

void State::initCursor(){
	cursor = new Cursor();
	cursor->setCursorX(0);
	cursor->setCursorY(0);
}

Cursor* State::getCursor(){
	return cursor;
}

void State::initPlayers(int nbPlayers){
	for (int i=0;i<nbPlayers;i++){
		players.push_back( new Player());
		players[i]->setName("J" + std::to_string(i+1));
		//players[i]->setClassId(static_cast<ClassId>(rand()%4));
	}

}

bool State::initGrid(std::string map_txt){
	std::ifstream fichier(map_txt, ios::in);
	std::string strnombre, ligne;
	int nombre;
	std::vector<Field*> ligneField;
	
	// Lecture Fichier
    if (fichier){
    	while (getline(fichier, ligne)){
			//Nouvelle ligne
			for(size_t i = 0; i < ligne.size();i++){ 
				if(isdigit(ligne.at(i)) != 0){
					strnombre+=ligne.at(i);
				}
				else{
					stringstream ssnombre(strnombre);
					ssnombre >> nombre;
					ligneField.push_back(new Field());
					ligneField.back()->setFieldType(static_cast<FieldTypeId>(nombre));
					
					//SWAMP -> POISON
					if (ligneField.back()->getFieldType() == SWAMP){
						ligneField.back()->updateFieldStatus({POISON, 999});
					}
					//SAND -> BURNED
					else if (ligneField.back()->getFieldType() == SAND){
						ligneField.back()->updateFieldStatus({BURNING, 999});
					}
					
					//WALL -> BLOCKMOVE + BLOCKATTACK
					else if (ligneField.back()->getFieldType() == WALL){
						ligneField.back()->updateFieldStatus({BLOCKMOVE, 999});
						ligneField.back()->updateFieldStatus({BLOCKATTACK, 999});
					}
					
					//WATER -> BLOCKMOVE
					else if (ligneField.back()->getFieldType() == WALL){
						ligneField.back()->updateFieldStatus({BLOCKMOVE, 999});
					}
					
					i++;
					strnombre = "";
				} 
			} 
			//Fin de ligne
			grid.push_back(ligneField);
			ligneField = {};
			strnombre = "";
		}
		fichier.close();
		
		return 1;
    }
    else {return 0;}
}

Player* State::getPlaying(){
	return playing;
}

void State::setPlaying(Player* newPlaying){
	playing = newPlaying;
}

//Comparateur d'initiative des joueurs
bool cmpPlayers(Player* player1, Player* player2){
	if (player1->getInitiative() <= player2->getInitiative()){
		return false;
	}else{
		return true;
	}		
}

// Trier les personnages par initiative pour l'ordre d'action et donner le tour d'action au premier
void State::sortPlayers(){
	std::sort(players.begin(), players.end(), cmpPlayers);
	setPlaying(players[0]);
}

