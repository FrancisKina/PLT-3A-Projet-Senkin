#include "Engine.h"
#include <iostream>
#include <unistd.h>

using namespace state;
using namespace engine;
using namespace std;

Engine::Engine(){
}

void Engine::executeCommand(Command* command, sf::RenderWindow& window){
	command->execute(currentState);
	
	//Placement du curseur le joueur qui joue
	currentState.getCursor()->setCursorX(currentState.getPlaying()->getX());
	currentState.getCursor()->setCursorY(currentState.getPlaying()->getY());
	
	//Verification joueur mort
	std::vector<state::Player*> players = currentState.getPlayers();
	for(size_t i=0; i<players.size(); i++) {
		if(players[i]->getHp() <= 0){
			players[i]->setX(-1);
			players[i]->setY(-1);
			cout << endl << players[i]->getName() <<  " est mort." << endl;
			players.erase(players.begin()+i);
		}
	}
	
	currentState.notifyObservers(currentState, window);
}

state::State& Engine::getState(){
	state::State& refState=currentState;
	return refState;
}

void Engine::startGame (state::State& state){
		//Initialisation du curseur
	currentState.initCursor();

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
	/*
	bool new_case;
	int proba_new_case= 80;
	int chanceNewCase=rand()%100+1;
	if(proba_new_case>chanceNewCase){
		new_case=true;
	}else{
		new_case=false;
	}
	
	
	int cx,cy; //aléatoire equiprobable
	int nb_player = state.getPlayers().size();
	int nb_tour_max=6;
	int nb_tour; //aléatoire de 1 à bcp decroissant
	int status_id_test;
	FieldStatusId status_id; //aléatoire equiprobable
	Field* case_field;
	std::vector<std::pair<FieldStatusId,int>> case_field_status;
	FieldTypeId case_field_type;
	bool meteo=true;
	
	while(new_case){
		chanceNewCase=rand()%100+1;
		if(proba_new_case>chanceNewCase){
			new_case=true;
		}else{
			new_case=false;
		}
		//def cx et cy
		cy=rand()%field_grid.size();
		cx=rand()%field_grid[1].size();
		//def nb_tour
		int chanceTour =rand()%nb_tour_max*(nb_tour_max+1)+1;//de 1 a nbtourmax
		int nb_tour_test=nb_tour_max*(nb_tour_max+1)/2;
		for(int u=1;u<nb_tour_max;u++){
			if(chanceTour<nb_tour_test){
				nb_tour = u*nb_player;
				break;
			}else{
				nb_tour_test=nb_tour_test+nb_tour_max*(nb_tour_max+1)/2-u*(nb_tour_max-1);
				
			}
		}
		
		case_field = field_grid[cy][cx];
		case_field_status = case_field->getFieldStatus();
		case_field_type = case_field->getFieldType();
		//calcul des probas de chaque status meteo
		std::vector<Field*> case_field_prox ={field_grid[cy+1][cx],field_grid[cy-1][cx],field_grid[cy][cx+1],field_grid[cy][cx-1]};
		for(int p=0;p<4;p++){
			if((case_field_prox[p]->getFieldType()==WATER || case_field_prox[p]->getFieldType()==SWAMP || case_field_prox[p]->getFieldType()==MOUNTAIN ) && case_field_type!=MOUNTAIN){
				chanceRain=60;
				chanceMist=35;
			}else if(case_field_type==MOUNTAIN){
				chanceRain=25;
				chanceSnow=40;
				chanceMist=35;
			}else if(case_field_type==SAND){
				chanceRain=1;
				chanceSnow=1;
				chanceMist=5;
			}
		}
		
		//def status_id
		cout<<"Il ";
		status_id_test=rand()%100 + 1;
		if(status_id_test<chanceRain){
			status_id=RAIN;
			cout<<"pleut";
		}else if(status_id_test<chanceRain+chanceMist){
			status_id=MIST;
			cout<<"brume";
		}else if(status_id_test<chanceRain+chanceMist+chanceSnow){
			status_id=SNOW;
			cout<<"neige";
		}else{
			meteo=false;
			cout<<"ne se passe rien";
		}
		if(meteo){
			//verfication status case
			if(case_field_status[status_id].second>0){
				//intensifie l'état
				case_field->updateFieldStatus({status_id,nb_tour+case_field_status[status_id].second});
				cout<<" encore plus fort pendant "<<nb_tour+case_field_status[status_id].second<<" tour(s)";
			}else{
				//change l'état
				for(int m=1;m<10;m++){//m<10 => état météo
					if(m!=status_id){
						std::pair<FieldStatusId,int> newFieldStatus={static_cast<FieldStatusId>(m),0};
						case_field->updateFieldStatus(newFieldStatus);
					}
				}
				case_field->updateFieldStatus({status_id,nb_tour});
				cout<<" pendant "<<nb_tour<<" tour(s)";
			}
		}else{
			//change l'état
			for(int m=1;m<10;m++){
				std::pair<FieldStatusId,int> newFieldStatus={static_cast<FieldStatusId>(m),0};
				case_field->updateFieldStatus(newFieldStatus);
			}
		}
		cout<<" sur la case ("<<cx<<","<<cy<<")."<<endl;
	}
	*/
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
