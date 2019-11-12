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
			for(size_t j; j<status.size(); j++){
				if(status[j].second>0){
					player->updateStatus({status[j].first,status[j].second-1});
					cout << "Decrementation de la duree du status " << j << endl;
				}
			}
			
			//Décrémentation de la durée des statuts terrain
			std::vector<std::vector<Field*>>& field_grid = state.getGrid();
			std::vector<std::pair<FieldStatusId,int>> field_status;
			for(size_t fy; fy<field_grid.size(); fy++){
				for(size_t fx; fx<field_grid[fy].size(); fx++){
					field_status=field_grid[fy][fx]->getFieldStatus();
				}
				for(size_t s;s<field_status.size(); s++){
					if(field_status[s].second>0){
						//field_status->updateStatus({field_status[s].first,status[s].second-1});
						cout << "Decrementation de la duree du status terrain n°" << s << endl;
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
			int chanceEffet;
			int X = player->getX();
			int Y = player->getY();
			
			std::vector<std::pair<FieldStatusId,int>> current_field_status;
			current_field_status = field_grid[Y][X]->getFieldStatus();
			FieldTypeId current_field_type = field_grid[Y][X]->getFieldType();
			//neige
			if(current_field_status[1].second>0){
			//if(current_field_status[s].first==SNOW && field_status[s].second>0){
				chanceEffet=rand()%100 + 1;
				cout<<chanceEffet<<" chance d'effet"<<endl;
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
					cout << "Effet BURN ratÃ© "<< endl;
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
				if(chanceEffet>=50){
					cout << "Effet poison ratÃ© "<< endl;
				}else{
					cout << "Joueur empoisonné"<< endl;
					bool test_fc=true;
					for(size_t s=0; s<status.size();s++){
						if ((status[s].first==POISONED) && (status[s].second>1)){
							test_fc=false;
							break;
						}
					}
					if(test_fc){
						std::pair<CharStatusId, int> newStatus = {BURNED,1};
						player->updateStatus(newStatus);
						cout<<"Le joueur "<<player->getName()<<" sera empoisonné pendant "<<1<<" tours"<<endl;
					}
				}
			} else if(current_field_type==SAND){
				//desert
				chanceEffet=rand()%100 + 1;
				if(chanceEffet>=50){
					cout << "Effet baisse d'initative ratÃ© "<< endl;
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
				
				//changment aléatoire d'état du terrain
				
				cout << "[ Nouveau round ]" << endl;
			}
			
			return;
		}
	}
	cout << "Joueur non trouvé pour la fin d'action" << endl;
	return;
}

