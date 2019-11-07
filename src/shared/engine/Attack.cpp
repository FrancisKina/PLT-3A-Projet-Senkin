#include "Attack.h"
#include <iostream> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <tuple>

using namespace engine;
using namespace state;
using namespace std;

Attack::Attack (std::pair<int,int> newPosTarget, state::Skill newAttack){
	id = ATTACK;
	attack = newAttack;
	posTarget = newPosTarget;
}
    

void Attack::execute (state::State& state){
	player = state.getPlaying();
	Player* attacker = player;
	
	bool attaque_possible=false;
	//TODO range f(gravity)
	if (attacker->getHp()>0 && attack.getCooldown()==0){
		int X = attacker->getX();
		int Y = attacker->getY();
		//case
		Field *casePlayer = state.getGrid()[Y][X];
		Field *caseCible = state.getGrid()[posTarget.second][posTarget.first];
		Field *caseInter;
		Field *caseTarget;
		//les deux lignes suivante a dÃ©finir comme sortie 2 et 1 de gettarget
		std::vector<Player*> targetPlayer;
		std::vector<Field*> v_caseField;
		std::vector<std::pair<int,int>> v_casePos;
		std::vector<std::tuple<EffectId, int, int>> effect = attack.getEffect();
		
		std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> gt = getTarget(state);
		v_casePos=gt.first;
		targetPlayer = gt.second;
		DirectionId direction_skill = checkDirection(state).second;
		attaque_possible = checkDirection(state).first;

		
		for(size_t i=0;i<v_casePos.size();i++){
			v_caseField.push_back(state.getGrid()[v_casePos[i].second][v_casePos[i].first]);
		}
		
		//VPT effet attaque passe-muraille (attaque_possible = true)
		//VPT test effet attaque lancÃ© mÃªme en cas d'echec (contrecoup par exemple)
		
		if(attaque_possible){
			//une attaque est utilisÃ©
			attacker->setSkillCount(attacker->getSkillCount()-1);
			attack.setCooldown(attack.getCooldownBase());
			
				//stats attaquant
				int attacker_damage=attacker->getCharacter()->getAttackBase();
				
				//stats skill
				float skill_damage = (float)attack.getDamage();
				float skill_precision = (float)attack.getPrecision();
				bool skill_special =attack.getSpecial();
				//stats peronnage attaquÃ©
				std::vector<float> target_HP;
				std::vector<float> target_Dodge;
				std::vector<std::vector<std::pair<CharStatusId,int>>> target_Status;
				if(targetPlayer.size()!=0){
					for(size_t i=0;i<targetPlayer.size();i++){
						cout<<"Joueur "<<targetPlayer[i]<<"visÃ©."<<endl;
						target_HP[i]=targetPlayer[i]->getHp();
						target_Dodge[i]=1;
						target_Status[i]=targetPlayer[i]->getStatus();
					}
				}else{
					cout<<"Aucun joueur visÃ©"<<endl;
				}

				
				//-----------------avantage terrain----------------------------------------
				//attaquant (terrain de l'attaquant)
				for(size_t i=0; i<casePlayer->getFieldStatus().size();i++){
					//Brume
					if (casePlayer->getFieldStatus()[i].first==MIST && casePlayer->getFieldStatus()[i].second!=0){
						skill_precision=skill_precision*0.75;
						cout<<"La brume diminiue la precision de l'attaque (attaquant)."<<endl;
					}
					//faire condition distance gravitÃ©
					if (casePlayer->getFieldStatus()[i].first==GRAVITY && casePlayer->getFieldStatus()[i].second!=0){
						if(attack.getRange().first==0 && attack.getRange().second==0 && !skill_special){
							skill_damage=skill_damage*1.5;
							cout<<"La gravitÃ© augemente les dÃ©gats de l'attaque au corps Ã  corps (attaquant)."<<endl;
						}
					}
				}
				
				//terrain ciblÃ©
				//status
				for(size_t i=0; i<caseCible->getFieldStatus().size();i++){
					//Brume
					if (caseCible->getFieldStatus()[i].first==MIST && caseCible->getFieldStatus()[i].second!=0){
						skill_precision=skill_precision*0.75;
						cout<<"La brume diminiue la precision de l'attaque (case ciblÃ©)."<<endl;
					}
				}
				//type
				if (caseCible->getFieldType()==MOUNTAIN){
					if(attack.getRange().first==0 && attack.getRange().second==0&& !skill_special){
						skill_precision=skill_precision*0.85;
						cout<<"La montagne baisse la prÃ©cision de l'attaque au corps Ã  corps (case ciblÃ©)."<<endl;
					}
				}else if (caseCible->getFieldType()==FOREST){
					if((attack.getRange().first==0 || attack.getRange().second==0)){
						skill_precision=skill_precision*0.85;
						cout<<"La foret baisse la prÃ©cision de l'attaque Ã  distance (case ciblÃ©)."<<endl;
					}
				}
				
				//attaquÃ© (terrain de l'attaquÃ© XOR terrain ciblÃ©)
				//todo modifier pour avoir une precision par perso
				for(size_t p=0;p<targetPlayer.size();p++){
					caseTarget = state.getGrid()[targetPlayer[p]->getY()][targetPlayer[p]->getX()];
					if (caseTarget!=caseCible){
						for(size_t i=0; i<caseTarget->getFieldStatus().size();i++){
							//Brume
							if (caseTarget->getFieldStatus()[i].first==MIST && caseTarget->getFieldStatus()[i].second!=0){
								target_Dodge[p]=target_Dodge[p]*0.75;
								cout<<"La brume diminiue la precision de l'attaque sur l'attaquÃ© nÂ°"<<p<<"."<<endl;
							}
						}
						if (caseTarget->getFieldType()==MOUNTAIN){
							if(attack.getRange().first==0 && attack.getRange().second==0&& !skill_special){
								target_Dodge[p]=target_Dodge[p]*0.85;
								cout<<"La montagne baisse la prÃ©cision de l'attaque au corps Ã  corps sur l'attaquÃ© nÂ°"<<p<<"."<<endl;
							}
						}else if (caseTarget->getFieldType()==FOREST){
							if((attack.getRange().first==0 || attack.getRange().second==0)){
								target_Dodge[p]=target_Dodge[p]*0.85;
								cout<<"La foret baisse la prÃ©cision de l'attaque Ã  distance sur l'attaquÃ© nÂ°"<<p<<"."<<endl;
							}
						}
					}
				}
				
				//attaque Ã  distance (terrain entre le terrain ciblÃ© et l'attaquant)
				//uniquement pris en compte le centre de l'attaque si eloigne de plus de 2 case
				if(abs(posTarget.first-X)>1 || abs(posTarget.second-Y)>1){
					if(direction_skill==NORTH){
						for(int k=1;k<abs(posTarget.second-Y)-1;k++){
							caseInter = state.getGrid()[Y-k][X];
							for(size_t i=0; i<caseInter->getFieldStatus().size();i++){
								//Brume
								if (caseInter->getFieldStatus()[i].first==MIST && caseInter->getFieldStatus()[i].second!=0){
									skill_precision=skill_precision*0.75;
									cout<<"La brume diminiue la precision de l'attaque Ã  distance (inter)."<<endl;
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
									cout<<"La brume diminiue la precision de l'attaque Ã  distance (inter)."<<endl;
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
									cout<<"La brume diminiue la precision de l'attaque Ã  distance (inter)."<<endl;
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
									cout<<"La brume diminiue la precision de l'attaque Ã  distance (inter)."<<endl;
								}
							}
						}
					}
							
				}
				//-----------------avantage Ã©tat-------------------------------------------
				for(size_t i=0; i<attacker->getStatus().size();i++){
					if (attacker->getStatus()[i].first==DAZZLED && attacker->getStatus()[i].second!=0){
						skill_precision=skill_precision*0.75;
						cout<<"L'attacker est Ã©bloui, la prÃ©cision de l'attaque est diminuÃ©."<<endl;
					}
				}

				//----------------avantage effet attaque-----------------------------------
				for(size_t i=0; i<effect.size();i++){
					if(get<0>(effect[i])==BOOST_ATK_RAIN){
						//augemente les dÃ©gat si il pleut
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
					cout << "Joueur " << attacker << " attaque ";
					for(size_t i=0; i<targetPlayer.size()-1;i++){
						cout<<"joueur "<< targetPlayer[i] << " , " ;
					}
					cout<<"joueur "<<targetPlayer[targetPlayer.size()-1]<<"."<< endl;
				}else{
					cout<<"Joueur " << attacker << " attaque case vide ("<<posTarget.first<<","<<posTarget.second<<")."<<endl;
				}
				//change l'alÃ©atoire
				srand(time(NULL));
				int chanceEsquive=rand()%100 + 1;
				
				//tester case central(si reussite=> effet zone terrain et perso cntral direct, effet zone perso =>refaire test
				//------------------------echec de l'attaque-------------------------------
				if(chanceEsquive<=skill_precision){
					cout << "Attaque ratÃ© "<< endl;
				
				//------------------------succes de l'attaque-------------------------------
				}else{
					int chanceEffet;
				//------------------------effet sur le terrain-------------------------------
					for(size_t i=0; i<effect.size();i++){
						if(get<0>(effect[i])==BURN_FIELD){
							//fait passer l'Ã©tat du terrain Ã  BURNING
							srand(time(NULL));
							chanceEffet=rand()%100 + 1;
							if(chanceEffet<=get<2>(effect[i])){
								cout << "Effet BURN_FIELD ratÃ© "<< endl;
							}else{
								cout << "Terrain(s) incendiÃ©"<< endl;
								bool test_burn=true;
								for(size_t f=0;f<v_caseField.size();f++){
									for(size_t s=0; s<v_caseField[f]->getFieldStatus().size();s++){
										if (v_caseField[f]->getFieldStatus()[s].first==BURNING && v_caseField[f]->getFieldStatus()[s].second>get<1>(effect[i])){
											test_burn=false;
											break;
										}
									}
									if(test_burn){
										//v_caseField[f]->updateStatus(BURNING,get<1>(effect[i]));
										cout<<"La case "<<v_caseField[f]->getFieldType()<<" brulera pendant "<<get<1>(effect[i])<<" tours"<<endl;
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
							srand(time(NULL));
							chanceEsquive=rand()%100 + 1;
							int esquive= skill_precision*target_Dodge[p];
							if(chanceEsquive<=esquive){
								touche=false;
							}
						}
				//------------------------echec de l'attaque-------------------------------
						if(!touche){
							cout << "Joueur "<<targetPlayer[p]<<" esquive l'attaque"<< endl;
				//------------------------succes de l'attaque-------------------------------
						}else{
				//---------------------------Degat subis-----------------------------------
							targetPlayer[p]->setHp(target_HP[p]-degats);
							cout << "Joueur " << targetPlayer[p] << " perd " << degats << " PV. " << endl;
							cout << "Il ne lui reste plus que " << targetPlayer[p]->getHp() << " PV."<< endl;
						
				//--------------------------verif mort------------------------------------
							if(targetPlayer[p]->getHp()==0){
								//cible.setStatut(MORT);
								targetPlayer[p]->setX(-1);
								targetPlayer[p]->setY(-1);
								cout << "Joueur " << targetPlayer[p] <<  " est mort. ++" << endl;
							}else{
				//--------------------------test effet------------------------------------
								for(size_t i=0; i<effect.size();i++){
									//POISON
									if(get<0>(effect[i])==POISON_CHAR){
										//fait passer l'Ã©tat du perso Ã  POISON
										srand(time(NULL));
										chanceEffet=rand()%100 + 1;
										if(chanceEsquive<=get<2>(effect[i])){
											cout << "Effet POISON ratÃ© "<< endl;
										}else{
											cout << "Joueur empoisonnÃ©"<< endl;
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
												cout<<"Le joueur "<<targetPlayer[p]<<" sera empoisonnÃ© pendant "<<get<1>(effect[i])<<" tours"<<endl;
											}
										}
									}
									//HEAL
									if(get<0>(effect[i])==HEAL_MEDIUM){
										//soigne les PV
										srand(time(NULL));
										chanceEffet=rand()%100 + 1;
										if(chanceEsquive<=get<2>(effect[i])){
											cout << "Effet HEAL(M) ratÃ© "<< endl;
										}else{
											cout << "Soin medium"<< endl;
											int heal = 0.1*target_HP[p];
											targetPlayer[p]->setHp(target_HP[p]+heal);
											cout<<"Le joueur "<<targetPlayer[p]<<" est soignÃ© de "<<heal<<" PV"<<endl;
										}
									}
									//MOVE FOE
									if(get<0>(effect[i])==MOVE_USER){
										//deplace la cible
										cout << "Le joueur est bousculÃ© vers une autre case"<< endl;
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
										srand(time(NULL));
										horizontal=rand()%1;
										int mf;
										srand(time(NULL));
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
		
			}
		// Cas attaque impossible
		}else{
			cout << "Attaque non autorisÃ©e !" << endl;		
		}
		
	}else if(attacker->getSkillCount()==0){
		cout << "le joueur nÂ°" << attacker << " a utilisÃ© tout ses PA, il ne peut plus attaquer." <<endl;  
	}
	
	cout << "\n" ;
}

std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> Attack::getTarget(state::State& state){
	//prend la position central de l'attaque et renvoie la zone d'effet de l'attaque et la liste des persos touchÃ©
		
		std::vector<Field*> v_caseField;
		std::vector<std::pair<int,int>> v_posField;
		std::vector<Player*> players =state.getPlayers();
		std::vector<state::Player*> targets;
		int sizePlayers = players.size();
		int pos_x;
		int pos_y;
		DirectionId direction_skill = checkDirection(state).second;
		bool is_blocked;
		
		//verifier si il n'ya pas de blockattck et si attaque possible
		for(size_t i=0;i<attack.getArea().size();i++){
			is_blocked=false;
			if(direction_skill==NORTH){
				pos_y=posTarget.second+attack.getArea()[i].second;
				pos_x=posTarget.first+attack.getArea()[i].first;
			}else if(direction_skill==EAST){
				pos_y=posTarget.second+attack.getArea()[i].first;
				pos_x=posTarget.first-attack.getArea()[i].second;
			}else if(direction_skill==SOUTH){
				pos_y=posTarget.second-attack.getArea()[i].second;
				pos_x=posTarget.first-attack.getArea()[i].first;
			}else if(direction_skill==WEST){
				pos_y=posTarget.second-attack.getArea()[i].first;
				pos_x=posTarget.first+attack.getArea()[i].second;
			}
			v_caseField.push_back(state.getGrid()[pos_y][pos_x]);
			std::vector<std::pair<state::FieldStatusId,int>> statusCase = v_caseField[i]->getFieldStatus();
			for(size_t c=0; c<statusCase.size();c++){
				if (statusCase[c].first==BLOCKATTACK && statusCase[c].second!=0){
					is_blocked=true;
				}
			}
			if(!is_blocked){
				v_posField.push_back({pos_x,pos_y});
				for(int p; p<sizePlayers;p++){
					if(players[p]->getX()==pos_x && players[p]->getY()==pos_y ){
						targets.push_back(players[p]);
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
	
	bool attaque_possible;
	int X = attacker->getX();
	int Y = attacker->getY();
	DirectionId direction_skill;
	if((Y-attack.getRange().first)>posTarget.second &&(Y-attack.getRange().second)<posTarget.second && X==posTarget.first){
		for(int j = attack.getRange().first; j<attack.getRange().second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y-j][X]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "la direction NORD est bloquÃ©" <<endl;
					break;
				}	
				if(Y+j==posTarget.second && i==statusCase.size()){
					cout << "la direction NORD n'est pas bloquÃ©" <<endl;
					direction_skill=NORTH;
					attaque_possible=true;
				}
			}
		}
	}else if((Y+attack.getRange().first)<posTarget.second && (Y+attack.getRange().second)>posTarget.second && X==posTarget.first){
		for(int j = attack.getRange().first; j<attack.getRange().second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y+j][X]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "la direction SUD est bloquÃ©" <<endl;
					break;
				}	
				if(Y+j==posTarget.second && i==statusCase.size()){
					cout << "la direction SUD n'est pas bloquÃ©" <<endl;
					direction_skill=SOUTH;
					attaque_possible=true;
				}
			}
		}
	}else if((X-attack.getRange().first)>posTarget.first && (X-attack.getRange().second)<posTarget.first && Y==posTarget.second){
		for(int j = attack.getRange().first; j<attack.getRange().second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y][X-j]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "la direction OUEST est bloquÃ©" <<endl;
					break;
				}	
				if(X-j==posTarget.first && i==statusCase.size()){
					cout << "la direction OUEST n'est pas bloquÃ©" <<endl;
					direction_skill=WEST;
					attaque_possible=true;
				}
			}
		}
	}else if((X+attack.getRange().first)<posTarget.first && (X+attack.getRange().second)>posTarget.first && Y==posTarget.second){
		for(int j = attack.getRange().first; j<attack.getRange().second +1; j++){
			std::vector<std::pair<FieldStatusId, int>> statusCase = state.getGrid()[Y][X+j]->getFieldStatus();
			for(size_t i=0; i<statusCase.size();i++){
				if (statusCase[i].first==BLOCKATTACK && statusCase[i].second!=0){
					cout << "la direction EST est bloquÃ©" <<endl;
					break;
				}	
				if(X+j==posTarget.first && i==statusCase.size()){
					cout << "la direction EST n'est pas bloquÃ©" <<endl;
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
