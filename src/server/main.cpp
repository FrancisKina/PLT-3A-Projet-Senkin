#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <time.h>
#include <unistd.h>
#include <ctime>
#include <SFML/Graphics.hpp>

#include "state.h"
#include "../client/render.h"
#include "engine.h"
#include "ai.h"
#include "../client/client.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;
using namespace client;

int main(int argc,char* argv[])
{
	
    if (argc>1){
		
		/*record: enregistre les commandes*/
		if(strcmp(argv[1],"record")==0){
			
			srand(time(NULL));
			//Initialisation générale
				//Initialisation de la grille par le moteur
			Engine engine;
			State& state = engine.getState();
			state.initGrid("res/maptest.txt");
			
				//Initialisation des personnages
			CharacterFactory cf;
			state.initPlayers(4);
			
					//Definition en IA
			state.getPlayers()[0]->setIa(true);
			state.getPlayers()[1]->setIa(true);
			state.getPlayers()[2]->setIa(true);
			state.getPlayers()[3]->setIa(true);
					//Classes
			state.getPlayers()[0]->setCharacter(cf.createCharacter(KNIGHT));
			state.getPlayers()[1]->setCharacter(cf.createCharacter(FROG));
			state.getPlayers()[2]->setCharacter(cf.createCharacter(ARCHER));
			state.getPlayers()[3]->setCharacter(cf.createCharacter(DWARF));
					//Positions
			state.getPlayers()[0]->setX(10);
			state.getPlayers()[0]->setY(10);
			state.getPlayers()[1]->setX(10);
			state.getPlayers()[1]->setY(14);
			state.getPlayers()[2]->setX(14);
			state.getPlayers()[2]->setY(14);
			state.getPlayers()[3]->setX(14);
			state.getPlayers()[3]->setY(10);
				//Initialisation des skills
			SkillFactory sf;
			state.getPlayers()[0]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(PIETINEMENT)});
			state.getPlayers()[1]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(SOIN_LEGER)});
			state.getPlayers()[2]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(ARC)});
			state.getPlayers()[3]->setSkills({sf.createSkill(SOIN), sf.createSkill(FEU_D_ENFER)});
			
			//debut du jeu
			engine.setEnableRecord(true);
			engine.startGame(state);	
			
			Json::StyledWriter styledWriter;
			std::ofstream file_id;
			file_id.open("res/replay.txt");
			
			RandomIA ai;
			clock_t begin = clock();
			clock_t end = clock();

			while((end - begin)/ CLOCKS_PER_SEC<60){
				engine.getRecord();
				ai.run(engine);
				end = clock();
				cout<<endl<<endl<<"---------------------------------------------------"<<endl;
				cout<<"         temps clock = "<<(end - begin)/ CLOCKS_PER_SEC<<endl<<"-----------------------------------------"<<endl<<endl;
			}
			file_id << styledWriter.write(engine.getRecord());

			file_id.close();
		
		}
		
		cout << argv[1] << endl;
	
	}
	
}
