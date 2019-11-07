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
			
			//Résolution des statuts
			std::vector<std::pair<CharStatusId, int>> status = player->getStatus();
			if (status[0].second>0){
				cout << "Degats de brulure" << endl;
			}
			if (status[1].second>0){
				cout << "Degats de poison" << endl;
			}
			
			//Décrémentation de la durée des statuts
			for(size_t j; j<status.size(); j++){
				if(status[i].second>0){
					player->updateStatus({status[i].first,status[i].second-1});
					cout << "Decrementation de la duree du status " << i << endl;
				}
			}
			
			//Avancement du tour d'action
			if(i<state.getPlayers().size()-1){ //Si ce n'est pas le dernier joueur du round
				//cout << "playing : " << state.getPlaying() << " | newplaying : " << state.getPlayers()[i+1] << endl;
				state.setPlaying(state.getPlayers()[i+1]);
				cout << "Fin d'action et joueur suivant" << endl;
			}
			else{
				cout << "Fin de round" << endl;
			}
			
			return;
		}
	}
	cout << "Joueur non trouvé pour la fin d'action" << endl;
	return;
}

