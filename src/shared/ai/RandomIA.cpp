#include "ai.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;


int RandomIA::run (engine::Engine& engine, sf::RenderWindow& window){
	// L'IA effectue ces actions uniquement si c'est son tour
	State state = engine.getState();
	if(state.getPlaying()->getIa()==true){
		int randomAction;
		std::pair<int,int> randomPosition;
		int randomAttaque;
		bool premierEssai = true;
		bool actionPossible=true;
		bool attaquePossible=true;
		Player* bot =state.getPlaying();
		int randomDirection;
		int X, Y, range;
		std::vector<int> attaquesValides;
		srand(time(NULL));
			
			
		while ((bot->getHp() > 0) && actionPossible){
			clock_t start_time = clock();
			while(clock()<start_time+1000000);
			//sleep(1);
			if(premierEssai == true){
				cout<< "\t [Controle par le CPU] " << endl;
				premierEssai = false;
			}
			
			/*
			 * randomAction: 0 = deplacemnt
			 * 				 1 = attaque
			 * 				 2 = fin de tour
			 * */
			 //to do augementer proba d'action en fct des pm pa restant
			if (bot->getMovement()==0 && (bot->getSkillCount()==0 || !attaquePossible)){
				randomAction = 2;
			}
			else if (bot->getMovement()==0 && bot->getSkillCount()>0 && attaquePossible){
				randomAction = rand()%2+1 ;
			}
			else if (bot->getMovement()>0 && (bot->getSkillCount()==0 || !attaquePossible)){
				randomAction = rand()%2;
				if(randomAction == 1){
					randomAction = 2;
				}
			}
			else{randomAction = rand()%3;}
			//cout<<"Action selectionné :";
												
			// 0 : Cas du deplacement
			if (randomAction == 0){
				//cout<<" déplacement"<<endl;
				randomDirection=rand()%4;
				if(randomDirection==0){
					randomPosition = {bot->getX(),bot->getY()-1};
				}else 
				if(randomDirection==1){
					randomPosition = {bot->getX()+1,bot->getY()};
				}else 
				if(randomDirection==2){
					randomPosition = {bot->getX(),bot->getY()+1};
				}else 
				if(randomDirection==3){
					randomPosition = {bot->getX()-1,bot->getY()};
				}
				Move* move = new Move(randomPosition);
				engine.executeCommand(move, window);
				
			}
			// 1 : Cas de l'attaque
			else if (randomAction == 1){
				//cout<<" attaque"<<endl;
				std::vector<Skill*> listeAttaques = bot-> getSkills();
				
				for(size_t a=0;a<listeAttaques.size();a++){
					if(listeAttaques[a]->getCooldown()<=0){
						attaquesValides.push_back(a);
					}
				}
				if (attaquesValides.size() !=0){
					//cout<<"attaques valides"<<endl;
					
					//choix aléatoire de l'attaque parmi les attaques possibles
					randomAttaque = attaquesValides[rand()%attaquesValides.size()];
					//cout<<"attaque selectionné :"<<listeAttaques[randomAttaque]->getName()<<endl;
							
					
					//choix de la direction aléatoire
					randomDirection=rand()%4;
					if(randomDirection==0){
						X=0;
						Y=-1;
						//cout<<"Direction : Nord"<<endl;
					}else 
					if(randomDirection==1){
						X=1;
						Y=0;
						//cout<<"Direction : Est"<<endl;
					}else 
					if(randomDirection==2){
						X=0;
						Y=1;
						//cout<<"Direction : Sud"<<endl;
					}else 
					if(randomDirection==3){
						X=-1;
						Y=0;
						//cout<<"Direction : Ouest"<<endl;
					}
					//choix de la case attaqué (range) aléatoire
					range=rand()%(listeAttaques[randomAttaque]->getRange().second -listeAttaques[randomAttaque]->getRange().first+1)+listeAttaques[randomAttaque]->getRange().first;
					
					randomPosition={bot->getX()+X*range,bot->getY()+Y*range};
					
					// Commande d'attaque
					
					Attack* attack = new Attack(randomPosition , randomAttaque);
					engine.executeCommand(attack, window);
					
					
				}else{
					cout<<"Le CPU n'a pas d'attaque possible"<<endl;
					attaquePossible=false;
				}		
			}
			
			// 2 : Cas de fin d'actions
			else if (randomAction == 2){
				//cout<<" fin de tour"<<endl;
				EndActions* endactions = new EndActions();
				engine.executeCommand(endactions, window);
				actionPossible=false;
				return 0;
			}
		}	
		//test verification hors boucle

		if(actionPossible == false){
			cout<< "\t\t-> Action impossible pour le  CPU " << bot->getName() << " <-" << endl;
		}	
	}
	return 0;
}
