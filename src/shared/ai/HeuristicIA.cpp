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


int HeuristicIA::run (engine::Engine& engine){
	cout<<"----------------------------------------------"<<endl;
	cout<<"fonction HeuristicIA::run : début d'execution"<<endl;
	State state = engine.getState();
	Player* player = state.getPlaying();
	if(player->getIa()==true){
		std::pair<int,int> aimed_position;
		bool premierEssai = true;
		srand(time(NULL));
		
		bool gotheal=false;
		int idheal;
		int id_skill;
		bool gotattack=false;
		std::vector<int> idskill;
		std::vector<Player*> targets;
		std::vector<Player*> targetsdirect;
		std::pair<int,int> player_position;
		std::vector<std::vector<Field*>> grid;
		std::vector<std::pair<int,int>> listPosMove;
		std::vector<state::Player*> players;
		Skill* skill;
		std::vector<Player*> gtd;
		std::vector<std::pair<FieldStatusId,int>> player_field_status;
		
		bool dangerousfield;
		bool move_from_danger;
		bool move_to_foe;
			
		std::vector<std::pair<FieldStatusId, int>> field_status;
		
		
		std::tuple<Player*,int,std::pair<int,int>,bool,bool> gmt;
	
		move_from_danger=true;
		move_to_foe=true;
		
		while (player->getHp() > 0){
			clock_t start_time = clock();
			while(clock()<start_time+100000);
			//cout<<"new time"<<endl;
			idskill={};
			state = engine.getState();
			player = state.getPlaying();
			player_position={player->getX(),player->getY()};
			grid =state.getGrid();
			targetsdirect={};
			Field* player_field = grid[player_position.second][player_position.first];
			players={};
			for(size_t p = 0; p< state.getPlayers().size(); p++){
				if(state.getPlayers()[p]!=player){
					players.push_back(state.getPlayers()[p]);
				}
			}
			//ensemble des attaques à dégat
			for(size_t s=0;s<player->getSkills().size();s++){
				Skill* skill = player->getSkills()[s];
				if(skill->getDamage()>0){
					idskill.push_back(s);
				}
			}
			if(premierEssai == true){
				cout<< "\t [Controle par le CPU heuristique] " << endl;
				premierEssai = false;
			}
			gotattack = false;
			gotheal = false;
			//test attaques
			if(player->getSkillCount()>0){
				//test : attaque de soin dispo
				for(size_t s=0;s<player->getSkills().size();s++){
					skill = player->getSkills()[s];
					for(size_t e=0; e<skill->getEffect().size();e++){
						if(skill->getCooldown()==0 && (get<0>(skill->getEffect()[e])==HEAL_LOW || get<0>(skill->getEffect()[e])==HEAL_MEDIUM || get<0>(skill->getEffect()[e])==HEAL_HIGH ) && skill->getRange().first==0){
							gotheal=true;
							//voir plus tard si tjr soin le plus fort dispo d'abord
							idheal=s;
						}
					}
				}
			}
			dangerousfield=false;
			player_field_status=player_field->getFieldStatus();
			//test : terrain dangereux
			if(player->getMovement()>0 && (player_field_status[12].second>0 || player_field_status[13].second>0)){
				dangerousfield=true;
			}
		
			
			gmt = getMainTarget(engine);
			bool direct_attack = get<3>(gmt);
			id_skill = get<1>(gmt);
			aimed_position = get<2>(gmt);
			bool can_attack = get<4>(gmt);
			/*cout<<"direct attaque : "<<direct_attack<<endl;
			cout<<"id_skill : "<<id_skill<<endl;
			cout<<"aimed position : ("<<aimed_position.first<<","<<aimed_position.second<<")"<<endl;
			cout<<"can attaque : "<<can_attack<<endl;
			
			cout<<"move_to_foe"<<move_to_foe<<endl;*/
			//-------------------------COMPORTEMENT-----------------------------------
			cout<<endl<<"IA Action:";
			//si peu de pv et sort de soin dispo
			if(player->getHp()<player->getCharacter()->getHpBase()*0.2 and gotheal){
				cout<<"soin urgent"<<endl;
				Attack* attack = new Attack(player_position , idheal);
				engine.executeCommand(attack);
				//si ennemi a porté direct, attaque (au meilleur socre si plusieur)*/
			}else if(direct_attack){
			//}else if(gotrangedirect){
				cout<<"attaque direct"<<endl;Attack* attack = new Attack(aimed_position , id_skill);
				cout<<"skill lancé :"<<id_skill<<endl;
				engine.executeCommand(attack);
				
			//sinon, avancé vers l'ennemi le plus proche
			}else if(!direct_attack && can_attack && move_to_foe && !(player->getX()==aimed_position.first && player->getY()==aimed_position.second)){
				cout<<"rapproche ennemi"<<endl;
				cout<<"position d'origine "<<player->getX()<<","<<player->getY()<<endl;
				cout<<"Movement vers ennemi :"<<aimed_position.first<<","<<aimed_position.second<<endl;
				//move_to_foe=randomWay(aimed_position, state, player, engine, window);
				Move* move = new Move(aimed_position);
				engine.executeCommand(move);
				if(player->getX()==aimed_position.first && player->getY()==aimed_position.second){
					move_to_foe=true;
					cout<<"Mouvement réussi"<<endl<<endl;
				}else{
					move_to_foe=false;
					cout<<"Mouvement raté"<<endl;
				}
				//cout<<"move done : "<<move_to_foe<<endl;
			//sinon si attaque de soin et pv<0.9*pvbase, soin
			}else if(player->getHp()<player->getCharacter()->getHpBase()*0.9 and gotheal){
				cout<<"soin"<<endl;
				Attack* attack = new Attack(player_position ,idheal);
				engine.executeCommand(attack);
			}else{
				cout<<"fin action"<<endl;
				//si a porté ennemi et pas attaque ou plus pm et pa, fin de tour
				EndActions* endactions = new EndActions();
				engine.executeCommand(endactions);
				cout<<"fonction HeuristicIA::run : fin d'execution"<<endl;
	cout<<"----------------------------------------------"<<endl;
				return 0;
			}
		}
		cout<<"fonction HeuristicIA::run : fin d'execution(erreur)"<<endl;
	cout<<"----------------------------------------------"<<endl;
		return 0;
	}
	cout<<"fonction HeuristicIA::run : fin d'execution(erreur n#2 : pas une ia)"<<endl;
	cout<<"----------------------------------------------"<<endl;
	return 0;
}

bool HeuristicIA::randomWay(std::pair<int,int> destination, state::State state, state::Player* player, engine::Engine& engine){
	cout<<"fonction HeuristicIA::randomWay : début d'execution"<<endl;
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
	cout<<"Mouvement total : "<<H<<" horizontal, "<<V<<" vertical"<<endl;
	if(H==0 && V==0){
		move_possible=false;
		cout<<"Mouvement sur place"<<endl;
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
		cout<<"Mouvement de ("<<X<<","<<Y<<") vers ("<<Xp<<","<<Yp<<")"<<endl;
		position={Xp,Yp};
		
		Move* move = new Move(position);
		engine.executeCommand(move);
		if(player->getX()==position.first && player->getY()==position.second){
			X=player->getX();
			Y=player->getY();
			H=Xf-X;
			V=Yf-Y;
			cout<<"Mouvement réussi"<<endl<<endl;
		}else{
			move_possible=false;
			cout<<"Mouvement raté"<<endl;
		}
	}
	cout<<"fonction HeuristicIA::randomWay : fin d'execution"<<endl;
	return move_possible;
}

std::tuple<state::Player*,int,std::pair<int,int>,bool,bool> HeuristicIA::getMainTarget(engine::Engine& engine){
	//renvoie le meilleur cible pour le joueur et pour une liste d'attaque, sur l'ennemi le plus proche (renvoie alors la position a attaquer), sinon la cible optimal( la case pour etre a porté/se rapprocher de la cible optimal)
	cout<<"fonction HeuristicIA::getMainTarget : début d'execution"<<endl;
	//attention postion en sortie=aimed_position!=position target sortie (si attaque en zone)
	std::vector<int>  score={}; //fct de d_s, p_s, hp_t, dist et nb_t vpt etat target
	//std::vector<std::vector<int>>  precision_skill; //precision par attaque par personnage ciblé
	//std::vector<int>  num_targets;//nombre de personnage touché par attaque
	std::vector<int> distance;
	std::vector<int> id_skills={};
	std::vector<int> id_direct_skills={};
	state::State state=engine.getState();	
	int id_skill;
	Player* main_target;
	//std::vector<std::vector<Player*>> target;
	bool direct_attack=false;
	//bool range_attack=false;
	std::pair<int,int> aimed=std::make_pair(0,0);
	int hp;
	int dist;
	Player* player = state.getPlaying();
	std::vector<Player*> players = state.getPlayers();
	std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> direct;
	std::vector<int> inter;
	std::vector<Player*> inter_p;
	Skill* skill;
	std::vector<Skill*> direct_skills = {};
	std::vector<Skill*> skills = {};
	int score_min;
	std::pair<int,int> main_skill_range;
	std::vector<std::pair<int,int>> main_skill_area;
	int x_area,y_area, x_range, y_range;
	int r_init;
	std::vector<std::pair<int,int>> listPos;
	//test player : pas mettre le joueur
	//cout<<"size players de base "<<players.size()<<endl;
	for(size_t i=0;i<players.size();i++){
		if(players[i]==player){
			players.erase(players.begin()+i);
			//cout<<"player = "<<player->getName()<<endl;
		}
	}
	//cout<<"size players "<<players.size()<<endl;
	//test cible plus proche
	Player* closest_foe;
	closest_foe=players[0];
	int dist_min;
	dist_min=(player->getX()-players[0]->getX())*(player->getX()-players[0]->getX())+(player->getY()-players[0]->getY())*(player->getY()-players[0]->getY());
	int dist_cf;
	for(size_t cf=1; cf<players.size();cf++){
		dist_cf =(player->getX()-players[cf]->getX())*(player->getX()-players[cf]->getX())+(player->getY()-players[cf]->getY())*(player->getY()-players[cf]->getY());
		if(dist_min>dist_cf){
			closest_foe=players[cf];
			dist_min=dist_cf;
		}
	}
	main_target=closest_foe;
	hp=main_target->getHp();
	bool can_attack=false;
	//test attaque degat direct ou deplacement
	if(player->getSkillCount()>0){
		//cout<<"fonction HeuristicIA::getMainTarget : test type d'action : debut boucle skills"<<endl;
		//cout<<"size skills : "<<player->getSkills().size()<<endl;
		for(size_t s=0; s<player->getSkills().size(); s++){
			skill = player->getSkills()[s];
			//cout<<"fonction HeuristicIA::getMainTarget : test type d'action : boucle skills n#"<<s<<endl;
			//attaques de degat
			if(skill->getDamage()>0){
				//cout<<"fonction HeuristicIA::getTargetDirect : appelé par getMainTarget"<<endl;
				id_skills.push_back(s);
				skills.push_back(skill);
				direct = getTargetDirect(state, player, s);
				cout<<endl<<endl<<"Attaque "<<skill->getName();
				cout<<" a "<<direct.second.size()<<" cibles directes pour "<<direct.first.size()<<" cases visable"<<endl<<endl<<endl;
				can_attack=true;
				for(size_t d=0;d<direct.second.size();d++){
					//attaque de dégat lancable tout de suite
					if(direct.second[d]==closest_foe && skill->getCooldown()<1){
						direct_attack=true;
						direct_skills.push_back(skill);
						id_direct_skills.push_back(s);
					}
				}
			}
			//cout<<"fonction HeuristicIA::getMainTarget : test type d'action : fin boucle player"<<endl;
		}
	}
	//cout<<"fonction HeuristicIA::getMainTarget : test type d'action : fin boucle skills"<<endl;
	//cout<<"resultat test: directe attaque= "<<direct_attack<<endl;
	//calcul des scores
	if(can_attack){
		if(direct_attack){
			skills=direct_skills;
			id_skills=id_direct_skills;
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score (direct): attaque directe"<<endl;
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score (direct): debut boucle skills"<<endl;
			for(size_t s=0; s<skills.size(); s++){
				skill=player->getSkills()[id_skills[s]];
				//cout<<"fonction HeuristicIA::getMainTarget : calcul de score (direct): boucle skill n#"<<s<<endl;
				//si ennemi a porté direct, attaque
					//vpt:calcul precision
					score.push_back(hp-skill->getDamage());//(hp-skill->getDamage())*num_target/precision vpt change en float
				//cout<<"score attaque directe n#"<<s<<"="<<hp-skill->getDamage()<<endl;
			}
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score (direct): fin boucle skills"<<endl;
		}else{
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: aucun ennemie à porte "<<endl;
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: debut boucle skills"<<endl;
			for(size_t s=0; s<skills.size(); s++){
				skill=player->getSkills()[id_skills[s]];
				//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: boucle skill n#"<<s<<endl;
				//vpt:calcul precision
					dist=(main_target->getX()-player->getX())*(main_target->getX()-player->getX())+(main_target->getY()-player->getY())*(main_target->getY()-player->getY());
					//cout<<"dist="<<dist<<endl;
					//cout<<"hp="<<hp<<endl;
					score.push_back((hp-skill->getDamage())*dist);//(hp-skill->getDamage())*num_target*dist/precision vpt change en float 
				//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: fin calcul"<<endl;
				//cout<<"score attaque n#"<<s<<"="<<hp-skill->getDamage()<<endl;
			}
			//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: fin boucle player"<<endl;
		}
		//cout<<"fonction HeuristicIA::getMainTarget : calcul de score: fin boucle skills"<<endl;
		//comparaison des scores
		if(skills.size()!=0){
			score_min = score[0];
		}
		//cout<<"fonction HeuristicIA::getMainTarget : definition main_taget: debut boucle skill"<<endl;
		for(size_t s=0; s<skills.size(); s++){
			//cout<<"fonction HeuristicIA::getMainTarget : definition main_taget: boucle skill n#"<<s<<endl;
			
				//cout<<"score min="<<score_min<<endl;
				//cout<<"Score attaque n#"<<id_skills[s]<<" = "<<score[id_skills[s]]<<endl;
				if(score_min>=score[s] ){
					//cout<<"nouveau score min"<<endl;
					score_min=score[s];
					id_skill=id_skills[s];
				}
				//cout<<endl;
				//cout<<"skill_id.size()"<<id_skills.size()<<endl<<endl;
			
		}
		//definition cible
		if(id_skills.size()!=0){
			main_skill_range=player->getSkills()[id_skill]->getRange();
			main_skill_area=player->getSkills()[id_skill]->getArea();
			//cout<<"fonction HeuristicIA::getMainTarget : definition case ciblé:"<<endl;
			if(direct_attack){
				aimed={player->getX()+main_skill_range.first,player->getY()};
				//verifier dans quel direction et range est l'attaque
				for(size_t d=0; d<4;d++){
					if(main_skill_range.first==0){
						r_init=1;
					}else{
						r_init=main_skill_range.first;
					}
					for(int r=r_init;r<main_skill_range.second+1;r++){
						for(size_t a=0;a<main_skill_area.size();a++){
							if(d==0){
								//nord
								x_area=-main_skill_area[a].first;
								y_area=-main_skill_area[a].second;
								x_range=0;
								y_range=-r;
							}else if(d==1){
								//sud
								x_area=main_skill_area[a].first;
								y_area=main_skill_area[a].second;
								x_range=0;
								y_range=r;
							}else if(d==2){
								//est
								x_area=-main_skill_area[a].second;
								y_area=main_skill_area[a].first;
								x_range=r;
								y_range=0;
							}else if(d==3){
								//ouest
								x_area=main_skill_area[a].second;
								y_area=-main_skill_area[a].first;
								x_range=-r;
								y_range=0;
							}
							if(main_target->getX()==(player->getX()+x_area+x_range) && main_target->getY()==(player->getY()+y_area+y_range)){
								aimed={player->getX()+x_range,player->getY()+y_range};
								cout<<"Cible prinipal:"<<main_target->getName()<<endl;
								cout<<"position visé à attaqué = ("<<aimed.first<<","<<aimed.second<<")"<<endl;
								cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(directe)"<<endl;
								return std::make_tuple(main_target,id_skill,aimed,direct_attack,can_attack);
							}
						}
					}
				}
			}else{
				int H, V, X, Xf, Xp, Y, Yf, Yp, ax,ay,bx,by;
				X=player->getX();
				Y=player->getY();
				Xf=main_target->getX();
				Yf=main_target->getY();
				H=Xf-X;
				V=Yf-Y;
				if((abs(Xp-X)==0 && abs(Yp-Y)==1) || (abs(Xp-X)==1 && abs(Yp-Y)==0)){
					cout<<"Cible prinipal:"<<main_target->getName()<<endl;
					cout<<"position aléatoir pour ce raprocher de l'ennemi = ("<<aimed.first<<","<<aimed.second<<")"<<endl;
					cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(a une case de la cible)"<<endl;
					return std::make_tuple(main_target,id_skill,aimed,direct_attack,can_attack);
				}
				int direction;
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
				aimed={Xp,Yp};
				cout<<"Cible prinipal:"<<main_target->getName()<<endl;
				cout<<"position aléatoir pour ce raprocher de l'ennemi = ("<<aimed.first<<","<<aimed.second<<")"<<endl;
				cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(ditance)"<<endl;
				return std::make_tuple(main_target,id_skill,aimed,direct_attack,can_attack);
			}
			cout<<"Cible prinipal:"<<main_target->getName()<<endl;
			cout<<"position pour se rapprocher de l'ennemi = ("<<aimed.first<<","<<aimed.second<<")"<<endl;
			cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(cible trop loin)"<<endl;
			return std::make_tuple(main_target,id_skill,aimed,direct_attack,can_attack);
		}
	}else{
		cout<<"Cible prinipal:"<<main_target->getName()<<endl;
		cout<<"position pour se rapprocher de l'ennemi = ("<<aimed.first<<","<<aimed.second<<")"<<endl;
		cout<<"fonction HeuristicIA::getMainTarget : fin d'execution(pas d'attaque possible)"<<endl;
		return std::make_tuple(main_target,id_skill,aimed,direct_attack,can_attack);
	}
}

std::pair<std::vector<std::pair<int,int>>,std::vector<state::Player*>> HeuristicIA::getTargetDirect(state::State state, state::Player* player, int skill_num){
	//cout<<"fonction HeuristicIA::getTargetDirect : début d'execution"<<endl;
	//renvoie l'ensemble des cases attaquables directement par le joueur pour une attaque donnée (tout type)
	state::Skill* attack = player->getSkills()[skill_num];
	std::pair<int,int> pos_player = {player->getX(),player->getY()};
	std::vector<std::pair<int,int>> v_posField={};
	std::vector<state::Player*> targets={};
	std::vector<std::pair<state::FieldStatusId,int>> statusCase;
	std::vector<state::Player*> players = state.getPlayers();
	size_t sizePlayers = players.size();
	int pos_x;
	int pos_y;
	bool is_blocked;

	
	//ajoute tout les cible target case possible avec range
	std::vector<std::pair<int,int>> posTargets={};
	std::pair<int,int> posTarget;
	for(size_t d=0;d<4;d++){
		int r_init;
		if(attack->getRange().first==0){
			r_init = 1;
			posTargets.push_back(pos_player);
		}else{
			r_init =attack->getRange().first;
		}
		for(int r= r_init;r<attack->getRange().second+1;r++){
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
	//cout<<"taille area "<<attack->getArea().size()<<endl;
	//cout<<"taille posTargets "<<posTargets.size()<<endl;
	for (size_t p=0; p<posTargets.size();p++){
		//cout<<"boucle de postargets n"<<p<<endl;
		
		//verifier si il n'ya pas de blockattck et si attaque possible
		for(size_t i=0;i<attack->getArea().size();i++){
			//cout<<"boucle de area n"<<i<<endl;
			for(int d=0;d<4;d++){
				is_blocked=false;
				//ajoute la zone d'effet de l'attaque
				if(d==0){
					//nord
					pos_y=posTargets[p].second-attack->getArea()[i].second;
					pos_x=posTargets[p].first-attack->getArea()[i].first;
				}else if(d==1){
					//sud
					pos_y=posTargets[p].second+attack->getArea()[i].second;
					pos_x=posTargets[p].first+attack->getArea()[i].first;
				}else if(d==2){
					//est
					pos_y=posTargets[p].second+attack->getArea()[i].first;
					pos_x=posTargets[p].first-attack->getArea()[i].second;
				}else if(d==3){
					//ouest
					pos_y=posTargets[p].second-attack->getArea()[i].first;
					pos_x=posTargets[p].first+attack->getArea()[i].second;
				}
				if(pos_x>=0 && pos_x<state.getGrid()[0].size() && pos_y>=0 && pos_y<state.getGrid().size()){
					statusCase = state.getGrid()[pos_y][pos_x]->getFieldStatus();
					for(size_t c=0; c<statusCase.size();c++){
						if (statusCase[c].first==BLOCKATTACK && statusCase[c].second!=0){
							is_blocked=true;
						}
					}
					for(size_t v=0;v<v_posField.size();v++){
						if(v_posField[v]==std::make_pair(pos_x,pos_y)){
							is_blocked=true;
						}
					}
					if(!is_blocked){
						//cout << "Attaque non bloquee par un obstacle" << endl;
						//todo verifier si case pas déja presente dans liste
						v_posField.push_back({pos_x,pos_y});
						for(size_t q = 0; q<sizePlayers;q++){
							if(players[q]->getX()==pos_x && players[q]->getY()==pos_y && players[q]!=player){
								targets.push_back(players[q]);
							}
						}
					}
				}

			}
		}
	}
		
	
	//cout<<"fonction HeuristicIA::getTargetDirect : fin d'execution"<<endl;
	return {v_posField,targets};
}

