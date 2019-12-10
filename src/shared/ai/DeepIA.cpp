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


int DeepIA::run (engine::Engine& engine, sf::RenderWindow& window){
	
	if(!engine.getState().getPlaying()->getIa()) return 0;
	
	actionsList = possibleActions(engine); //Liste des actions possibles
	//cout << "actionsList " << actionsList.size() << endl;
	
	actionEvaluation(engine, window);
	
	Command* optimalaction = actionsList[minmax()];
	cout << "===================== MOUVEMENT OPTIMAL  " <<optimalaction->getCommandID() << endl;
	engine.executeCommand(optimalaction, window);
	
	for(size_t i=0; i<actionsList.size(); i++){
		actionsList.pop_back();
	}
	
	for(size_t i=0; i<actionsScoreList.size(); i++){
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
			cout << skill->getName() << " " << skill->getCooldown() << endl;
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
							if(player != playing && x == player->getX() && y == player->getY() && state.getGrid()[y][x]->getFieldStatus()[10].second==0){
								actionslist.push_back(new Attack(std::make_pair (x,y), s));
								stop = true;
							}
							if(skill->getEffect().size()>0 && player==playing && (get<0>(skill->getEffect()[0])==HEAL_MEDIUM || get<0>(skill->getEffect()[0])==HEAL_LOW || get<0>(skill->getEffect()[0])==HEAL_HIGH) && playing->getHp()<playing->getCharacter()->getHpBase()){
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
		actionslist.push_back(new EndActions());
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
void DeepIA::actionEvaluation (engine::Engine& engine, sf::RenderWindow& window){
	State& state = engine.getState();
	Player* playing = state.getPlaying();
	
	//Simulation avec des attaques qui touchent obligatoirement : precision a 100
	std::vector<std::vector<int>> backPrecisionList;
	for(Player* player : state.getPlayers()){
		std::vector<int> backPrecision;
		for (Skill* skill : player->getSkills()){
			backPrecision.push_back(skill->getPrecision());
			skill->setPrecision(100);
		}
		backPrecisionList.push_back(backPrecision);
	}
	std::vector<engine::Command*> playingActions = possibleActions(engine);
	
	engine.simulateCommand(new engine::EndActions());
	Player* player = state.getPlaying();
	engine.rollBack();
	
	//Parcourir les actions possibles
	for(Command* action : actionsList){
		std::vector<int> actionScore;
		
		//Simulation de l'action du joueur
		engine.simulateCommand(action);

		//Simulation des actions des adversaires
		/*
		for(size_t p=0; p<state.getPlayers().size(); p++){
			Player* player = state.getPlayers()[p];
			if (player != playing){
				state.setPlaying(player);
				std::vector<engine::Command*> playerActions = possibleActions(engine); //Toutes les actions possibles de l'adversaire
				//cout << "playerActions " << playerActions.size() << endl;;
				for(Command* playeraction : playerActions){
					
					//Simulation d'une action
					state.setPlaying(player);
					engine.simulateCommand(playeraction);
					actionScore.push_back(stateScore(state));
					
					//Rollback adversaire
					engine.rollBack();
				}
			}
		}
		*/
		
		if (player != playing){
			state.setPlaying(player);
			std::vector<engine::Command*> playerActions = possibleActions(engine); //Toutes les actions possibles de l'adversaire
			//cout << "playerActions " << playerActions.size() << endl;
			for(Command* playeraction : playerActions){
				
				//Simulation d'une action
				state.setPlaying(player);
				engine.simulateCommand(playeraction);
				actionScore.push_back(stateScore(state));
				
				//Rollback adversaire
				engine.rollBack();
			}
		}
		
		//Rollback joueur
		engine.rollBack();
		actionsScoreList.push_back(actionScore);
	}

	//Remise aux valeurs de base de la precision des skills
	for (size_t j=0; j<state.getPlayers().size(); j++){
		for (size_t i=0; i<state.getPlayers()[j]->getSkills().size(); i++){
			Skill* skill = state.getPlayers()[j]->getSkills()[i];
			skill->setPrecision(backPrecisionList[j][i]);
		}
	}
}

int DeepIA::stateScore (state::State& state){
	Player* playing = state.getPlaying();
	int score = 0;
	
	for(Player* player : state.getPlayers()){
		if (player == playing) score += 100 * player->getHp();
		else score -= 1000 * player->getHp();
		
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
