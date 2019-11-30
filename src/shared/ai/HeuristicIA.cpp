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


int HeuristicIA::run (engine::Engine& engine, sf::RenderWindow& window){
	cout<<"----------------------------------------------"<<endl;
	cout<<"fonction HeuristicIA::run : début d'execution"<<endl;
	State state = engine.getState();
	Player* player = state.getPlaying();
	//std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> listPosSkill = getPosSkill(player,state);
	//std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> listPosAttack = getPosAttack(player,state);
	//cout<<"fonction HeuristicIA::run : appel getPosMove"<<endl;
	std::vector<std::pair<int,int>> listPosMove = getPosMove(player, state);
	if(player->getIa()==true){
		std::pair<int,int> aimed_position;
		bool premierEssai = true;
		srand(time(NULL));
		
		bool gotheal=false;
		int idheal;
		int id_skill;
		bool gotrange=false;
		bool gotattack=false;
		bool gotrangedirect=false;
		std::vector<int> idattack;
		std::vector<int> idskill;
		std::vector<int> idattackrange;
		std::vector<Player*> targets;
		std::vector<int> idattackrangedirect;
		std::vector<Player*> targetsdirect;
		
		//todo : fonction determinant ennemie le plus proche et cible principal
		Player* closest_foe;
		//Player* main_target;
		/*while (player->getHp() > 0){
			sleep(1);*/
			
			
		
		bool move_done;
		bool move_from_danger=true;
		bool move_to_foe=true;
		bool move_run_away=true;
		std::vector<state::Player*> players = state.getPlayers();
		//ensemble des attaques à dégat
		for(size_t s=0;s<player->getSkills().size();s++){
			Skill* skill = player->getSkills()[s];
			if(skill->getDamage()>0){
				idskill.push_back(s);
			}
		}
		/*
		if(player->getSkillCount()>0){
			//test : attaque de soin dispo
			for(size_t s=0;s<player->getSkills().size();s++){
				Skill* skill = player->getSkills()[s];
				for(size_t e=0; e<skill->getEffect().size();e++){
					if(skill->getCooldown()==0 && (get<0>(skill->getEffect()[e])==HEAL_LOW || get<0>(skill->getEffect()[e])==HEAL_MEDIUM || get<0>(skill->getEffect()[e])==HEAL_HIGH ) && skill->getRange().first==0){
						gotheal=true;
						//voir plus tard si tjr soin le plus fort dispo d'abord
						idheal=s;
					}
				}
			}
				
			//test : attaque de dégat dispo
			for(size_t s=0;s<player->getSkills().size();s++){
				Skill* skill = player->getSkills()[s];
				if(skill->getCooldown()==0 && skill->getDamage()>0){
					gotattack=true;
					//voir plus tard si tjr soin le plus fort dispo d'abord
					idattack.push_back(s);
					cout<<"attaque n"<<s<<" ajoute"<<endl;
				}
			}
					
				
			//test : attaque de degat a porté
			//if(listPosAttack.first.size()!=0){
				for(size_t a=0;a<player->getSkills().size();a++){
					Skill* skill = player->getSkills()[a];
	cout<<"fonction HeuristicIA::getTargetAccessible : appelé par run(test range boucle "<<a<<")"<<endl;
					std::vector<Player*> gta=getTargetAccessible(state,player,a).second;
					if(gta.size()!=0 && skill->getCooldown()==0 && skill->getDamage()>0){
						targets.insert(targets.end(),gta.begin(), gta.end());
						idattackrange.push_back(a);
						gotrange=true;
					}
				}
			//}
			
			//test : attaque de degat à porté direct	
			//if(listPosAttack.first.size()!=0){
				//for(size_t a=0;a<idattack.size();a++){ a remplace idattack[a]
				for(size_t a=0;a<player->getSkills().size();a++){
					Skill* skill = player->getSkills()[a];
					cout<<"fonction HeuristicIA::getTargetDirect : appelé par run(test direct)"<<endl;
					std::vector<Player*> gtd=getTargetDirect(state,player,a).second;
					if(gtd.size()!=0 && skill->getCooldown()==0 && skill->getDamage()>0){
						targetsdirect.insert(targetsdirect.end(),gtd.begin(), gtd.end());
						idattackrangedirect.push_back(a);
						gotrangedirect=true;
					}
				}
			//}
		}
		bool dangerousfield=false;
		std::vector<std::pair<FieldStatusId,int>> player_field_status=player_field->getFieldStatus();
		//test : terrain dangereux
		if(player->getMovement()>0 && (player_field_status[12].second>0 || player_field_status[13].second>0)){
			dangerousfield=true;
		}
		
		//definition closest_foe
		closest_foe=players[0];
		int dist_min=9999;
		int dist_cf =(player->getX()-players[0]->getX())*(player->getX()-players[0]->getX())+(player->getY()-players[0]->getY())*(player->getY()-players[0]->getY());
		for(size_t cf=0; cf<players.size();cf++){
			if(dist_min>dist_cf){
				closest_foe=players[cf];
				dist_min=dist_cf;
			}
		}*/
			
		while (player->getHp() > 0){
			sleep(1);
			std::pair<int,int> player_position={player->getX(),player->getY()};
			std::vector<std::vector<Field*>> grid =state.getGrid();
			Field* player_field = grid[player_position.second][player_position.first];
			if(premierEssai == true){
				cout<< "\t [Controle par le CPU heuristique] " << endl;
				premierEssai = false;
			}
			gotrangedirect = false;
			gotrange = false;
			gotattack = false;
			gotheal = false;
			if(player->getSkillCount()>0){
				//test : attaque de soin dispo
				for(size_t s=0;s<player->getSkills().size();s++){
					Skill* skill = player->getSkills()[s];
					for(size_t e=0; e<skill->getEffect().size();e++){
						if(skill->getCooldown()==0 && (get<0>(skill->getEffect()[e])==HEAL_LOW || get<0>(skill->getEffect()[e])==HEAL_MEDIUM || get<0>(skill->getEffect()[e])==HEAL_HIGH ) && skill->getRange().first==0){
							gotheal=true;
							//voir plus tard si tjr soin le plus fort dispo d'abord
							idheal=s;
						}
					}
				}
					
				//test : attaque de dégat dispo
				for(size_t s=0;s<player->getSkills().size();s++){
					Skill* skill = player->getSkills()[s];
					if(skill->getCooldown()==0 && skill->getDamage()>0){
						gotattack=true;
						//voir plus tard si tjr soin le plus fort dispo d'abord
						idattack.push_back(s);
						cout<<"attaque n"<<s<<" ajoute"<<endl;
					}
				}
						
					
				//test : attaque de degat a porté
				for(size_t a=0;a<player->getSkills().size();a++){
					Skill* skill = player->getSkills()[a];
					cout<<"fonction HeuristicIA::getTargetAccessible : appelé par run(test range boucle "<<a<<")"<<endl;
					std::vector<Player*> gta=getTargetAccessible(state,player,a).second;
					if(gta.size()!=0 && skill->getCooldown()==0 && skill->getDamage()>0){
						targets.insert(targets.end(),gta.begin(), gta.end());
						idattackrange.push_back(a);
						gotrange=true;
					}
				}
				
				//test : attaque de degat à porté direct	
				//for(size_t a=0;a<idattack.size();a++){ a remplace idattack[a]
				for(size_t a=0;a<player->getSkills().size();a++){
					Skill* skill = player->getSkills()[a];
					cout<<"fonction HeuristicIA::getTargetDirect : appelé par run(test direct)"<<endl;
					std::vector<Player*> gtd=getTargetDirect(state,player,a).second;
					if(gtd.size()!=0 && skill->getCooldown()==0 && skill->getDamage()>0){
						targetsdirect.insert(targetsdirect.end(),gtd.begin(), gtd.end());
						idattackrangedirect.push_back(a);
						gotrangedirect=true;
					}
				}
			}
			bool dangerousfield=false;
			std::vector<std::pair<FieldStatusId,int>> player_field_status=player_field->getFieldStatus();
			//test : terrain dangereux
			if(player->getMovement()>0 && (player_field_status[12].second>0 || player_field_status[13].second>0)){
				dangerousfield=true;
			}
		
			//definition closest_foe
			closest_foe=players[0];
			int dist_min=9999;
			int dist_cf =(player->getX()-players[0]->getX())*(player->getX()-players[0]->getX())+(player->getY()-players[0]->getY())*(player->getY()-players[0]->getY());
			for(size_t cf=0; cf<players.size();cf++){
				if(dist_min>dist_cf){
					closest_foe=players[cf];
					dist_min=dist_cf;
				}
			}
			
			//si peu de pv et sort de soin dispo
			if(player->getHp()<player->getCharacter()->getHpBase()*0.2 and gotheal){
				cout<<"soin urgent"<<endl;
					Attack* attack = new Attack(player_position , idheal);
					engine.executeCommand(attack, window);
				//si aucune attaque degat(cooldown porté general) et terrain dangereux, fuite vers case safe (si existe)
			}else if((!gotattack || !gotrange) && dangerousfield && move_from_danger){
				cout<<"terrain dangereux"<<endl;
				for(size_t c = 0; c<listPosMove.size();c++){
					std::vector<std::pair<FieldStatusId, int>> field_status=grid[listPosMove[c].second][listPosMove[c].first]->getFieldStatus();
					if(field_status[12].second>0 || field_status[13].second>0){
						aimed_position=listPosMove[c];
						cout<<"randomWay terrain dangeureux"<<endl;
						move_from_danger=randomWay(aimed_position, state, player, engine, window);
						break;
					}
				} 
				//si aucune attaque degat(cooldown) et moins de vie que l'ennemi le proche,fuite a lopposé
			}else if(!gotattack && closest_foe->getHp()>player->getHp() && player->getMovement()>0 && move_run_away){
				cout<<"ennemie proche"<<endl;
				aimed_position.first=2*player_position.first - closest_foe->getX();
				aimed_position.second=2*player_position.second - closest_foe->getY();
				cout<<"randomWay fuite"<<endl;
				move_run_away=randomWay(aimed_position, state, player, engine, window);
				cout<<"move possible : "<<move_run_away<<endl;
				//si ennemi a porté direct, attaque (au meilleur socre si plusieur)
			}else if(gotrangedirect){
				cout<<"attaque direct"<<endl;
				std::tuple<Player*,int,std::pair<int,int>> gmt = getMainTarget(state, targetsdirect,idattackrangedirect);
				id_skill = get<1>(gmt);
				aimed_position = get<2>(gmt);
				//std::make_tuple(main_target,id_skill,aimed_position) = getMainTarget(state, targetsdirect,idattackrangedirect); //getMainTarget=> choissi la cible qui finira avecles pv les plus bas avec l'attaque la mieux adapté (aussi en fct de precision
				Attack* attack = new Attack(aimed_position , id_skill);
				cout<<"skill lancé "<<id_skill<<endl;
				engine.executeCommand(attack, window);
				//sinon, avancé vers ennemi (le plus faible si pas de porté, au meilleur socre sinon)
			}else if(!gotrange && player->getMovement()>0 && move_to_foe){
				cout<<"rapproche ennemi hors porté"<<endl;
				//std::make_tuple(main_target,id_skill,aimed_position) = getMainTarget(state, players,idskill);
				std::tuple<Player*,int,std::pair<int,int>> gmt = getMainTarget(state, players,idskill);
				id_skill = get<1>(gmt);
				aimed_position = get<2>(gmt);
				cout<<"randomWay vers ennemi"<<endl;
				move_to_foe=randomWay(aimed_position, state, player, engine, window);
				//si ennemie(s) à porté general, score pour décidé qui attaquer avec quel attaque puis deplacement vers case pour avoir ennemi a porté directe, si reussi, attaque, sinon retour boucle
			}else if(gotrange && player->getMovement()>0){
				cout<<"deplacement puis attaque"<<endl;
				//std::make_tuple(main_target,id_skill,aimed_position) = getMainTarget(state, targets,idattackrange);
				std::tuple<Player*,int,std::pair<int,int>> gmt = getMainTarget(state, targets,idattackrange);
				id_skill = get<1>(gmt);
				aimed_position = get<2>(gmt);
				cout<<"randomWay attaque"<<endl;
				move_done=randomWay(aimed_position, state, player, engine, window);
				if(move_done){
					Attack* attack = new Attack(aimed_position , id_skill);
					engine.executeCommand(attack, window);
				}
				
				//sinon si attaque de soin et pv<0.9*pvbase, soin
			}else if(player->getHp()<player->getCharacter()->getHpBase()*0.9 and gotheal){
				cout<<"soin"<<endl;
				Attack* attack = new Attack(player_position ,idheal);
				engine.executeCommand(attack, window);
			}else{
				cout<<"fin action"<<endl;
				//si a porté ennemi et pas attaque ou plus pm et pa, fin de tour
				EndActions* endactions = new EndActions();
				engine.executeCommand(endactions, window);
				cout<<"fonction HeuristicIA::run : fin d'execution"<<endl;
	cout<<"----------------------------------------------"<<endl;
				return 0;
			}
		}
		cout<<"fonction HeuristicIA::run : fin d'execution(erreur)"<<endl;
	cout<<"----------------------------------------------"<<endl;
		return 0;
	}
	cout<<"fonction HeuristicIA::run : fin d'execution(erreur n#2)"<<endl;
	cout<<"----------------------------------------------"<<endl;
	return 0;
}

bool HeuristicIA::randomWay(std::pair<int,int> destination, state::State state, state::Player* player, engine::Engine& engine, sf::RenderWindow& window){
	//cout<<"fonction HeuristicIA::randomWay : début d'execution"<<endl;
	int H, V, X, Xf, Xp, Y, Yf, Yp, ax,ay,bx,by;
	std::pair<int,int> position;
	X=player->getX();
	Y=player->getY();
	Xf=destination.first;
	Yf=destination.second;
	H=Xf-X;
	V=Yf-Y;
	bool move_possible=true;
	int direction;
	if(H==0 && V==0){
		move_possible=false;
	}
	while(move_possible && !(H==0 && V==0)){
		direction =rand()%2;
		if(V<0){
			by=-1;
			bx=0;
		}else if(V>0){
			by=1;
			bx=0;
		}
		if(H<0){
			ax=-1;
			ay=0;
		}else if(H>0){
			ay=0;
			ax=1;
		}
		if(V==0){
			ay=0;
			by=0;
			if(H<0){
				ax=-1;
				bx=-1;
			}else{
				ax=1;
				bx=1;
			}
		}else if(H==0){
			ax=0;
			bx=0;
			if(V<0){
				ay=-1;
				by=-1;
			}else{
				ay=1;
				by=1;
			}
		}
		
		if(direction==0){
			Xp=X+ax;
			Yp=Y+ay;
		}else{
			Xp=X+bx;
			Yp=Y+by;
		}
		position={Xp,Yp};
		
		Move* move = new Move(position);
		engine.executeCommand(move, window);
		if(player->getX()==position.first && player->getY()==position.second){
			X=player->getX();
			Y=player->getY();
			H=Xf-X;
			V=Yf-Y;
		}else{
			move_possible=false;
		}
	}
	//cout<<"fonction HeuristicIA::randomWay : fin d'execution"<<endl;
	return move_possible;
}

std::tuple<state::Player*,int,std::pair<int,int>> HeuristicIA::getMainTarget(state::State state,std::vector<state::Player*> players,std::vector<int> skills_id){
	cout<<"fonction HeuristicIA::getMainTarget : début d'execution"<<endl;
	cout<<"size skills id "<<skills_id.size()<<endl;
	  //attention postion en sortie=aimed_position!=position target sortie (si attaque en zone)
	  std::vector<std::vector<int>>  score; //fct de d_s, p_s, hp_t, dist et nb_t vpt etat target
	  std::vector<int>  damage_skill;
	  //std::vector<std::vector<int>>  precision_skill; //precision par attaque par personnage ciblé
	  //std::vector<int>  num_targets;//nombre de personnage touché par attaque
	  std::vector<int> distance;
	  int id_skill;
	  Player* main_target;
	  std::vector<std::vector<Player*>> target;
	  bool direct_attack=false;
	  bool range_attack=false;
	  std::pair<int,int> aimed;
	  int hp;
	  int dist;
	  Player* player = state.getPlaying();
	//test attaque direct et deplacement
	cout<<"fonction HeuristicIA::getMainTarget : debut boucle skills"<<endl;
	for(size_t s=0; s<skills_id.size(); s++){
		cout<<"fonction HeuristicIA::getMainTarget : boucle skills n#"<<s<<endl;
		cout<<"fonction HeuristicIA::getMainTarget : debut boucle player"<<endl;
		for(size_t t=0; t<players.size();t++){
			cout<<"fonction HeuristicIA::getMainTarget : boucle player"<<t<<endl;
			cout<<"fonction HeuristicIA::getTargetDirect : appelé par getMainTarget"<<endl;
			std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> direct = getTargetDirect(state, state.getPlaying(), skills_id[s]);
	cout<<"fonction HeuristicIA::getTargetAccessible : appelé par getMainTarget"<<endl;
			std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> range = getTargetAccessible(state, state.getPlaying(), skills_id[s]);
			for(size_t d=0;d<direct.second.size();d++){
				if(direct.second[d]==players[t]){
					direct_attack=true;
				}
			}
			for(size_t d=0;d<range.second.size();d++){
				if(range.second[d]==players[t] && !direct_attack){
					range_attack=true;
				}else if(direct_attack){
					range_attack=false;//priorité de l'attaque directe
				}
			}
		}
		cout<<"fonction HeuristicIA::getMainTarget : fin boucle player"<<endl;
	}
	cout<<"fonction HeuristicIA::getMainTarget : fin boucle skills"<<endl;
	//calcul des scores
	cout<<"fonction HeuristicIA::getMainTarget : calcul de score: debut boucle skills"<<endl;
	for(size_t s=0; s<skills_id.size(); s++){
		Skill* skill=player->getSkills()[skills_id[s]];
		damage_skill.push_back(skill->getDamage());
		std::vector<int> inter;
		std::vector<Player*> inter_p;
		cout<<"fonction HeuristicIA::getMainTarget : calcul de score: boucle n#"<<s<<endl;
		cout<<"fonction HeuristicIA::getMainTarget : calcul de score: debut boucle player"<<endl;
		for(size_t t=0; t<players.size();t++){
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: boucle player n#"<<t<<endl;
			//si ennemi a porté direct, attaque
			if(direct_attack){
				//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: attaque direct "<<endl;
				std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> direct = getTargetDirect(state, state.getPlaying(), skills_id[s]);
				for(size_t d=0;d<direct.second.size();d++){
					if(direct.second[d]==players[t]){
						//vpt:calcul precision
						hp=direct.second[d]->getHp();
						inter_p.push_back(players[t]);
						inter.push_back(hp-skill->getDamage());//(hp-skill->getDamage())*num_target/precision vpt change en float
					}
				}
			//si ennemi a porté, déplacement
			
			}else if(range_attack){
				cout<<"fonction HeuristicIA::getMainTarget : calcul de score: attaque distance "<<endl;
				cout<<"fonction HeuristicIA::getTargetAccessible : appelé par getMainTarget n#2"<<endl;
				std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> range = getTargetAccessible(state, state.getPlaying(), skills_id[s]);
				for(size_t d=0;d<range.second.size();d++){
					if(range.second[d]==players[t]){
						//vpt:calcul precision
						dist=(players[t]->getX()-player->getX())*(players[t]->getX()-player->getX())+(players[t]->getY()-player->getY())*(players[t]->getY()-player->getY());
						hp=range.second[d]->getHp();
						inter_p.push_back(players[t]);
						inter.push_back((hp-skill->getDamage())*dist);//(hp-skill->getDamage())*num_target*dist/precision vpt change en float 
					}
				}
			}else{
				//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: aucun ennemie à porte "<<endl;
				//si aucun ennemie a porté
				//vpt:calcul precision
				dist=(players[t]->getX()-player->getX())*(players[t]->getX()-player->getX())+(players[t]->getY()-player->getY())*(players[t]->getY()-player->getY());
				//cout<<"dist="<<dist<<endl;
				hp=players[t]->getHp();
				//cout<<"hp="<<hp<<endl;
				inter_p.push_back(players[t]);
				//cout<<"push back inter p"<<endl;
				inter.push_back((hp-skill->getDamage())*dist);//(hp-skill->getDamage())*num_target*dist/precision vpt change en float 
				//cout<<"push back inter"<<endl;
			}
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: fin calcul"<<endl;
		}
		cout<<"fonction HeuristicIA::getMainTarget : calcul de score: fin boucle player"<<endl;
		score.push_back(inter);
		target.push_back(inter_p);
	}
	cout<<"fonction HeuristicIA::getMainTarget : calcul de score: fin boucle skills"<<endl;
	//comparaison des scores
	int score_min = 999999;
	//cout<<"fonction HeuristicIA::getMainTarget : definition main_taget: debut boucle "<<endl;
	for(size_t s=0; s<skills_id.size(); s++){
		//if(direct_attack){
			for(size_t t=0; t<players.size();t++){
				if(score_min>score[s][t]){
					score_min=score[s][t];
					id_skill=skills_id[s];
					main_target=target[s][t];
				}
			}
		//}
	}
	//definition cible
	std::pair<int,int> main_skill_range=player->getSkills()[id_skill]->getRange();
	std::vector<std::pair<int,int>> main_skill_area=player->getSkills()[id_skill]->getArea();
	int x_area,y_area, x_range, y_range;
	cout<<"fonction HeuristicIA::getMainTarget : definition case ciblé:"<<endl;
	if(direct_attack){
		//verifier dans quel direction et range est l'attaque
		for(size_t d=0; d<4;d++){
			int r_init;
			if(main_skill_range.first==0){
				r_init=1;
			}else{
				r_init=main_skill_range.first;
			}
			for(int r=r_init;r<main_skill_range.second;r++){
				for(size_t a=0;a<main_skill_area.size();a++){
					if(d==0){
						x_area=main_skill_area[a].first;
						y_area=main_skill_area[a].second;
						x_range=0;
						y_range=-r;
					}else if(d==1){
						x_area=main_skill_area[a].first;
						y_area=-main_skill_area[a].second;
						x_range=0;
						y_range=r;
					}else if(d==2){
						x_area=main_skill_area[a].second;
						y_area=main_skill_area[a].first;
						x_range=r;
						y_range=0;
					}else if(d==3){
						x_area=-main_skill_area[a].second;
						y_area=main_skill_area[a].first;
						x_range=-r;
						y_range=0;
					}
					if(main_target->getX()==(player->getX()+x_area+x_range) && main_target->getY()==(player->getY()+y_area+y_range)){
						aimed={player->getX()+x_range,player->getY()+y_range};
						cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(directe)"<<endl;
						return std::make_tuple(main_target,id_skill,aimed);
					}
				}
			}
		}
	}else{
		//todo modifié avec getposaccessible
		int d_x_max,d_y_max, sens_x, sens_y;
		d_x_max=abs(player->getX()-main_target->getX());
		d_y_max=abs(player->getY()-main_target->getY());
		if((player->getX()-main_target->getX())>0){
			sens_x=-1;
		}else{
			sens_x=1;
		}
		if((player->getY()-main_target->getY())>0){
			sens_y=-1;
		}else{
			sens_y=1;
		}
		for(int d_x=0; d_x<d_x_max;d_x++){
			for(int d_y=0; d_y<d_y_max;d_y++){
				for(size_t d=0; d<4;d++){
					
					for(int r=main_skill_range.first;r<main_skill_range.second;r++){
						for(size_t a=0;a<main_skill_area.size();a++){
							if(d==0){
								x_area=main_skill_area[a].first;
								y_area=main_skill_area[a].second;
								x_range=0;
								y_range=-r;
							}else if(d==1){
								x_area=main_skill_area[a].first;
								y_area=-main_skill_area[a].second;
								x_range=0;
								y_range=r;
							}else if(d==2){
								x_area=main_skill_area[a].second;
								y_area=main_skill_area[a].first;
								x_range=r;
								y_range=0;
							}else if(d==3){
								x_area=-main_skill_area[a].second;
								y_area=main_skill_area[a].first;
								x_range=-r;
								y_range=0;
							}
							if(main_target->getX()==(player->getX()+x_area+x_range+d_x*sens_x) && main_target->getY()==(player->getY()+y_area+y_range+d_y*sens_y)){
								aimed={player->getX()+d_x*sens_x,player->getY()+d_y*sens_y};
								cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(ditance)"<<endl;
								return std::make_tuple(main_target,id_skill,aimed);
							}
						}
					}
				}
			}
		}
	}
	
	cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(aucun)"<<endl;
	return std::make_tuple(main_target,id_skill,aimed);
}

std::vector<std::pair<int,int>>  HeuristicIA::getPositionsAccessible(state::State state, state::Player* player, std::vector<std::pair<int,int>> lmpp, int PM){
	//cout<<"fonction HeuristicIA::getPositionsAccessible : début d'execution"<<endl;
	//renvoie l'ensemble des cases accessibles pour un joueur a partir d'une case donné pour un nombre de PM donné
	std::vector<std::pair<int,int>>  lmp=lmpp;
	std::vector<std::pair<int,int>> new_pos;
	std::pair<int,int> pos_next_case;
	Field* next_case;
	if(PM>0){
		for(int d=0;d<4;d++){
			//definition next_case
			if(d==0){
				pos_next_case.first=player->getX()+1;
				pos_next_case.second=player->getY();
			}else if(d==1){
				pos_next_case.first=player->getX()-1;
				pos_next_case.second=player->getY();
			}else if(d==2){
				pos_next_case.first=player->getX();
				pos_next_case.second=player->getY()-1;
			}else if(d==3){
				pos_next_case.first=player->getX();
				pos_next_case.second=player->getY()+1;
			}
			next_case = state.getGrid()[pos_next_case.second][pos_next_case.first];
			
			//ajout de cases
			//todo add f(PM) et presence autre joueur
			if(next_case->getFieldStatus()[11].second!=0){
				int PMused=1;
				bool toadd;
				new_pos=lmpp;
				new_pos.push_back(pos_next_case);
				//cout<<"fonction HeuristicIA::getPositionsAccessible : appelé par lui même avec "<<PM<<" PM restant"<<endl;
				new_pos = getPositionsAccessible(state,player,new_pos,PM-PMused);
				for(size_t i=0;i<new_pos.size();i++){
					toadd=true;
					for(size_t j=0; j<lmp.size();j++){
						if(new_pos[i]==lmp[j]){
							toadd=false;
						}
					}
					if(toadd){
						lmp.push_back(new_pos[i]);
					}
				}
			}
		}
	}
	//cout<<"fonction HeuristicIA::getPositionsAccessible : fin d'execution"<<endl;
	return lmp;
}


std::vector<std::pair<int,int>> HeuristicIA::getPosMove (state::Player* player, state::State state){
	//cout<<"fonction HeuristicIA::getPosMove : début d'execution"<<endl;
	//renvoie l'ensemble des cases accessibles par le joueur
	int X=player->getX();
	int Y=player->getY();
	std::vector<std::pair<int,int>> listPosMove={{X,Y}};
	//cout<<"fonction HeuristicIA::getPositionsAccessible : appelé par getPosMove"<<endl;
	listPosMove=getPositionsAccessible(state,player,listPosMove,player->getMovement());
	//cout<<"fonction HeuristicIA::getPosMove : fin d'execution"<<endl;
	return listPosMove;
}

std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> HeuristicIA::getTargetAccessible(state::State state, state::Player* player, int skill_num){
	cout<<"fonction HeuristicIA::getTargetAccessible : début d'execution"<<endl;
	//renvoie l'ensemble des cases attaquables (tout type) par le joueur pour une attaque donnée
	state::Skill* attack = player->getSkills()[skill_num];
	//cout<<"fonction HeuristicIA::getPosMove : appel par getTargetAccessible"<<endl;
	std::vector<std::pair<int,int>> listPosMove = getPosMove(player, state);
	std::vector<std::pair<int,int>> v_posField;
	std::vector<state::Player*> targets;
			int pos_x;
			int pos_y;
			bool is_blocked;
			std::vector<std::pair<state::FieldStatusId,int>> statusCase;
	cout<<"fonction HeuristicIA::getTargetAccessible : debut boucle case move possible"<<endl;
	
	//boucle pour chaque position
	for (size_t l=0; l<listPosMove.size();l++){
		cout<<"fonction HeuristicIA::getTargetAccessible : boucle case move possible n#"<<l<<endl;
		
		//ajoute tout les cible target case possible avec range
		std::vector<std::pair<int,int>> posTargets;
		std::pair<int,int> posTarget;
		//cout<<"fonction HeuristicIA::getTargetAccessible : debut boucle directions"<<endl;
		for(size_t d=0;d<4;d++){
			//cout<<"fonction HeuristicIA::getTargetAccessible : direction "<<d<<endl;
			int r_init;
			if(attack->getRange().first==0){
				r_init = 1;
				posTargets.push_back(listPosMove[l]);
			}else{
				r_init =attack->getRange().first;
			}
			for(int r= r_init;r<attack->getRange().second;r++){
				if(d==0){
					posTarget.first=player->getX()+r;
					posTarget.second=player->getY();
				}else if(d==1){
					posTarget.first=player->getX()-r;
					posTarget.second=player->getY();
				}else if(d==2){
					posTarget.first=player->getX();
					posTarget.second=player->getY()-r;
				}else if(d==3){
					posTarget.first=player->getX();
					posTarget.second=player->getY()+r;
				}
				posTargets.push_back(posTarget);
			}
			
		}
		cout<<"posTargets défini (ta)"<<endl;
		cout<<"fonction HeuristicIA::getTargetAccessible : boucle targets"<<endl;
		for (size_t p=0; p<posTargets.size();p++){
			cout<<"fonction HeuristicIA::getTargetAccessible : target n#"<<p<<endl;
			std::vector<Field*> v_caseField;
			std::vector<state::Player*> players = state.getPlayers();
			size_t sizePlayers = players.size();
			//verifier si il n'ya pas de blockattck et si attaque possible
			for(int d=0;d<4;d++){
				cout<<"direction "<<d<<endl;
				for(size_t i=0;i<attack->getArea().size();i++){
					is_blocked=false;
					cout<<"area"<<endl;
					//ajoute la zone d'effet de l'attaque
					if(d==0){
						pos_y=posTargets[p].second+attack->getArea()[i].second;
						pos_x=posTargets[p].first+attack->getArea()[i].first;
					}else if(d==1){
						pos_y=posTargets[p].second+attack->getArea()[i].first;
						pos_x=posTargets[p].first-attack->getArea()[i].second;
					}else if(d==2){
						pos_y=posTargets[p].second-attack->getArea()[i].second;
						pos_x=posTargets[p].first-attack->getArea()[i].first;
					}else if(d==3){
						pos_y=posTargets[p].second-attack->getArea()[i].first;
						pos_x=posTargets[p].first+attack->getArea()[i].second;
					}
					cout<<"pos x/y defini"<<endl;
					v_caseField.push_back(state.getGrid()[pos_y][pos_x]);
					statusCase = v_caseField[i]->getFieldStatus();
					//for(size_t c=0; c<statusCase.size();c++){
						//if (statusCase[c].first==BLOCKATTACK && statusCase[c].second!=0){
						if (statusCase[10].second!=0){
							is_blocked=true;
						}
					//}
					if(!is_blocked){
						cout << "Attaque non bloquee par un obstacle" << endl;
						//todo verifier si case pas déja presente dans liste
						v_posField.push_back({pos_x,pos_y});
						for(size_t q = 0; q<sizePlayers;q++){
							if(players[q]->getX()==pos_x && players[q]->getY()==pos_y ){
								targets.push_back(players[q]);
							}
						}
					}

				}
			}
		}
		
	}
	cout<<"fonction HeuristicIA::getTargetAccessible : fin d'execution"<<endl;
	return {v_posField,targets};
}


std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> HeuristicIA::getTargetDirect(state::State state, state::Player* player, int skill_num){
	cout<<"fonction HeuristicIA::getTargetDirect : début d'execution"<<endl;
	//renvoie l'ensemble des cases attaquables directement par le joueur pour une attaque donnée (tout type)
	state::Skill* attack = player->getSkills()[skill_num];
	std::pair<int,int> pos_player = {player->getX(),player->getY()};
	std::vector<std::pair<int,int>> v_posField;
	std::vector<state::Player*> targets;

	
	//ajoute tout les cible target case possible avec range
	std::vector<std::pair<int,int>> posTargets;
	std::pair<int,int> posTarget;
	for(size_t d=0;d<4;d++){
		int r_init;
		if(attack->getRange().first==0){
			r_init = 1;
			posTargets.push_back(pos_player);
		}else{
			r_init =attack->getRange().first;
		}
		for(int r= r_init;r<attack->getRange().second;r++){
			if(d==0){
				posTarget.first=player->getX()+r;
				posTarget.second=player->getY();
			}else if(d==1){
				posTarget.first=player->getX()-r;
				posTarget.second=player->getY();
			}else if(d==2){
				posTarget.first=player->getX();
				posTarget.second=player->getY()-r;
			}else if(d==3){
				posTarget.first=player->getX();
				posTarget.second=player->getY()+r;
			}
			posTargets.push_back(posTarget);
		}
		
	}
	cout<<"posTargets défini (td)"<<endl;
	for (size_t p=0; p<posTargets.size();p++){
		
		std::vector<Field*> v_caseField;
		std::vector<state::Player*> players = state.getPlayers();
		size_t sizePlayers = players.size();
		int pos_x;
		int pos_y;
		bool is_blocked;
		
		//verifier si il n'ya pas de blockattck et si attaque possible
		for(int d=0;d<4;d++){
			for(size_t i=0;i<attack->getArea().size();i++){
				is_blocked=false;
				//ajoute la zone d'effet de l'attaque
				if(d==0){
					pos_y=posTargets[p].second+attack->getArea()[i].second;
					pos_x=posTargets[p].first+attack->getArea()[i].first;
				}else if(d==1){
					pos_y=posTargets[p].second+attack->getArea()[i].first;
					pos_x=posTargets[p].first-attack->getArea()[i].second;
				}else if(d==2){
					pos_y=posTargets[p].second-attack->getArea()[i].second;
					pos_x=posTargets[p].first-attack->getArea()[i].first;
				}else if(d==3){
					pos_y=posTargets[p].second-attack->getArea()[i].first;
					pos_x=posTargets[p].first+attack->getArea()[i].second;
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
					//todo verifier si case pas déja presente dans liste
					v_posField.push_back({pos_x,pos_y});
					for(size_t q = 0; q<sizePlayers;q++){
						if(players[q]->getX()==pos_x && players[q]->getY()==pos_y ){
							targets.push_back(players[q]);
						}
					}
				}

			}
		}
	}
		
	
	cout<<"fonction HeuristicIA::getTargetDirect : fin d'execution"<<endl;
	return {v_posField,targets};
}

std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> HeuristicIA::getPosAttack (state::Player* player, state::State state){
	cout<<"fonction HeuristicIA::getPosAttack : début d'execution"<<endl;
	//renvoie l'ensemble des joueurs etcases attaquables (dégats) par le joueur
	std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> listPosSkill;
	std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>>   targets;
	for(size_t s=0; s<player->getSkills().size();s++){
		if(player->getSkills()[s]->getCooldown()==0 && player->getSkills()[s]->getDamage()>0){
	cout<<"fonction HeuristicIA::getTargetAccessible : appelé par getPosAttack"<<endl;
			targets = getTargetAccessible(state,player,s);
			listPosSkill.first.insert(listPosSkill.first.end(),targets.first.begin(), targets.first.end());
			listPosSkill.second.insert(listPosSkill.second.end(),targets.second.begin(), targets.second.end());
		}
	}
	cout<<"fonction HeuristicIA::getPosAttack : fin d'execution"<<endl;
	return listPosSkill;
}

std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> HeuristicIA::getPosSkill (state::Player* player, state::State state){
	cout<<"fonction HeuristicIA::getPosSkill : début d'execution"<<endl;
	//renvoie l'ensemble des joueurs et cases attaquables (tout type) et par le joueur
	std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> listPosSkill;
	std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> targets;
	for(size_t s=0; s<player->getSkills().size();s++){
		if(player->getSkills()[s]->getCooldown()==0){
	cout<<"fonction HeuristicIA::getTargetAccessible : appelé par getPosSkill"<<endl;
			targets = getTargetAccessible(state,player,s);
			listPosSkill.first.insert(listPosSkill.first.end(),targets.first.begin(), targets.first.end());
			listPosSkill.second.insert(listPosSkill.second.end(),targets.second.begin(), targets.second.end());
		}
	}
	cout<<"fonction HeuristicIA::getPosSkill : fin d'execution"<<endl;
	return listPosSkill;
}
