#include "EndActions.h"
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

EndActions::EndActions(){
	id = ENDACTIONS;
}

void EndActions::execute(state::State& state){
	player = state.getPlaying();
	int chanceEffet;
	for(size_t i=0; i<state.getPlayers().size();i++){ //Parcourir la liste des joueurs qui est triée par ordre d'action
		
		if(player==state.getPlayers()[i]){ //Si le joueur est bien dans la liste des joueurs
			
			cout << endl << "| Fin d'action |" << endl;
			
			//Dégat de status et terrain
			std::vector<std::pair<CharStatusId, int>> status = player->getStatus();
			if (status[1].second>0){
				cout << "Degats de brulure" << endl;
				player->setHp(player->getHp()*0.95f);
			}
			if (status[0].second>0){
				cout << "Degats de poison" << endl;
				player->setHp(player->getHp()*0.95f);
				if(player->getInitiative()>0){
					player->setInitiative(player->getInitiative()-1);
				cout << "Baisse d'initiative du poison" << endl;
				}
			}
			
			//Décrémentation de la durée des statuts joueur
			for(size_t j=0; j<status.size(); j++){
				if(status[j].second>0){
					player->updateStatus({status[j].first,status[j].second-1});
					cout << "Decrementation de la duree du status " << j << endl;
				}
			}
			
			//Décrémentation de la durée des statuts terrain
			std::vector<std::vector<Field*>>& field_grid = state.getGrid();
			std::vector<std::pair<FieldStatusId,int>> field_status;
			Field* field_changed;
			for(size_t fy = 0; fy<field_grid.size(); fy++){
				for(size_t fx= 0; fx<field_grid[fy].size(); fx++){
					field_status=field_grid[fy][fx]->getFieldStatus();
					field_changed =field_grid[fy][fx];
					for(size_t s=0;s<field_status.size(); s++){
						if(field_status[s].second>0){
							field_changed->updateFieldStatus({field_status[s].first,field_status[s].second-1});
							field_status=field_grid[fy][fx]->getFieldStatus();//redefinir pour actualiser
							if(field_status[0].second>0 && field_status[12].second>0){//s'il pleut et brule
								chanceEffet=rand()%100 + 1;
								if(chanceEffet<=50){
									cout << "La pluie affaiblit l'incendie" << endl;
									field_changed->updateFieldStatus({field_status[12].first,status[12].second-1});
								}
							}
							if(field_grid[fy][fx]->getFieldStatus()[s].second==0){
								//cout<<"Fin du status n°"<<s<<endl;
							}
						}
					}
				}
			}
			//Réinitialisation des PM et PA (et de l'initiative)
			player->setMovement(player->getCharacter()->getMovementBase());
			cout << "Reinitialisation des PM du joueur" << endl;
			player->setSkillCount(player->getCharacter()->getSkillCountBase());
			cout << "Reinitialisation des PA du joueur" << endl;
			player->setInitiative(player->getCharacter()->getInitiativeBase());
			
			//Application des effets du terrain
			int X = player->getX();
			int Y = player->getY();
			
			std::vector<std::pair<FieldStatusId,int>> current_field_status;
			current_field_status = field_grid[Y][X]->getFieldStatus();
			FieldTypeId current_field_type = field_grid[Y][X]->getFieldType();
			//neige
			if(current_field_status[1].second>0){
				chanceEffet=rand()%100 + 1;
				if(chanceEffet>=50){
					cout << "Effet de diminution des PM raté "<< endl;
				}else{
					cout << "Diminution des PM" << endl;
					player->setMovement(player->getMovement()-1);
				}
			}
			//incendie
			if(field_status[12].second>0){
				chanceEffet=rand()%100 + 1;
				if(chanceEffet>=50){
					cout << "Effet BURN rate "<< endl;
				}else{
					cout << "Joueur brule"<< endl;
					bool test_fc=true;
					for(size_t s=0; s<status.size();s++){
						if ((status[s].first==BURNED) && (status[s].second>2)){
							test_fc=false;
							break;
						}
					}
					if(test_fc){
						std::pair<CharStatusId, int> newStatus = {BURNED,2};
						player->updateStatus(newStatus);
						cout<<"Le joueur "<<player->getName()<<" sera brule pendant "<<2<<" tours"<<endl;
					}
				}
			}
			
			//marécage
			if(current_field_type==SWAMP){
				chanceEffet=rand()%100 + 1;
				if(chanceEffet>=50 || player->getMovement()<2){
					cout << "Effet reduction de PM rate "<< endl;
				}else{
					cout << "Diminution des PM"<< endl;
					player->setMovement(player->getMovement()-2);
				}
			}
			//desert
			if(current_field_type==SAND){
				chanceEffet=rand()%100 + 1;
				if(chanceEffet>=50){
					cout << "Effet baisse d'initative rate "<< endl;
				}else{
					cout << "Baisse d'initative du joueur"<< endl;
					player->setInitiative(player->getInitiative()-1);
				}
			}
			
			//Avancement du tour d'action
			//Si ce n'est pas le dernier joueur du round
			if(i<state.getPlayers().size()-1){ 
				state.setPlaying(state.getPlayers()[i+1]);
				cout << endl << "---------- Tour du joueur " << state.getPlaying()->getName() << " ----------" << endl;
				return;
			}
			//Fin de round
			else{ 
				cout << endl << "[ Fin de round "<< state.getRound() <<"]" << endl;
				state.setRound(state.getRound()+1);
				state.sortPlayers();
				cout << "Nouvel ordre de jeu : ";
				for (Player* p : state.getPlayers()){
					cout << p->getName() << " ";
				}
				cout << endl;
				
			//------------------------changment aléatoire d'état du terrain------------------------
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
				cout << endl << "[ Début de round "<< state.getRound() <<"]" << endl;
				cout << endl << "---------- Tour du joueur " << state.getPlaying()->getName() << " ----------" << endl;
				return;
			}
		}
	}
	cout << "Joueur non trouvé pour la fin d'action" << endl;
	return;
}

