#include "Engine.h"
#include <iostream>
#include <unistd.h>
#include "engine.h"

using namespace state;
using namespace engine;
using namespace std;

Engine::Engine(){

}

void Engine::executeCommand(Command* command, sf::RenderWindow& window){
	//cout<<"execute appel"<<endl;
	std::vector<int> inter;
	std::vector<std::vector<int>> inter_b;
	std::vector<std::vector<std::vector<int>>> inter_c;
	for(size_t i=0;i<currentState.getGrid().size();i++){
		inter_b={};
		for(size_t j=0;j<currentState.getGrid()[i].size();j++){
			inter={};
			for(size_t k=0;k<currentState.getGrid()[i][j]->getFieldStatus().size();k++){
				inter.push_back(currentState.getGrid()[i][j]->getFieldStatus()[k].second);
			}
			inter_b.push_back(inter);
		}
		inter_c.push_back(inter_b);
	}
	previousGridStatus.push_back(inter_c);
	
	
	std::vector<int> interX;
	std::vector<int> interY;
	std::vector<state::DirectionId> interDirection;
	std::vector<int> interHp;
	std::vector<int> interMovement;
	std::vector<int> interInitiative;
	std::vector<int> interSkillCount;
	std::vector<std::vector<std::pair<state::CharStatusId,int>>> interStatus;
	std::vector<bool> interIa;
	std::vector<state::Character*> interCharacter;
	std::vector<int> interCoolDown;
	std::vector<std::vector<int>> interSkillsCoolDown;
	std::vector<std::string> interName;
	Player* player;
	for(size_t i=0;i<currentState.getPlayers().size();i++){
		player = currentState.getPlayers()[i];
		interX.push_back(player->getX());
		interY.push_back(player->getY());
		interDirection.push_back(player->getDirection());
		interHp.push_back(player->getHp());
		interMovement.push_back(player->getMovement());
		interInitiative.push_back(player->getInitiative());
		interSkillCount.push_back(player->getSkillCount());
		interStatus.push_back(player->getStatus());
		interIa.push_back(player->getIa());
		interCharacter.push_back(player->getCharacter());
		interCoolDown={};
		for(size_t s=0; s<player->getSkills().size();s++){
			Skill* skill =player->getSkills()[s];
			interCoolDown.push_back(skill->getCooldown());
		}
		interSkillsCoolDown.push_back(interCoolDown);
		interName.push_back(player->getName());
	}
	previousPlayersStatsX.push_back(interX);
	previousPlayersStatsY.push_back(interY);
	previousPlayersStatsDirection.push_back(interDirection);
	previousPlayersStatsHp.push_back(interHp);
	previousPlayersStatsMovement.push_back(interMovement);
	previousPlayersStatsInitiative.push_back(interInitiative);
	previousPlayersStatsSkillCount.push_back(interSkillCount);
	previousPlayersStatsStatus.push_back(interStatus);
	previousPlayersStatsIa.push_back(interIa);
	previousPlayersStatsCharacter.push_back(interCharacter);
	previousPlayersStatsSkillsCoolDown.push_back(interSkillsCoolDown);
	previousPlayersStatsName.push_back(interName);
	
	previousStates.push_back(currentState);
	currentState = getNextState(currentState,command);
	//Placement du curseur le joueur qui joue
	currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
	currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
	
	currentState.notifyObservers(currentState, window);
}

void Engine::simulateCommand(Command* command){
	//cout<<"simulate appel"<<endl;
	std::vector<int> inter;
	std::vector<std::vector<int>> inter_b;
	std::vector<std::vector<std::vector<int>>> inter_c;
	for(size_t i=0;i<currentState.getGrid().size();i++){
		inter_b={};
		for(size_t j=0;j<currentState.getGrid()[i].size();j++){
			inter={};
			for(size_t k=0;k<currentState.getGrid()[i][j]->getFieldStatus().size();k++){
				inter.push_back(currentState.getGrid()[i][j]->getFieldStatus()[k].second);
			}
			inter_b.push_back(inter);
		}
		inter_c.push_back(inter_b);
	}
	previousGridStatus.push_back(inter_c);
	
	
	std::vector<int> interX;
	std::vector<int> interY;
	std::vector<state::DirectionId> interDirection;
	std::vector<int> interHp;
	std::vector<int> interMovement;
	std::vector<int> interInitiative;
	std::vector<int> interSkillCount;
	std::vector<std::vector<std::pair<state::CharStatusId,int>>> interStatus;
	std::vector<bool> interIa;
	std::vector<state::Character*> interCharacter;
	std::vector<int> interCoolDown;
	std::vector<std::vector<int>> interSkillsCoolDown;
	std::vector<std::string> interName;
	Player* player;
	for(size_t i=0;i<currentState.getPlayers().size();i++){
		player = currentState.getPlayers()[i];
		interX.push_back(player->getX());
		interY.push_back(player->getY());
		interDirection.push_back(player->getDirection());
		interHp.push_back(player->getHp());
		interMovement.push_back(player->getMovement());
		interInitiative.push_back(player->getInitiative());
		interSkillCount.push_back(player->getSkillCount());
		interStatus.push_back(player->getStatus());
		interIa.push_back(player->getIa());
		interCharacter.push_back(player->getCharacter());
		interCoolDown={};
		for(size_t s=0; s<player->getSkills().size();s++){
			Skill* skill =player->getSkills()[s];
			interCoolDown.push_back(skill->getCooldown());
		}
		interSkillsCoolDown.push_back(interCoolDown);
		interName.push_back(player->getName());
	}
	previousPlayersStatsX.push_back(interX);
	previousPlayersStatsY.push_back(interY);
	previousPlayersStatsDirection.push_back(interDirection);
	previousPlayersStatsHp.push_back(interHp);
	previousPlayersStatsMovement.push_back(interMovement);
	previousPlayersStatsInitiative.push_back(interInitiative);
	previousPlayersStatsSkillCount.push_back(interSkillCount);
	previousPlayersStatsStatus.push_back(interStatus);
	previousPlayersStatsIa.push_back(interIa);
	previousPlayersStatsCharacter.push_back(interCharacter);
	previousPlayersStatsSkillsCoolDown.push_back(interSkillsCoolDown);
	previousPlayersStatsName.push_back(interName);
	
	previousStates.push_back(currentState);
	currentState = getNextState(currentState,command);
	//Placement du curseur le joueur qui joue
	currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
	currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
	
	//currentState.notifyObservers(currentState, window);
}

state::State& Engine::getState(){
	return currentState;
}

sf::RenderWindow& Engine::getWindow(){
	//currentWindow = new sf::RenderWindow(sf::VideoMode(tilesize * state.getGrid()[0].size(), tilesize * state.getGrid().size()), "Test");
	return currentWindow;
}

void Engine::startGame (state::State& state){
		//Initialisation du curseur
	currentState.initCursor();
	currentState.setCommandMode(FIELD);
	previousStates={};

	cout<<"Lancement du jeu"<<endl;
	currentState.setRound(1);
	currentState.sortPlayers();// Trier les personnages par initiative pour l'ordre d'action et donner le tour d'action au premier joueur
	cout << "Ordre de jeu : ";
	for (Player* p : currentState.getPlayers()){
		cout << p->getName() << " ";
	}
	cout << endl;
	
	//Placement du curseur le joueur qui joue
	currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
	currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
	
	//------------------------changment aléatoire d'état du terrain-----------------------

	cout << "Evenements aléatoires" << endl;
	int nzone = rand()%3+1;
	
	for(int i=0; i<nzone; i++){
		unsigned int x = rand()%state.getGrid()[0].size();
		unsigned int y = rand()%state.getGrid().size();
		unsigned int ntour = state.getPlayers().size() * (rand()%2+1);
		
		int chanceRain, chanceMist, chanceSnow, chanceBurn;
		FieldTypeId type = state.getGrid()[y][x]->getFieldType();
		if(type == MOUNTAIN) chanceRain=25, chanceMist=35, chanceSnow=40, chanceBurn = 0 ;
		else if (type == SAND) chanceRain=10, chanceMist=5, chanceSnow=5, chanceBurn = 80;
		else chanceRain=60, chanceMist=30, chanceSnow=10;
		
		int reffect = rand()%100+1;
		FieldStatusId effect;
		if(reffect<chanceRain) effect = RAIN;
		else if(reffect<chanceRain+chanceMist) effect = MIST;
		else if(reffect<chanceRain+chanceMist+chanceSnow) effect = SNOW;
		else if(reffect<chanceRain+chanceMist+chanceSnow+chanceBurn) effect = BURNING;
		
		int proba = 100;
		while(proba>0){
			if (rand()%100<proba){
				state.getGrid()[y][x]->updateFieldStatus(std::make_pair(effect, ntour));
				x += rand()%3-1;
				y += rand()%3-1;
				if (x<0) x=0;
				if (y<0) y=0;	
				if (x>=state.getGrid()[0].size()) x=state.getGrid()[0].size()-1;
				if (y>=state.getGrid().size()) y=state.getGrid().size()-1;	
			}
			proba-=5;
		}
	}
	
	cout << endl << "[ Début de round "<< currentState.getRound() <<"]" << endl;
	cout << endl << "---------- Tour du joueur " << currentState.getPlaying()->getName() << " ----------" << endl;
}

void Engine::keyCommand (sf::Event event, sf::RenderWindow& window){
	
	if (currentState.getPlaying()->getIa() == true) return;

	Cursor* cursor = currentState.getCursor();
	Cursor* cursorinfo = currentState.getCursorInfo();
	
	switch (event.key.code){
		//FLECHE GAUCHE
		case sf::Keyboard::Left:
			if (currentState.getCommandMode() == FIELD || currentState.getCommandMode() == SKILLTARGET || currentState.getCommandMode() == MOVEMENT) {
				if (cursor->getCursorX() > 0) cursor->setCursorX(cursor->getCursorX() - 1);
				else cursor->setCursorX(0);	
			}
			break;
		//FLECHE HAUT
		case sf::Keyboard::Up:
			if (currentState.getCommandMode() == FIELD || currentState.getCommandMode() == SKILLTARGET || currentState.getCommandMode() == MOVEMENT) {
				if (cursor->getCursorY() > 0) cursor->setCursorY(cursor->getCursorY() - 1);
				else cursor->setCursorY(0);	
			}
			else if (currentState.getCommandMode() == COMMAND) {
				if (cursorinfo->getCursorY() > 21) cursorinfo->setCursorY(cursorinfo->getCursorY() - 1);
				else cursorinfo->setCursorY(21);	
			}
			else if (currentState.getCommandMode() == SKILL) {
				if (cursorinfo->getCursorY() > 6) cursorinfo->setCursorY(cursorinfo->getCursorY() - 1);
				else cursorinfo->setCursorY(6);	
			}
			break;
		//FLECHE DROITE
		case sf::Keyboard::Right:
			if (currentState.getCommandMode() == FIELD || currentState.getCommandMode() == SKILLTARGET || currentState.getCommandMode() == MOVEMENT) {
				if (cursor->getCursorX() < (int)currentState.getGrid()[0].size()-1) cursor->setCursorX(cursor->getCursorX() + 1);
				else cursor->setCursorX(currentState.getGrid()[0].size()-1);	
			}
			break;
		//FLECHE BAS	
		case sf::Keyboard::Down:
			if (currentState.getCommandMode() == FIELD || currentState.getCommandMode() == SKILLTARGET || currentState.getCommandMode() == MOVEMENT) {
				if (cursor->getCursorY() < (int)currentState.getGrid().size()-1) cursor->setCursorY(cursor->getCursorY() + 1);
				else cursor->setCursorY(currentState.getGrid().size()-1);
			}
			else if (currentState.getCommandMode() == COMMAND) {
				if (cursorinfo->getCursorY() < 23) cursorinfo->setCursorY(cursorinfo->getCursorY() + 1);
				else cursorinfo->setCursorY(23);	
			}
			else if (currentState.getCommandMode() == SKILL) {
				if (cursorinfo->getCursorY() < 7) cursorinfo->setCursorY(cursorinfo->getCursorY() + 1);
				else cursorinfo->setCursorY(7);	
			}
			break;
			
		//ESPACE OU ENTREE
		case sf::Keyboard::Space:	
		case sf::Keyboard::Return:
			if (currentState.getCommandMode() == FIELD) {
				currentState.setCommandMode(COMMAND);
				cursorinfo->setCursorY(21);
			}
			else if (currentState.getCommandMode() == COMMAND) {
				if (cursorinfo->getCursorY() == 21){
					cursor->setCursorX(currentState.getPlaying()->getX());
					cursor->setCursorY(currentState.getPlaying()->getY());
					currentState.setCommandMode(MOVEMENT);
				}
				else if (cursorinfo->getCursorY() == 22){ 
					currentState.setCommandMode(SKILL);
					cursorinfo->setCursorY(6);
				}
				else if (cursorinfo->getCursorY() == 23) {
					currentState.setCommandMode(FIELD);
					executeCommand(new EndActions(), currentWindow);
				}
			}
			else if (currentState.getCommandMode() == SKILL) {
				cursor->setCursorX(currentState.getPlaying()->getX());
				cursor->setCursorY(currentState.getPlaying()->getY());
				currentState.setCommandMode(SKILLTARGET);
			}
			else if (currentState.getCommandMode() == SKILLTARGET) {
				executeCommand(new Attack(std::make_pair (cursor->getCursorX(),cursor->getCursorY()) , cursorinfo->getCursorY()-6), currentWindow);
				currentState.setCommandMode(SKILL);
			}
			else if (currentState.getCommandMode() == MOVEMENT) {
				executeCommand(new Move(std::make_pair (cursor->getCursorX(),cursor->getCursorY())), currentWindow);
				currentState.setCommandMode(COMMAND);
			}
			break;
	
		case sf::Keyboard::Escape:
			if (currentState.getCommandMode() == COMMAND) {
				currentState.setCommandMode(FIELD);
			}
			else if (currentState.getCommandMode() == SKILL || currentState.getCommandMode() == MOVEMENT) {
				currentState.setCommandMode(COMMAND);
				cursorinfo->setCursorY(21);
			}
			else if (currentState.getCommandMode() == SKILLTARGET) {
				currentState.setCommandMode(SKILL);
				cursorinfo->setCursorY(6);
			}
			break;
			
		default:
			break;
	}
	
	currentState.notifyObservers(currentState, window);

}

state::State Engine::getNextState (state::State state, Command* command){
	state::State& nextState=state;
	command->execute(nextState);
		//Verification joueur mort
	std::vector<state::Player*> players = nextState.getPlayers();
	for(size_t i=0; i<players.size(); i++) {
		if(players[i]->getHp() <= 0){
			players[i]->setX(-1);
			players[i]->setY(-1);
			//cout << endl << players[i]->getName() <<  " est mort." << endl;
			players.erase(players.begin()+i);
		}
	}
	return nextState;
}
    
state::State Engine::getPreviousState (int previous){
	state::State previousState = previousStates[previousStates.size()-previous-1];
	/*for(size_t i=0;i<previousState.getGrid().size();i++){
		for(size_t j=0;j<previousState.getGrid()[j].size();j++){
			for(size_t k=0;k<currentState.getGrid()[i][j]->getFieldStatus().size();k++){
				previousState.getGrid()[i][j]->updateFieldStatus({static_cast<state::FieldStatusId>(k),previousGridStatus[previousGridStatus.end()-previous-1][i][j][k]});
			}
		}
	}*/
	return previousState;
}

bool Engine::rollBack(){
	bool first;
	/*
	for(size_t i=0;i<previousPlayersStatsSkillsCoolDown[previousPlayersStatsSkillsCoolDown.size()-1].size();i++){ 
		for(size_t j=0;j<previousPlayersStatsSkillsCoolDown[previousPlayersStatsSkillsCoolDown.size()-1][i].size();j++){
			cout << previousPlayersStatsSkillsCoolDown[previousPlayersStatsSkillsCoolDown.size()-1][i][j] << " ";
		}
		cout << endl;
	}
	*/
	if(previousStates.size()>0){
		//cout<<"Etat restant : "<<previousStates.size()<<endl;
		currentState=previousStates[previousStates.size()-1];
		previousStates.pop_back();
		state::Player* player;
		
		for(size_t i=0;i<currentState.getPlayers().size();i++){
			player = currentState.getPlayers()[i];
			/*
			cout<<"Joueur "<<previousPlayersStatsName[previousPlayersStatsName.size()-1][i]<<":";
			cout<<" X="<<previousPlayersStatsX[previousPlayersStatsX.size()-1][i];
			cout<<" Y="<<previousPlayersStatsY[previousPlayersStatsY.size()-1][i];
			cout<<" HP="<<previousPlayersStatsHp[previousPlayersStatsHp.size()-1][i];
			cout<<" PA="<<previousPlayersStatsSkillCount[previousPlayersStatsSkillCount.size()-1][i];
			cout<<" PM="<<previousPlayersStatsMovement[previousPlayersStatsMovement.size()-1][i]<<endl;
			*/
			player->setX(previousPlayersStatsX[previousPlayersStatsX.size()-1][i]);
			player->setY(previousPlayersStatsY[previousPlayersStatsY.size()-1][i]);
			player->setDirection(previousPlayersStatsDirection[previousPlayersStatsDirection.size()-1][i]);
			player->setHp(previousPlayersStatsHp[previousPlayersStatsHp.size()-1][i]);
			player->setMovement(previousPlayersStatsMovement[previousPlayersStatsMovement.size()-1][i]);
			player->setInitiative(previousPlayersStatsInitiative[previousPlayersStatsInitiative.size()-1][i]);
			player->setSkillCount(previousPlayersStatsSkillCount[previousPlayersStatsSkillCount.size()-1][i]);
			player->setStatus(previousPlayersStatsStatus[previousPlayersStatsStatus.size()-1][i]);
			player->setIa(previousPlayersStatsIa[previousPlayersStatsIa.size()-1][i]);
			//player->setCharacter(previousPlayersStatsCharacter[previousPlayersStatsCharacter.size()-1][i]);
			/*
			cout<<"Test j "<<player->getName()<<":";
			cout<<" X="<<player->getX();
			cout<<" Y="<<player->getY();
			cout<<" HP="<<player->getHp();
			cout<<" PA="<<player->getSkillCount();
			cout<<" PM="<<player->getMovement()<<endl;
			*/
			for(size_t s=0;s<player->getSkills().size();s++){
				Skill* skill =player->getSkills()[s];
				skill->setCooldown(previousPlayersStatsSkillsCoolDown[previousPlayersStatsSkillsCoolDown.size()-1][i][s]);
			}
			player->setName(previousPlayersStatsName[previousPlayersStatsName.size()-1][i]);
		}
		for(size_t i=0;i<currentState.getGrid().size();i++){
			for(size_t j=0;j<currentState.getGrid()[i].size();j++){
				for(size_t k=0;k<currentState.getGrid()[i][j]->getFieldStatus().size();k++){
					currentState.getGrid()[i][j]->updateFieldStatus(std::make_pair(static_cast<state::FieldStatusId>(k+1),previousGridStatus[previousGridStatus.size()-1][i][j][k]));
				}
			}
		}
		previousGridStatus.pop_back();
		previousPlayersStatsX.pop_back();
		previousPlayersStatsY.pop_back();
		previousPlayersStatsDirection.pop_back();
		previousPlayersStatsHp.pop_back();
		previousPlayersStatsMovement.pop_back();
		previousPlayersStatsInitiative.pop_back();
		previousPlayersStatsSkillCount.pop_back();
		previousPlayersStatsStatus.pop_back();
		previousPlayersStatsIa.pop_back();
		previousPlayersStatsCharacter.pop_back();
		previousPlayersStatsSkillsCoolDown.pop_back();
		previousPlayersStatsName.pop_back();
		//Placement du curseur le joueur qui joue
		currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
		currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
		currentState.notifyObservers(currentState, currentWindow);
		first= false;
	}else{
		cout<<"Etat initial atteint"<<endl;
		first = true;
	}
	return first;
}
