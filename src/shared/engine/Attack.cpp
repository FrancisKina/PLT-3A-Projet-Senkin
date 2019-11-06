/*#include "Attack.h"
#include <iostream> 
#include <unistd.h>
#include <stdlib.h>

using namespace engine;
using namespace state;
using namespace std;

Attaque::Attaque (state::Character& ptr_attaquant, std::pair<int,int> positionCible, state::Skill attaque){
	id = ATTAQUE;
	joueur=newJoueur;
}
    
/*
void Attaque::execute (state::State& state, state::Skill attaque){
	bool attaque_possible=false;
	//vector<Position> listePosAtq=attaquant.getLegalAttack(etat);
	
	if (attaquant.getSkillCount()!=0) && attaquant.getHP()>0){
		//on considère uniquement les attaques horizontales et verticales
		//cas pas de 0 dans range
			/*int rangeSize=4*(attaque.getRange().second-attaque.getRange().first+1);
			for(size_t j=0; j<rangeSize; j++){
				if(listePosAtq[j].equals(cible.getPosition())){
					attaque_possible=true;
					break;
				}
				
			}*//*
			//teste renvoyant la liste position attaquable
			for(int i=1, i<5,i++){
				for(int j = attaque.getRange().first, j<attaque.getRange().second +1, j++){
					int x = attaquant.getX();
					int y = attaquant.getY();
					std::pair<FieldStatusId, int> statusCase = state.getGrid[
					if(){
					}
					if(j=attaque.getRange().second){
						cout << "la direction n°" << i << " n'est pas bloqué" <<endl;
					}
				}
			}
		
		
		
		
		
		
		if(attaque_possible){
			
				int attaque_attaquant=attaquant.getStatistiques().getAttaque();
				int critique_attaquant=attaquant.getStatistiques().getCritique();
				string nomArme_attaquant=attaquant.getNomArme();

				int defense_cible=cible.getStatistiques().getDefense();
				int pv_cible=cible.getStatistiques().getPV();
				int esquive_cible=cible.getStatistiques().getEsquive();
				string nomArme_cible=cible.getNomArme();
				
								
				if (contreAtk == true){
					cout << "\tCONTRE-ATTAQUE" << endl;
				}
				
				//-----------------triangle des armes--------------------------------
				int bonus_attaque=-1;
				int bonus_esquive=-1;
				string afficheBonus;

				if(nomArme_cible==nomArme_attaquant){
					bonus_attaque=0;
					bonus_esquive=0;
				}
				else if(nomArme_attaquant=="Arc" || nomArme_cible=="Arc"){
					bonus_attaque=0;
					bonus_esquive=0;
				}
				else if((nomArme_attaquant=="Hache" && nomArme_cible=="Lance")|| (nomArme_attaquant=="Lance" && nomArme_cible=="Epee") || (nomArme_attaquant=="Epee" && nomArme_cible=="Hache")){
					bonus_attaque=5;
					bonus_esquive=5;
					afficheBonus = "\t|\tBonus d'arme pour " + attaquant.getNom() + ": +5 en ATTAQUE et +5 en ESQUIVE";
				}
				else if ((nomArme_cible=="Hache" && nomArme_attaquant=="Lance")|| (nomArme_cible=="Lance" && nomArme_attaquant=="Epee") || (nomArme_cible=="Epee" && nomArme_attaquant=="Hache")){
					bonus_attaque=-5;
					bonus_esquive=-5;
					afficheBonus = "\t|\tMalus d'arme pour " + attaquant.getNom() + ": -5 en ATTAQUE et -5 en ESQUIVE";
				}

				//------------------------succes ou echec----------------------------------
				cout << "\t- " << attaquant.getNom() << " attaque " << cible.getNom() << " ! -" << endl;
				cout << afficheBonus << endl;
				srand(time(NULL));
				int chanceEsquive=rand()%100 + 1;
				
				//------------------------echec de l'attaque-------------------------------
				if(chanceEsquive<=esquive_cible + bonus_esquive){
					cout << "\t|\t " << cible.getNom() << " évite l'attaque." << endl;
					cout << "\t|\t L'attaque échoue ! " << endl;
				}
				//------------------------succes de l'attaque-------------------------------
				else{
				//------------------------Calcul bonus critique-------------------------------------
					srand(time(NULL));
					int chanceCritique= rand()%100 + 1 ;
					int bonus_critique=0;
					if(chanceCritique<=critique_attaquant){
						bonus_critique=5;
						cout << "\t|\t COUP CRITIQUE ! (+" << bonus_critique << " dégâts)" << endl;
					}

				//-------------------------Calcul degats------------------------------------
					int degats=attaque_attaquant-defense_cible + bonus_attaque + bonus_critique;
					if (degats < 0){
						degats = 0;
					}
				//---------------------------Attaque--------------------------------------
					cible.getStatistiques().setPV(pv_cible-degats);
					cout << "\t|\t " << cible.getNom() << " perd " << degats << " PV. " << endl;
					cout << "\t|\t Il ne lui reste plus que " << cible.getStatistiques().getPV() << " PV."<< endl;
				}

				if(cible.getStatistiques().getPV()==0){
					cible.setStatut(MORT);
					cible.getPosition().setX(-1);
					cible.getPosition().setY(-1);
					cout << "\t\t++ " << cible.getNom() << " est mort. ++" << endl;
					FinActions finattaque(attaquant, joueur);
					sleep(2);
					finattaque.execute(etat);
				}
				
				else{
					if(contreAtk==false){
						Attaque contre_attaque(cible, attaquant, !joueur);
						contre_attaque.contreAtk=true;
						sleep(2);
						contre_attaque.execute(etat);	
					}
					if(contreAtk==true){
						*//*// Le tour du contre-attaquant ne se termine pas apres sa contre-attaque
						if(attaquant.getType()==ARCHER && cible.getType()!=ARCHER){
							FinActions finattaque(attaquant, joueur);
							sleep(2);
							finattaque.execute(etat);		
						}*//*
						//else{
							FinActions finattaque(cible, joueur);
							sleep(2);
							finattaque.execute(etat);
						//}		
					}
					
				}
			
		}
		
		// Cas attaque impossible
		else{
			cout << "Attaque non autorisée !" << endl;		
		}
	}
	else if(attaquant.getSkillCount()==0){
		cout << "le joueur n°" << attaquant.getPlayerId() << " a utilisé tout ses PA, il ne peut plus attaquer." <<endl;  
	}
	
	cout << "\n" ;
}

//void getCibles(
	//prend la position central de l'attaque et renvoie la zone d'effet de l'attaque et la liste des persos touché
	*/