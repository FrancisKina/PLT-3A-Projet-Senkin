#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <unistd.h>
#include <microhttpd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> 

#include "state.h"
#include "../client/render.h"
#include "../client/client.h"
#include "engine.h"
#include "ai.h"
#include "server.h"

#define SOCKET_ERROR -1

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;
using namespace server;

class Request {
public:
    struct MHD_PostProcessor *pp = nullptr;
    string data;
    ~Request() {
        if (pp) MHD_destroy_post_processor (pp);
    }
};

static int post_iterator(void *cls,
        enum MHD_ValueKind kind,
        const char *key,
        const char *filename,
        const char *content_type,
        const char *transfer_encoding,
        const char *data, uint64_t off, size_t size) 
{
    return MHD_NO;
}

static int
main_handler (void *cls,      
          struct MHD_Connection *connection,
          const char *url, 
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr) 
{
    // Données pour une requête (en plusieurs appels à cette fonction)
    Request *request = (Request*)*ptr;

    // Premier appel pour cette requête
    if (!request) { 
        request = new Request();
        if (!request) {
            return MHD_NO;
        }
        *ptr = request;
        if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
         || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
            request->pp = MHD_create_post_processor(connection,1024,&post_iterator,request);
            if (!request->pp) {
                cerr << "Failed to setup post processor for " << url << endl;
                return MHD_NO;
            }
        }
        return MHD_YES;
    }    
    
    // Cas où il faut récupérer les données envoyés par l'utilisateur
    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
     || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
        MHD_post_process(request->pp,upload_data,*upload_data_size);
        if (*upload_data_size != 0) {
            request->data = upload_data;
            *upload_data_size = 0;
            return MHD_YES;
        }    
    }

    HttpStatus status;
    string response;
    try {

        ServicesManager *manager = (ServicesManager*) cls;
        status = manager->queryService(response,request->data,url,method);
    }
    catch(ServiceException& e) {
        status = e.status();
        response = e.what();
        response += "\n";
    }
    catch(exception& e) {
        status = HttpStatus::SERVER_ERROR;
        response = e.what();
        response += "\n";
    }
    catch(...) {
        status = HttpStatus::SERVER_ERROR;
        response = "Unknown exception\n";
    }

    struct MHD_Response *mhd_response;
    mhd_response = MHD_create_response_from_buffer(response.size(),(void *)response.c_str(),MHD_RESPMEM_MUST_COPY);
    if (strcmp(method,MHD_HTTP_METHOD_GET) == 0) {
        MHD_add_response_header(mhd_response,"Content-Type","application/json; charset=utf-8");
    }
    int ret = MHD_queue_response(connection, status, mhd_response);
    MHD_destroy_response(mhd_response);
    return ret;
}

static void request_completed (void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe){
  Request *request = (Request*)*con_cls;
  if (request) {
      delete request;
      *con_cls = nullptr;
  }
}

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
		
		/*	listen : le serveur écoute et attend l'arrivée de nouveaux joueurs */
		else if(strcmp(argv[1], "listen") == 0){
			try {
				VersionService versionService;
				std::unique_ptr<AbstractService> ptr_versionService (new VersionService(versionService));

				ServicesManager servicesManager;
				servicesManager.registerService(move(ptr_versionService));

				Game game;

				PlayerService playerService(std::ref(game));
				std::unique_ptr<AbstractService> ptr_playerService (new PlayerService(playerService));

				servicesManager.registerService(move(ptr_playerService));

				struct MHD_Daemon *d;
				if (argc != 2) {
				    printf("%s PORT\n", argv[0]);
				    return 1;
				}
				
				d = MHD_start_daemon(// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
				        MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
				        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
				        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
				        8080,
				      	NULL, NULL, 
				        &main_handler, (void*) &servicesManager,
				        MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
				        MHD_OPTION_END);
				        
				if (d == NULL)
					return 1;
				cout << "Pressez <entrée> pour arrêter le serveur" << endl;
				(void) getc(stdin);
				MHD_stop_daemon(d);
   			 }
			catch(exception& e) {
				cerr << "Exception: " << e.what() << endl;
			}
		}
		
		cout << argv[1] << endl;
	
	}
	
	
}
