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
	//définition chance changement météo
	int chanceRain=70;
	int chanceSnow=5;
	int chanceMist=15;
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
							//cout << "Decrementation de la duree du status n°" << s <<" sur le terrain ("<<fx<<","<<fy<<"."<< endl;
							field_status=field_grid[fy][fx]->getFieldStatus();//redefinir pour actualiser
							if(field_status[0].second>0 && field_status[12].second>0){//s'il pleut et brule
								chanceEffet=rand()%100 + 1;
								if(chanceEffet<=50){
									cout << "La pluie affaiblie l'incendie" << endl;
									field_changed->updateFieldStatus({field_status[s].first,status[s].second-1});
								}
							}
							if(field_grid[fy][fx]->getFieldStatus()[s].second==0){
								cout<<"Fin du status n°"<<s<<endl;
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
			//if(current_field_status[s].first==SNOW && field_status[s].second>0){
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
					cout << "Joueur brulé"<< endl;
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
						cout<<"Le joueur "<<player->getName()<<" sera brulé pendant "<<2<<" tours"<<endl;
					}
				}
			}
			
			//marécage
			if(current_field_type==SWAMP){
				chanceEffet=rand()%100 + 1;
				if(chanceEffet>=50 || player->getMovement()<2){
					cout << "Effet reduction de PM rate "<< endl;
				}else{
					cout << "Baisse des PM du joueur"<< endl;
					player->setMovement(player->getMovement()-2);
				}
			} else if(current_field_type==SAND){
				//desert
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
				bool newcase;
				int proba_new_case= 80;
				int chanceFirstCase=rand()%100+1;
				if(proba_new_case>chanceFirstCase){
					newcase=true;
				}else{
					newcase=false;
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
				while(newcase){
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
					int chanceNewCase=rand()%100+1;
					if(proba_new_case>chanceNewCase){
						newcase=true;
					}else{
						newcase=false;
					}
				}
				cout << endl << "[ Début de round "<< state.getRound() <<"]" << endl;
				cout << endl << "---------- Tour du joueur " << state.getPlaying()->getName() << " ----------" << endl;
			}
			
			return;
		}
	}
	cout << "Joueur non trouvé pour la fin d'action" << endl;
	return;
}

