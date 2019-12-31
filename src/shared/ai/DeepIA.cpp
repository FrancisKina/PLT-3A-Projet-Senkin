#include "ai.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;




int DeepIA::run (engine::Engine& engine){
	cout << "DeepIA run" << endl;
	if(!engine.getState().getPlaying()->getIa()) return 0;
	
	actionsList = possibleActions(engine); //Liste des actions possibles
	//cout << "actionsList " << actionsList.size() << endl;
	
	actionEvaluation(engine);
	
	int minmaxout = minmax();
	Command* optimalaction = actionsList[minmaxout];
	
	cout << "===================== MOUVEMENT OPTIMAL  " << optimalaction->getCommandID() << endl;
	//engine.executeCommand(optimalaction);
	engine.setNextCommand(optimalaction);
	
	while (actionsList.size()>0){
		actionsList.pop_back();
	}
	
	while (actionsScoreList.size()>0){
		actionsScoreList.pop_back();
	}
	
	return 0;
}

std::vector<engine::Command*> DeepIA::possibleActions (engine::Engine& engine){
	
	State& state = engine.getState();
	std::vector<engine::Command*> actionslist;
	
	int x,y;
	Player* playing = state.getPlaying();
	
	bool stop = false;
	
	//ATTAQUES
	if (playing->getSkillCount() > 0){ // Si le joueur a encore des PA
		for (size_t s=0; s<playing->getSkills().size(); s++){ //Pour chaque skill
			Skill* skill = playing->getSkills()[s];
			//cout << skill->getName() << " " << skill->getCooldown() << endl;
			if (skill->getCooldown() <= 0){ //Si le skill n'est pas en rechargement
				for (int range = skill->getRange().first; range<=skill->getRange().second; range++){ //Pour chaque portee
					for(int d=0; d<4; d++){ //Pour chaque direction
						if (d==0){ 
							x = playing->getX()+range;
							y = playing->getY();
						}
						else if (d==1){ 
							x = playing->getX()-range;
							y = playing->getY();
						}
						else if (d==2){ 
							x = playing->getX();
							y = playing->getY()+range;
						}
						else if (d==3){ 
							x = playing->getX();
							y = playing->getY()-range;
						}
						for(Player* player : state.getPlayers()){
							if(x == player->getX() && y == player->getY() && state.getGrid()[y][x]->getFieldStatus()[10].second==0){
								actionslist.push_back(new Attack(std::make_pair (x,y), s));
								stop = true;
							}
						}
					}
				}
			}
		}
	}
	
	if(stop){ 
		return actionslist;
	}
	
	//DEPLACEMENTS
	if (playing->getMovement() > 0){ // Si le joueur a encore des PM
		for(int d=0; d<4; d++){
			if (d==0){ 
				x = playing->getX()+1;
				y = playing->getY();
			}
			else if (d==1){ 
				x = playing->getX()-1;
				y = playing->getY();
			}
			else if (d==2){ 
				x = playing->getX();
				y = playing->getY()+1;
			}
			else if (d==3){ 
				x = playing->getX();
				y = playing->getY()-1;
			}
			
			//Calcul de cout en PM
			bool addMove = true;
			FieldTypeId destinationtype = state.getGrid()[y][x]->getFieldType();
			MovementId movementtype = playing->getCharacter()->getMovementType();
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
			
			for(Player* player : state.getPlayers()){
				if (distance > playing->getMovement() || (player != playing && x == player->getX() && y == player->getY()) || state.getGrid()[y][x]->getFieldStatus()[11].second>0){ //Si BLOCKMOVE, autre joueur, ou pas assez de PM
					addMove = false;
				}
			}
			if (addMove){ 
				actionslist.push_back(new Move({x,y}));
				stop = true;
			}
		}
	}
	
	if(stop){ 
		actionslist.push_back(new EndActions());
		return actionslist;
	}
	
	//FIN D'ACTIONS
	actionslist.push_back(new EndActions());
	return actionslist;
}

/* Evaluation du score final de chaque action en fonction des coups suivants de l'adversaire*/ 
void DeepIA::actionEvaluation (engine::Engine& engine){
	State& state = engine.getState();
	Player* playing = state.getPlaying();
	
	std::vector<int> backHP;
	std::vector<int> backX;
	std::vector<int> backY;
	std::vector<int> interHP;
	std::vector<int> interX;
	std::vector<int> interY;
	
	//Parcourir les actions possibles
	for(Command* action : actionsList){
		//Sauvegarde 1
		for(Player* player: state.getPlayers()){
			 backHP.push_back(player->getHp());
			 backX.push_back(player->getX());
			 backY.push_back(player->getY());
		}
		
		std::vector<int> actionScore;
		
		//Simulation de l'action du joueur
		simulateCommand(state, action);
		
		//Simulation des actions de chaque adversaire
		for(size_t p=0; p<state.getPlayers().size(); p++){
			Player* player = state.getPlayers()[p];
			if (player != playing){
				state.setPlaying(player);
				//Toutes les actions possibles de l'adversaire
				std::vector<engine::Command*> playerActions = possibleActions(engine); 
				for(Command* playeraction : playerActions){
					//Sauvegarde 2
					for(Player* player: state.getPlayers()){
						 interHP.push_back(player->getHp());
						 interX.push_back(player->getX());
						 interY.push_back(player->getY());
					}
					
					//Simulation d'une action de l'adversaire
					state.setPlaying(player);
					simulateCommand(state, playeraction);
					actionScore.push_back(stateScore(state));
					
					//Chargement 2
					for(Player* player: state.getPlayers()){
						player->setHp(interHP.front());
						interHP.erase(interHP.begin());
						player->setX(interX.front());
						interX.erase(interX.begin());
						player->setY(interY.front());
						interY.erase(interY.begin());
					}
				}
			}
		}
		//Chargement 1
		state.setPlaying(playing);
		for(Player* player: state.getPlayers()){
			player->setHp(backHP.front());
			backHP.erase(backHP.begin());
			player->setX(backX.front());
			backX.erase(backX.begin());
			player->setY(backY.front());
			backY.erase(backY.begin());
		}
		actionsScoreList.push_back(actionScore);
	}
}

int DeepIA::stateScore (state::State& state){
	Player* playing = state.getPlaying();
	int score = 0;
	
	for(Player* player : state.getPlayers()){
		if (player == playing) score += 100 * player->getHp();
		else score -= 100 * player->getHp();
		
		//Difference de points de vie par rapport a la distance
		if (playing->getHp() >= player->getHp()){
			score += (playing->getHp() - player->getHp()) * (-abs(playing->getX() - player->getX()) - abs(playing->getY() - player->getY()) + 100);
		}
		else{
			score += (playing->getHp() - player->getHp()) * (abs(playing->getX() - player->getX()) + abs(playing->getY() - player->getY()) - 100);
		}
	}
	cout << "SCORE" << score << endl;
	return score;
}

int DeepIA::minmax(){
	//MIN
	std::vector<int> minlist;
	//cout << "actionsScoreList.size()" << actionsScoreList.size() << endl;
	for(size_t i=0; i<actionsScoreList.size(); i++){
		int min = 9999;
		for(size_t j=0; j<actionsScoreList[i].size(); j++){
			if(actionsScoreList[i][j]<min){
				min = actionsScoreList[i][j];
			}
		}
		minlist.push_back(min);
	}
	
	//cout << "minlist.size()" << minlist.size() << endl;
	//MAX
	int max = -9999;
	int optimal = 0;
	//cout << "minlist.size()" << minlist.size() << endl;
	for(size_t i=0; i<minlist.size(); i++){
		 if(minlist[i] > max){
			 max = minlist[i];
			 optimal = i;
		 }
	}
	return optimal;
}

void DeepIA::simulateCommand(state::State& state, engine::Command* action){
	Player* playing = state.getPlaying();
	std::pair<int,int> destination;
	std::pair<int,int> posTarget;
	int nSkill;
	state::Skill* skill;
	
	switch(action->getCommandID()){
		case(MOVE):
			destination = action->getPos();
			cout << playing->getName() << " MOVE " << destination.first << "," << destination.second << endl;
			playing->setX(destination.first);
			playing->setY(destination.second);
			break;
		case(ATTACK):
			nSkill = action->getN();
			posTarget = action->getPos();
			skill = playing->getSkills()[nSkill];
			cout << playing->getName() <<" ATTACK " << skill->getName() << endl;
			//Si l'attaque fait des dommages
			if (skill->getDamage()>0){ 
				for(Player* player : state.getPlayers()){
					if(posTarget.first == player->getX() && posTarget.second == player->getY()){
						player->setHp(player->getHp() - skill->getDamage());
					}
				}
			}
			//Si attaque de soin
			else if (get<0>(skill->getEffect()[0])==HEAL_MEDIUM || get<0>(skill->getEffect()[0])==HEAL_LOW || get<0>(skill->getEffect()[0])==HEAL_HIGH){
				playing->setHp(playing->getHp()+10);
			}
			break;
		default:
			cout << playing->getName() <<" ENDACTIONS " << endl;
			break;
	}
}
