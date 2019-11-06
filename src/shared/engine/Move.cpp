#include "Move.h"
#include <iostream>

using namespace engine;
using namespace state;
using namespace std;

int sgn(int val){
	return (val>=0) - (val<=0);
}


Move::Move(state::Player* newPlayer, std::pair<int,int> newDestination){
	id = MOVE;
	player = newPlayer;
	destination = newDestination;
}

void Move::execute (state::State& state){
	
	//Generation de l'ensemble des coordonnees du chemin a prendre a partir de la position de depart et d'arrivee
	//TODO : AMELIORER PATHFINDING
	std::vector<std::pair<int,int>> path;
	int x = player->getX(), y = player->getY();
	int dx = destination.first, dy = destination.second;
	while((x!=dx) || (y!=dy)){ //Recherche de chemin utilisant la plus grande distance
		if(abs(x-dx)>abs(y-dy)){
			x = x-sgn(x-dx);
		}
		else{
			y = y-sgn(y-dy);
		}
		path.push_back({x,y});
	}
	path.push_back({dx,dy});
	
	
	//Avancee pas a pas avec path
	for(size_t i=0; i<path.size();i++){ //Parcourir la liste des terrains du chemin
		state::Field* field = state.getGrid()[path[i].first][path[i].second]; 
		for(size_t j=0; j < field->getFieldStatus().size(); j++){ //Parcourir la liste des statuts du terrain
			if(field->getFieldStatus()[j].first == POISON && field->getFieldStatus()[j].second > 0){
				cout << "Terrain : Joueur empoisonne" << endl;
			}
			if(field->getFieldStatus()[j].first == BURNING && field->getFieldStatus()[j].second > 0){
				cout << "Terrain : Joueur brule" << endl;
			}
		}
	} 
	
}
/*
	bool deplacementPossible=false;
	if (cible.getStatut()!=ATTENTE && cible.getStatut()!=MORT){

		if(cible.getChampMove()!=0){
			vector<Position> listePosMouv=cible.getLegalMove(etat);
			
			for(size_t j=0; j<listePosMouv.size(); j++){			
				if(listePosMouv[j].equals(destination)){
					deplacementPossible=true;
					break;
				}			
			}
			
			if(deplacementPossible){
				
				// Deduction du bonus precedent
				TerrainPraticable& refTerrainActuel = static_cast<TerrainPraticable&>(*etat.getGrille()[cible.getPosition().getY()][cible.getPosition().getX()]);
				
				cible.getStatistiques().setAttaque(cible.getStatistiques().getAttaque()-refTerrainActuel.getStatistiques().getAttaque());
				cible.getStatistiques().setDefense(cible.getStatistiques().getDefense()-refTerrainActuel.getStatistiques().getDefense());
				cible.getStatistiques().setEsquive(cible.getStatistiques().getEsquive()-refTerrainActuel.getStatistiques().getEsquive());
				cible.getStatistiques().setCritique(cible.getStatistiques().getCritique()-refTerrainActuel.getStatistiques().getCritique());
				
				// Modification de Position
				cible.getPosition().setX(destination.getX());
				cible.getPosition().setY(destination.getY());
				cible.setChampMove(cible.getChampMove()-1);
				cout << cible.getNom() << " se déplace sur la case de coordonnées [" << destination.getX() << ", " << destination.getY() << "] avec succès !" << endl;	
				cout << "\tIl lui reste " << cible.getChampMove() << " points de deplacement." << endl;
				
				// Nouveau bonus de terrain
				TerrainPraticable& refTerrainDestination = static_cast<TerrainPraticable&>(*etat.getGrille()[destination.getY()][destination.getX()]);
				
								
				if (refTerrainDestination.getTerrainPraticableID() == FORET){
					cible.getStatistiques().setEsquive(cible.getStatistiques().getEsquive()+refTerrainDestination.getStatistiques().getEsquive());
					cout << "+ Il obtient un bonus de +" ;
					cout << refTerrainDestination.getStatistiques().getEsquive() << " en ESQUIVE sur cette case FORET. +" << endl;
					
				}
				else if (refTerrainDestination.getTerrainPraticableID() == COLLINE){
					cible.getStatistiques().setDefense(cible.getStatistiques().getDefense()+refTerrainDestination.getStatistiques().getDefense());
					cout << "+ Il obtient un bonus de +" ;
					cout << refTerrainDestination.getStatistiques().getDefense() << " en DEFENSE sur cette case COLLINE. +" << endl;
				}
				
			}
			else{
				cerr << "Deplacement non autorise " << endl;
			}
		}
		else {
			cout<< "Deplacement impossible, tous les points de mouvement ont été utilisés pour ce tour." << endl;
		}
	}
	else if (cible.getStatut()==ATTENTE){
		cout << cible.getNom() << " a terminé toutes son tour d'actions, il ne peut plus se déplacer." <<endl; 
	}
	
	cout << "\n" ;
}
*/
