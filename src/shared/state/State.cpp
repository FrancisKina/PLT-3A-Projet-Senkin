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
		players[i]->setPlayerId(i+1);
		players[i]->setX(i);
		players[i]->setY(i);
		players[i]->setDirection(SOUTH);
		//players[i]->setClassId(static_cast<ClassId>(rand()%4));
	}

}

bool State::initGrid(std::string map_txt){
	std::ifstream fichier(map_txt, ios::in);
	std::string strnombre, ligne;
	int nombre;
	std::vector<Field*> ligneField;
	//std::pair<FieldTypeId, std::vector<std::pair<FieldStatusId,int>>> stat;
	
	// Lecture Fichier
    if (fichier){
    	while (getline(fichier, ligne)){
			for(size_t i = 0; i < ligne.size();i++){ //Lecture d'une ligne
				if(isdigit(ligne.at(i)) != 0){
					strnombre+=ligne.at(i);
				}
				else{
					stringstream ssnombre(strnombre);
					ssnombre >> nombre;
					ligneField.push_back(new Field());
					ligneField.back()->setFieldType(static_cast<FieldTypeId>(nombre));
					cout<<ligneField.back()->getFieldType();
					i++;
					strnombre = "";
				} 
			} //Fin de ligne
			grid.push_back(ligneField);
			ligneField = {};
			strnombre = "";
			cout<<endl;
		}
		for(size_t i=0; i<grid.size(); i++){
			cout<<endl;
			for(size_t j=0; j<grid[0].size(); j++){
				cout<<grid[i][j]->getFieldType();
			}
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
