#include "Attack.h"
#include <iostream> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <tuple>

#include <vector>

using namespace engine;
using namespace state;
using namespace std;

Attack::Attack (std::pair<int,int> newPosTarget, int newnSkill){
	id = ATTACK;
	n = newnSkill;
	nSkill = newnSkill;
	pos = newPosTarget;
	posTarget = newPosTarget;
	attaque_possible = false;
	direction_skill = SOUTH;
}
    

void Attack::execute (state::State& state){
	player = state.getPlaying();
	Player* attacker = player;
	attack = attacker->getSkills()[nSkill];
	
	cout << endl << "| Lancement d'une attaque |" << endl;
	
	if (attacker->getHp()>0 && attack->getCooldown()<=0 && attacker->getSkillCount() > 0){
		cout << attacker->getName() << " tente d'utiliser " << attack->getName() << endl;
		
		state.addTextInfo(attacker->getName() + " UTILISE " + attack->getName());
		
		int X = attacker->getX();
		int Y = attacker->getY();
		//case
		Field *casePlayer = state.getGrid()[Y][X];
		Field *caseCible = state.getGrid()[posTarget.second][posTarget.first];
		Field *caseInter;
		Field *caseTarget;
		//les deux lignes suivante a definir comme sortie 2 et 1 de gettarget
		std::vector<Player*> targetPlayer;
		std::vector<Field*> v_caseField;
		std::vector<std::pair<int,int>> v_casePos;
		std::vector<std::tuple<EffectId, int, int>> effect = attack->getEffect();

		std::pair<bool,state::DirectionId> checkdir = checkDirection(state);
		attaque_possible = checkdir.first;
		direction_skill = checkdir.second;

		std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> gt = getTarget(state);
		v_casePos=gt.first;
		targetPlayer = gt.second;

		for(size_t i=0;i<v_casePos.size();i++){
			v_caseField.push_back(state.getGrid()[v_casePos[i].second][v_casePos[i].first]);
		}
		
		int range_first=attack->getRange().first;
		int range_second=attack->getRange().second;
		if(!attack->getSpecial() && attack->getRange().second>1){
		//effet gravité
			if (casePlayer->getFieldStatus()[14].second>0){
				range_second = attack->getRange().second-1;
				if(attack->getRange().first>1){
					range_first = attack->getRange().first-1;
				}
				attack->setRange({range_first,range_second});
				cout<<"La gravité diminue la porté de l'attaque."<<endl;
			}
		}
		//effet tour
		if (casePlayer->getFieldStatus()[21].second>0){
			range_second = attack->getRange().second+1;
				range_first = attack->getRange().first;
			attack->setRange({range_first,range_second});
			cout<<"La tour augmente la porté de l'attaque."<<endl;
		}
		
		//VPT effet attaque passe-muraille (attaque_possible = true)
		//VPT test effet attaque lance meme en cas d'echec (contrecoup par exemple)
		
		if(attaque_possible){
			//une attaque est utilise
			attacker->setSkillCount(attacker->getSkillCount()-1);
			attack->setCooldown(attack->getCooldownBase());
			
			//stats attaquant
			int attacker_damage=attacker->getCharacter()->getAttackBase();
			
			//stats skill
			float skill_damage = (float)attack->getDamage();
			float skill_precision = (float)attack->getPrecision();
			bool skill_special =attack->getSpecial();
			//stats personnage attaque
			std::vector<float> target_HP;
			std::vector<float> target_Dodge;
			std::vector<std::vector<std::pair<CharStatusId,int>>> target_Status;
			
			if(targetPlayer.size()!=0){
				for(size_t i=0;i<targetPlayer.size();i++){
					cout<<"Joueur "<<targetPlayer[i]->getName()<<" est vise"<<endl;
					target_HP.push_back(targetPlayer[i]->getHp());
					target_Dodge.push_back(1);
					target_Status.push_back(targetPlayer[i]->getStatus());
				}
			}else{
				cout<<"Aucun joueur n'est vise"<<endl;
			}

			
			//-----------------avantage terrain----------------------------------------
			//attaquant (terrain de l'attaquant)
			for(size_t i=0; i<casePlayer->getFieldStatus().size();i++){
				//Brume
				if (casePlayer->getFieldStatus()[i].first==MIST && casePlayer->getFieldStatus()[i].second!=0){
					skill_precision=skill_precision*0.75;
					cout<<"La brume diminiue la precision de l'attaque (attaquant)."<<endl;
				}
				//gravite
				if (casePlayer->getFieldStatus()[i].first==GRAVITY && casePlayer->getFieldStatus()[i].second!=0){
					if(range_first==0 && range_second==0 && !skill_special){
						skill_damage=skill_damage*1.5;
						cout<<"La gravite augemente les degats de l'attaque au corps a corps (attaquant)."<<endl;
					}
				}
				//fort
				if (casePlayer->getFieldStatus()[i].first==FORT && casePlayer->getFieldStatus()[i].second!=0){
					
						skill_damage=skill_damage*1.25;
						cout<<"Le chateau augemente les degats de l'attaque (attaquant)."<<endl;
					
				}
			}
			//terrain cible
			//status
			for(size_t i=0; i<caseCible->getFieldStatus().size();i++){
				//Brume
				if (caseCible->getFieldStatus()[i].first==MIST && caseCible->getFieldStatus()[i].second!=0){
					skill_precision=skill_precision*0.75;
					cout<<"La brume diminiue la precision de l'attaque (case cible)."<<endl;
				}
				//Fort
				if (caseCible->getFieldStatus()[i].first==FORT && caseCible->getFieldStatus()[i].second!=0){
					skill_precision=skill_precision*0.75;
					cout<<"La chateaux diminiue la precision de l'attaque (case cible)."<<endl;
				}
			}
			//type
			if (caseCible->getFieldType()==MOUNTAIN){
				if(range_first==0 && range_second==0&& !skill_special){
					skill_precision=skill_precision*0.85;
					cout<<"La montagne baisse la precision de l'attaque au corps a corps (case cible)."<<endl;
				}
			}else if (caseCible->getFieldType()==FOREST){
				if((range_first==0 || range_second==0)){
					skill_precision=skill_precision*0.85;
					cout<<"La foret baisse la precision de l'attaque a distance (case cible)."<<endl;
				}
			}
			//attaque (terrain de l'attaque XOR terrain cible)
			for(size_t p=0;p<targetPlayer.size();p++){
				caseTarget = state.getGrid()[targetPlayer[p]->getY()][targetPlayer[p]->getX()];
				if (caseTarget!=caseCible){
					for(size_t i=0; i<caseTarget->getFieldStatus().size();i++){
						//Brume
						if (caseTarget->getFieldStatus()[i].first==MIST && caseTarget->getFieldStatus()[i].second!=0){
							target_Dodge[p]=target_Dodge[p]*0.75;
							cout<<"La brume diminiue la precision de l'attaque n°"<<p<<"."<<endl;
						}
					}
					if (caseTarget->getFieldType()==MOUNTAIN){
						if(range_first==0 && range_second==0&& !skill_special){
							target_Dodge[p]=target_Dodge[p]*0.85;
							cout<<"La montagne baisse la precision de l'attaque au corps a corps sur l'attaque n°"<<p<<"."<<endl;
						}
					}else if (caseTarget->getFieldType()==FOREST){
						if((range_first==0 || range_second==0)){
							target_Dodge[p]=target_Dodge[p]*0.85;
							cout<<"La foret baisse la precision de l'attaque a distance sur l'attaque n°"<<p<<"."<<endl;
						}
					}
				}
			}
			
			//attaque a distance (terrain entre le terrain cible et l'attaquant)
			//uniquement pris en compte le centre de l'attaque si eloigne de plus de 2 case
			if(abs(posTarget.first-X)>1 || abs(posTarget.second-Y)>1){
				if(direction_skill==NORTH){
					for(int k=1;k<abs(posTarget.second-Y)-1;k++){
						caseInter = state.getGrid()[Y-k][X];
						for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
							//Brume
							if (caseInter->getFieldStatus()[i].first==MIST && caseInter->getFieldStatus()[i].second!=0){
								skill_precision=skill_precision*0.75;
								cout<<"La brume diminiue la precision de l'attaque a distance (inter)."<<endl;
							}
						}
					}
				}else if(direction_skill==SOUTH){
					for(int k=1;k<abs(posTarget.second-Y)-1;k++){
						caseInter = state.getGrid()[Y+k][X];
						for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
							//Brume
							if (caseInter->getFieldStatus()[i].first==MIST && caseInter->getFieldStatus()[i].second!=0){
								skill_precision=skill_precision*0.75;
								cout<<"La brume diminiue la precision de l'attaque a distance (inter)."<<endl;
							}
						}
					}
				}else if(direction_skill==WEST){
					for(int k=1;k<abs(posTarget.second-X)-1;k++){
						caseInter = state.getGrid()[Y][X-k];
						for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
							//Brume
							if (caseInter->getFieldStatus()[i].first==MIST && caseInter->getFieldStatus()[i].second!=0){
								skill_precision=skill_precision*0.75;
								cout<<"La brume diminiue la precision de l'attaque a distance (inter)."<<endl;
							}
						}
					}
				}else if(direction_skill==EAST){
					for(int k=1;k<abs(posTarget.second-X)-1;k++){
						caseInter = state.getGrid()[Y][X+k];
						for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
							//Brume
							if (caseInter->getFieldStatus()[i].first==MIST && caseInter->getFieldStatus()[i].second!=0){
								skill_precision=skill_precision*0.75;
								cout<<"La brume diminiue la precision de l'attaque a distance (inter)."<<endl;
							}
						}
					}
				}
						
			}
			//-----------------avantage etat-------------------------------------------
			for(size_t i=0; i<attacker->getStatus().size();i++){
				if (attacker->getStatus()[i].first==DAZZLED && attacker->getStatus()[i].second!=0){
					skill_precision=skill_precision*0.75;
					cout<<"L'attacker est ebloui, la precision de l'attaque est diminue."<<endl;
				}
			}

			//----------------avantage effet attaque-----------------------------------
			for(size_t i=0; i<effect.size();i++){
				if(get<0>(effect[i])==BOOST_ATK_RAIN){
					//augemente les degat si il pleut
					if(direction_skill==NORTH){
						for(int k=0;k<abs(posTarget.second-Y);k++){
							caseInter = state.getGrid()[Y-k][X];
							for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
								if (caseInter->getFieldStatus()[i].first==RAIN && caseInter->getFieldStatus()[i].second!=0){
									skill_damage=skill_damage*2;
									cout<<"La pluie augemente de l'attaque."<<endl;
									break;//non cumulable
								}
							}
						}
					}else if(direction_skill==SOUTH){
						for(int k=0;k<abs(posTarget.second-Y);k++){
							caseInter = state.getGrid()[Y+k][X];
							for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
								if (caseInter->getFieldStatus()[i].first==RAIN && caseInter->getFieldStatus()[i].second!=0){
									skill_damage=skill_damage*2;
									cout<<"La pluie augemente de l'attaque."<<endl;
									break;//non cumulable
								}
							}
						}
					}else if(direction_skill==WEST){
						for(int k=0;k<abs(posTarget.second-X);k++){
							caseInter = state.getGrid()[Y][X-k];
							for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
								if (caseInter->getFieldStatus()[i].first==RAIN && caseInter->getFieldStatus()[i].second!=0){
									skill_damage=skill_damage*2;
									cout<<"La pluie augemente de l'attaque."<<endl;
									break;//non cumulable
								}
							}
						}
					}else if(direction_skill==EAST){
						for(int k=0;k<abs(posTarget.second-X);k++){
							caseInter = state.getGrid()[Y][X+k];
							for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
								if (caseInter->getFieldStatus()[i].first==RAIN && caseInter->getFieldStatus()[i].second!=0){
									skill_damage=skill_damage*2;
									cout<<"La pluie augemente de l'attaque."<<endl;
									break;//non cumulable
								}
							}
						}
					}
				}
			}
			//------------------------succes ou echec----------------------------------
			if(targetPlayer.size()!=0){
				cout << "Joueur " << attacker->getName() << " attaque ";
				for(size_t i=0; i<targetPlayer.size();i++){
					cout<<"joueur "<< targetPlayer[i]->getName() << " " ;
				}
				cout << endl;
			}else{
				cout<<"Joueur " << attacker->getName() << " attaque case vide ("<<posTarget.first<<","<<posTarget.second<<")."<<endl;
			}
			//Test de reussite
			//srand(time(NULL));
			int chanceEsquive=rand()%100 + 1;
			cout << "Test de Reussite : " << chanceEsquive << " | Precision de l'attaque : " << skill_precision << endl;
			//Tester case central : si reussite => effet zone terrain et perso central direct, effet zone perso =>refaire test
			//------------------------echec de l'attaque-------------------------------
			if(chanceEsquive>=skill_precision){
				cout << "Attaque ratee "<< endl;
				state.addTextInfo("ATTAQUE RATEE");
			
			//------------------------succes de l'attaque-------------------------------
			}else{
				int chanceEffet;
			//------------------------effet sur le terrain-------------------------------
				for(size_t i=0; i<effect.size();i++){
					if(get<0>(effect[i])==BURN_FIELD){
						//Fait passer l'etat du terrain a BURNING
						chanceEffet=rand()%100 + 1;
						if(chanceEffet>=get<2>(effect[i])){
							cout << "Effet BURN FIELD rate "<<endl;
						}else{
							cout << "Terrain incendie"<< endl;
							for(size_t f=0;f<v_caseField.size();f++){
								if (v_caseField[f]->getFieldStatus()[12].second < get<1>(effect[i])){
									v_caseField[f]->updateFieldStatus({BURNING,get<1>(effect[i])});
									//cout<<"La case "<<v_caseField[f]->getFieldType()<<" brulera pendant "<<get<1>(effect[i])<<" tours"<<endl;
								}
							}
						}
					}
					if(get<0>(effect[i])==POISON_FIELD){
						//Fait passer l'etat du terrain a POISON
						chanceEffet=rand()%100 + 1;
						if(chanceEffet>=get<2>(effect[i])){
							cout << "Effet POISON_FIELD rate "<< endl;
						}else{
							cout << "Terrain empoisonne"<< endl;
							for(size_t f=0;f<v_caseField.size();f++){
								if (v_caseField[f]->getFieldStatus()[13].second < get<1>(effect[i])){
									v_caseField[f]->updateFieldStatus({POISON,get<1>(effect[i])});
									//cout<<"La case "<<v_caseField[f]->getFieldType()<<" brulera pendant "<<get<1>(effect[i])<<" tours"<<endl;
								}
							}
						}
					}
				}
			//------------------------effet sur le lanceur-------------------------------
				for(size_t i=0; i<effect.size();i++){
					if(get<0>(effect[i])==MOVE_USER){
						//deplace le lanceur
						cout << "Le joueur bondit vers sa cible"<< endl;
						attacker->setX(posTarget.first);
						attacker->setY(posTarget.second);
					}
				}
			//------------------------ATTAQUE------------------------------------------
				if(targetPlayer.size()!=0){
			//-------------------------Calcul degats-----------------------------------
					int degats=(int)(attacker_damage*skill_damage/5);
					if (degats < 0){
						degats = 0;
					}
					for(size_t p=0; p<targetPlayer.size();p++){
			//------------------------succes ou echec sur zone-------------------------
						bool touche = true;						
						Field *caseTarget = state.getGrid()[targetPlayer[p]->getY()][targetPlayer[p]->getX()];
						if (caseTarget!=caseCible){
							chanceEsquive=rand()%100 + 1;
							int esquive= skill_precision*target_Dodge[p];
							if(chanceEsquive<=esquive){
								touche=false;
							}
						}
			//------------------------echec de l'attaque-------------------------------
						if(!touche){
							cout << "Joueur "<<targetPlayer[p]->getName()<<" esquive l'attaque"<< endl;
			//------------------------succes de l'attaque-------------------------------
						}else{
			//---------------------------Degat subis-----------------------------------
							targetPlayer[p]->setHp(target_HP[p]-degats);
							cout << "Joueur " << targetPlayer[p]->getName() << " perd " << degats << " PV " << endl;
							cout << "Il ne lui reste plus que " << targetPlayer[p]->getHp() << " PV"<< endl;
							
							if (degats > 0){
								state.addTextInfo(targetPlayer[p]->getName() + " PERD " + std::to_string(degats) + " PV");
							}
						}
						if(targetPlayer[p]->getHp()>0){
							for(size_t i=0; i<effect.size();i++){
								//POISON
								if(get<0>(effect[i])==POISON_CHAR){
									//fait passer l'etat du perso a POISON
									chanceEffet=rand()%100 + 1;
									if(chanceEffet<=get<2>(effect[i])){
										cout << "Effet POISON rate "<< endl;
									}else{
										cout << "Joueur empoisonne"<< endl;
										bool test_fc=true;
										for(size_t s=0; s<targetPlayer[p]->getStatus().size();s++){
											if ((targetPlayer[p]->getStatus()[s].first==POISONED) && (targetPlayer[p]->getStatus()[s].second>get<1>(effect[i]))){
												test_fc=false;
												break;
											}
										}
										if(test_fc){
											std::pair<CharStatusId, int> newStatus = {POISONED,get<1>(effect[i])};
											targetPlayer[p]->updateStatus(newStatus);
											cout<<"Le joueur "<<targetPlayer[p]->getName()<<" sera empoisonne pendant "<<get<1>(effect[i])<<" tours"<<endl;
										}
									}
								}
								//BURN
								if(get<0>(effect[i])==BURN_CHAR){
									//fait passer l'etat du perso a BURN
									chanceEffet=rand()%100 + 1;
									if(chanceEffet>=get<2>(effect[i])){
										cout << "Effet BURN rate"<< endl;
									}else{
										cout << "Joueur brulé"<< endl;
										bool test_fc=true;
										for(size_t s=0; s<targetPlayer[p]->getStatus().size();s++){
											if ((targetPlayer[p]->getStatus()[s].first==BURNED) && (targetPlayer[p]->getStatus()[s].second>get<1>(effect[i]))){
												test_fc=false;
												break;
											}
										}
										if(test_fc){
											std::pair<CharStatusId, int> newStatus = {BURNED,get<1>(effect[i])};
											targetPlayer[p]->updateStatus(newStatus);
											cout<<"Le joueur "<<targetPlayer[p]->getName()<<" sera brulé pendant "<<get<1>(effect[i])<<" tours"<<endl;
										}
									}
								}
								//FEAR
								if(get<0>(effect[i])==FEAR_CHAR){
									//fait passer l'etat du perso a FEAR
									chanceEffet=rand()%100 + 1;
									if(chanceEffet>=get<2>(effect[i])){
										cout << "Effet FEAR rate "<< endl;
									}else{
										cout << "Joueur appeuré"<< endl;
										bool test_fc=true;
										for(size_t s=0; s<targetPlayer[p]->getStatus().size();s++){
											if ((targetPlayer[p]->getStatus()[s].first==FEAR) && (targetPlayer[p]->getStatus()[s].second>get<1>(effect[i]))){
												test_fc=false;
												break;
											}
										}
										if(test_fc){
											std::pair<CharStatusId, int> newStatus = {FEAR,get<1>(effect[i])};
											targetPlayer[p]->updateStatus(newStatus);
											cout<<"Le joueur "<<targetPlayer[p]->getName()<<" aura peur pendant "<<get<1>(effect[i])<<" tours"<<endl;
										}
									}
								}
								//DAZZLE
								if(get<0>(effect[i])==DAZZLE_CHAR){
									//fait passer l'etat du perso a DAZZLE

									chanceEffet=rand()%100 + 1;
									if(chanceEffet>=get<2>(effect[i])){
										cout << "Effet DAZZLE rate "<< endl;
									}else{
										cout << "Joueur ébloui"<< endl;
										bool test_fc=true;
										for(size_t s=0; s<targetPlayer[p]->getStatus().size();s++){
											if ((targetPlayer[p]->getStatus()[s].first==DAZZLED) && (targetPlayer[p]->getStatus()[s].second>get<1>(effect[i]))){
												test_fc=false;
												break;
											}
										}
										if(test_fc){
											std::pair<CharStatusId, int> newStatus = {DAZZLED,get<1>(effect[i])};
											targetPlayer[p]->updateStatus(newStatus);
											cout<<"Le joueur "<<targetPlayer[p]->getName()<<" sera ébloui pendant "<<get<1>(effect[i])<<" tours"<<endl;
										}
									}
								}
								//Heal status
								if(get<0>(effect[i])==HEAL_STATUS){
									//soigne les status
									chanceEffet=rand()%100 + 1;
									if(chanceEffet<=get<2>(effect[i])){
										cout << "Effet HEAL STATUS rate "<< endl;
									}else{
										cout << "Soin status"<< endl;
										for(size_t s=0; s<targetPlayer[p]->getStatus().size();s++){
												targetPlayer[p]->getStatus()[s].second=0;
										}
									}
								}
								//HEAL Medium
								if(get<0>(effect[i])==HEAL_MEDIUM){
									//soigne les PV
									chanceEffet=rand()%100 + 1;
									if(chanceEffet>=get<2>(effect[i])){
										cout << "Effet HEAL(M) rate "<< endl;
									}else{
										cout << "Soin moyen"<< endl;
										int heal = 0.1*target_HP[p];
										if(target_HP[p]+heal>targetPlayer[p]->getCharacter()->getHpBase()){
											heal = targetPlayer[p]->getCharacter()->getHpBase()-target_HP[p];
										}
										targetPlayer[p]->setHp(heal+target_HP[p]);
										cout<<"Le joueur "<<targetPlayer[p]->getName()<<" est soigne de "<<heal<<" PV"<<endl;
									}
								}
								//HEAL LOW
								if(get<0>(effect[i])==HEAL_LOW){
									//soigne les PV
									chanceEffet=rand()%100 + 1;
									if(chanceEffet>=get<2>(effect[i])){
										cout << "Effet HEAL(L) rate "<< endl;
									}else{
										cout << "Soin faible"<< endl;
										int heal = 0.05*target_HP[p];
										if(target_HP[p]+heal>targetPlayer[p]->getCharacter()->getHpBase()){
											heal = targetPlayer[p]->getCharacter()->getHpBase()-target_HP[p];
										}
										targetPlayer[p]->setHp(target_HP[p]+heal);
										cout<<"Le joueur "<<targetPlayer[p]->getName()<<" est soigne de "<<heal<<" PV"<<endl;
									}
								}
								//HEAL HIGH
								if(get<0>(effect[i])==HEAL_HIGH){
									//soigne les PV
									chanceEffet=rand()%100 + 1;
									if(chanceEffet>=get<2>(effect[i])){
										cout << "Effet HEAL(H) rate "<< endl;
									}else{
										cout << "Soin fort"<< endl;
										int heal = 0.2*target_HP[p];
										if(target_HP[p]+heal>targetPlayer[p]->getCharacter()->getHpBase()){
											heal = targetPlayer[p]->getCharacter()->getHpBase()-target_HP[p];
										}
										targetPlayer[p]->setHp(target_HP[p]+heal);
										cout<<"Le joueur "<<targetPlayer[p]->getName()<<" est soigne de "<<heal<<" PV"<<endl;
									}
								}
								
								//Limite de vie maximale pour le soin
								if (targetPlayer[p]->getHp() > targetPlayer[p]->getCharacter()->getHpBase()){
									targetPlayer[p]->setHp(targetPlayer[p]->getCharacter()->getHpBase());
								}
								
								//MOVE FOE
								if(get<0>(effect[i])==MOVE_USER){
									//deplace la cible
									cout << "Le joueur est bouscule vers une autre case"<< endl;
									int mf_x,mf_y;
									int x_m=-1;
									int x_p=1;
									int y_m=1;
									int y_p=-1;
									std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[targetPlayer[p]->getY()][targetPlayer[p]->getX()+1]->getFieldStatus();
									for(size_t i=0; i<statusCase.size();i++){
										if (statusCase[i].first==BLOCKMOVE && statusCase[i].second!=0){
											x_p=0;
										}
									}
									statusCase = state.getGrid()[targetPlayer[p]->getY()+1][targetPlayer[p]->getX()]->getFieldStatus();
									for(size_t i=0; i<statusCase.size();i++){
										if (statusCase[i].first==BLOCKMOVE && statusCase[i].second!=0){
											y_p=0;
										}
									}
									statusCase = state.getGrid()[targetPlayer[p]->getY()][targetPlayer[p]->getX()-1]->getFieldStatus();
									for(size_t i=0; i<statusCase.size();i++){
										if (statusCase[i].first==BLOCKMOVE && statusCase[i].second!=0){
											x_m=0;
										}
									}
									statusCase = state.getGrid()[targetPlayer[p]->getY()-1][targetPlayer[p]->getX()]->getFieldStatus();
									for(size_t i=0; i<statusCase.size();i++){
										if (statusCase[i].first==BLOCKMOVE && statusCase[i].second!=0){
											y_m=0;
										}
									}
									
									int horizontal;
									horizontal=rand()%1;
									int mf;
									mf=rand()%1;
									
									
									if(y_m==0 and y_p==0){
										horizontal=1;
										if(x_m==0){
											mf=1;
										}else if(x_p==0){
											mf=0;
										}
									}
									if(x_m==0 and x_p==0){
										horizontal=0;
										if(y_m==0){
											mf=1;
										}else if(y_p==0){
											mf=0;
										}
									}
									
									
									if(horizontal==1){
										mf_y=0;
										if(mf==1){
											mf_x=x_p;
										}else{
											mf_x=x_m;
										}
									}else{
										mf_x=0;
										if(mf==1){
											mf_y=y_p;
										}else{
											mf_y=y_m;
										}
									}
									targetPlayer[p]->setX(targetPlayer[p]->getX()+mf_x);
									targetPlayer[p]->setY(targetPlayer[p]->getY()+mf_y);
								}
							}
						}							
					}
				}
			}
		// Cas attaque impossible
		}else{
			cout << "Attaque non autorisee ! (Hors de portée)" << endl;		
		}
		
	}else if(attacker->getSkillCount()==0){
		cout << "Le joueur " << attacker->getName() << " a utilise tous ses PA, il ne peut plus attaquer." <<endl;  
	}else if(attack->getCooldown()>0){
		cout << "L'attaque n'a pas fini son chargement, le joueur ne peut plus attaquer." <<endl;  
	}else {
		cout<<"Erreur"<<endl;
	}
}

std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> Attack::getTarget(state::State& state){
	//prend la position central de l'attaque et renvoie la zone d'effet de l'attaque et la liste des persos touche
		
		std::vector<Field*> v_caseField;
		std::vector<std::pair<int,int>> v_posField;
		std::vector<Player*> players = state.getPlayers();
		std::vector<state::Player*> targets;
		size_t sizePlayers = players.size();
		int pos_x;
		int pos_y;
		bool is_blocked;
		
		//verifier si il n'ya pas de blockattck et si attaque possible
		if(attaque_possible){
			for(size_t i=0;i<attack->getArea().size();i++){
				is_blocked=false;
				//cout << direction_skill << endl;
				if(direction_skill==NORTH){
					pos_y=posTarget.second+attack->getArea()[i].second;
					pos_x=posTarget.first+attack->getArea()[i].first;
				}else if(direction_skill==EAST){
					pos_y=posTarget.second+attack->getArea()[i].first;
					pos_x=posTarget.first-attack->getArea()[i].second;
				}else if(direction_skill==SOUTH){
					pos_y=posTarget.second-attack->getArea()[i].second;
					pos_x=posTarget.first-attack->getArea()[i].first;
				}else if(direction_skill==WEST){
					pos_y=posTarget.second-attack->getArea()[i].first;
					pos_x=posTarget.first+attack->getArea()[i].second;
				}
				
				v_caseField.push_back(state.getGrid()[pos_y][pos_x]);
				std::vector<std::pair<state::FieldStatusId,int>> statusCase = v_caseField[i]->getFieldStatus();
				for(size_t c=0; c<statusCase.size();c++){
					if (statusCase[c].first==BLOCKATTACK && statusCase[c].second!=0){
						is_blocked=true;
					}
				}
				if(!is_blocked){
					//cout << "Attaque non bloquee par un obstacle" << endl;
					v_posField.push_back({pos_x,pos_y});
					for(size_t p = 0; p<sizePlayers;p++){
						if(players[p]->getX()==pos_x && players[p]->getY()==pos_y ){
							targets.push_back(players[p]);
						}
					}
				}

			}
		}
		
		return {v_posField,targets};
}

std::pair<bool,state::DirectionId> Attack::checkDirection(state::State& state){
	player = state.getPlaying();
	Player* attacker = player;
	
	//checkdirection
	
	int X = attacker->getX();
	int Y = attacker->getY();
	DirectionId direction_skill;
	int range_first=attack->getRange().first;
	int range_second=attack->getRange().second;
	Field *casePlayer = state.getGrid()[Y][X];
	//effet tour
	if (casePlayer->getFieldStatus()[21].second>0){
		range_second = attack->getRange().second+1;
		range_first = attack->getRange().first;
		attack->setRange({range_first,range_second});
		cout<<"La tour augmente la porté de l'attaque."<<endl;
	}
	if((Y-range_first)>=posTarget.second &&(Y-range_second)<=posTarget.second && X==posTarget.first){
		for(int j = range_first; j<range_second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y-j][X]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "La direction NORD est bloquee" <<endl;
					break;
				}	
				if(Y-j==posTarget.second && i==statusCase.size()-1){
					//cout << "La direction NORD n'est pas bloquee" <<endl;
					direction_skill=NORTH;
					attaque_possible=true;
				}
			}
		}
	}else if((Y+range_first)<=posTarget.second && (Y+range_second)>=posTarget.second && X==posTarget.first){
		for(int j = range_first; j<range_second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y+j][X]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "La direction SUD est bloquee" <<endl;
					break;
				}	
				if(Y+j==posTarget.second && i==statusCase.size()-1){
					//cout << "La direction SUD n'est pas bloquee" <<endl;
					direction_skill=SOUTH;
					attaque_possible=true;
				}
			}
		}
	}else if((X-range_first)>=posTarget.first && (X-range_second)<=posTarget.first && Y==posTarget.second){
		for(int j = range_first; j<range_second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y][X-j]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "La direction OUEST est bloquee" <<endl;
					break;
				}	
				if(X-j==posTarget.first && i==statusCase.size()-1){
					//cout << "La direction OUEST n'est pas bloquee" <<endl;
					direction_skill=WEST;
					attaque_possible=true;
				}
			}
		}
	}else if((X+range_first)<=posTarget.first && (X+range_second)>=posTarget.first && Y==posTarget.second){
		for(int j = range_first; j<range_second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y][X+j]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "La direction EST est bloquee" <<endl;
					break;
				}	
				if(X+j==posTarget.first && i==statusCase.size()-1){
					//cout << "La direction EST n'est pas bloquee" <<endl;
					direction_skill=EAST;
					attaque_possible=true;
				}
			}
		}
	}else{
		attaque_possible=false;
	}
	return {attaque_possible,direction_skill};
	//-----------------------------------
}

Json::Value Attack::serialize(){
	Json::Value newCmd;
	newCmd["id"] = this->id;
	newCmd["joueur"] = this->player->getName();;
	newCmd["xDestination"] = this->posTarget.first;
	newCmd["yDestination"] = this->posTarget.second;
	newCmd["attaque"] = this->nSkill;
	
	return newCmd;
}
