#include "NetworkClient.h"
#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include "state.h"
#include "ai.h"
#include "engine.h"

#include "../render/StateLayer.h"
namespace render {
  class StateLayer;
};

using namespace state;
using namespace engine;
using namespace render;
using namespace std;
using namespace client;
using namespace ai;

render::StateLayer net_statelayer;
bool net_updated = true;

void net_threadEngine(void* ptr1){
	Engine* ptr_engine=(Engine*)ptr1;
	while(1){
		ptr_engine->update();
		if(!net_updated){
			net_updated = true;
		}
	}
}

NetworkClient::NetworkClient(std::string url, int port, int character) : url(url), port(port), character(character), engine(engine)
{
	cout<<"network client appeleé"<<endl;
	//Initialisation générale
	//Initialisation de la grille par le moteur
	State& state = engine.getState();
	
    displayFirstTime = true;
	//creer le perso, voir si on utilise character
	cout<<"initialisation perso"<<endl;
    playerAI = new DeepIA;
    cout<<"ia chosie"<<endl;
    cout<<"state n player = "<<state.getPlayers().size()<<endl;
    state.getPlayers()[character]->setIa(true);
    cout<<"ia appliqué"<<endl;
			SkillFactory sf;
			CharacterFactory cf;
    if(character==1){
		cout<<"character=1"<<endl;
		//Classes
		state.getPlayers()[0]->setCharacter(cf.createCharacter(KNIGHT));
		//Positions
		state.getPlayers()[0]->setX(10);
		state.getPlayers()[0]->setY(10);
		//Initialisation des skills
		state.getPlayers()[0]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(SAUT_PIQUE), sf.createSkill(PIETINEMENT)});
	}else{
		cout<<"autre j"<<endl;
		state.getPlayers()[1]->setCharacter(cf.createCharacter(FROG));
		state.getPlayers()[1]->setX(10);
		state.getPlayers()[1]->setY(14);
		state.getPlayers()[1]->setSkills({sf.createSkill(FRAPPE), sf.createSkill(SOIN_LEGER)});
	}	
    cout<<"perso initialisé"<<endl;
    //Initialisation de la liste des différents elements pour l'affichage
	//StateLayer statelayer;
	net_statelayer.initLayers(state);
	net_statelayer.initWindow(state);
	state.registerObserver(&net_statelayer);
	cout<<"fin network client"<<endl;
}

std::string NetworkClient::getGameStatus()
{
    sf::Http http(url, port);
    sf::Http::Request players0;
    players0.setMethod(sf::Http::Request::Get);
    players0.setUri("/game");
    players0.setHttpVersion(1, 0);
    sf::Http::Response playersResp0 = http.sendRequest(players0);
    Json::Reader jsonReaderPlayers0;
    Json::Value jsonPlayers0;
    jsonReaderPlayers0.parse(playersResp0.getBody(), jsonPlayers0);
    return (jsonPlayers0["status"].asUInt() == 1) ? "CREATING" : "RUNNING";
}

bool NetworkClient::getServerCommands(Json::Value &out)
{
    std::pair<int,int> pos;
    int n;
    // /!\ execute remplaer par addcommand dans lexemple et tabCmd par commande
	for (unsigned int i = 0; i < out["commands"].size(); i++){
		// Cas du deplacement
		if(out["commands"][i]["id"].asUInt() == 1){
			cout<<"deplacement"<<endl;
		
			pos.first=out["commands"][i]["xDestination"].asUInt();
			pos.second=out["commands"][i]["yDestination"].asUInt();
			
			Move* move = new Move(pos);
			
			engine.executeCommand(move);
		}
		// Cas de l'attaque
		else if(out["commands"][i]["id"].asUInt() == 2){
			cout<<"attaque"<<endl;
		
			pos.first=out["commands"][i]["xDestination"].asUInt();
			pos.second=out["commands"][i]["yDestination"].asUInt();
			n=out["commands"][i]["attaque"].asUInt();
			
			Attack* attack=new Attack(pos , n);
			
			engine.executeCommand(attack);
			
		}
		// Cas de la fin d'actions
		else if(out["commands"][i]["id"].asUInt() == 3){
			cout<<"fin de tour"<<endl;
			
			EndActions* endactions = new EndActions();
			
			engine.executeCommand(endactions);
		}
		else{
			cout << "La commande " << i << " est inconnue" << endl;
		}
		//engine.getState().notifyObservers(engine.getState());						
	}
    return true;
}

void NetworkClient::putServerCommand(engine::Command *c)
{
    sf::Http http(url, port);
    sf::Http::Request request1;
    request1.setMethod(sf::Http::Request::Post);
    request1.setUri("/command");
    request1.setHttpVersion(1, 0);
    string body = c->serialize().toStyledString();
    request1.setBody(body);
    sf::Http::Response response = http.sendRequest(request1);
}

void NetworkClient::run()
{
    sf::RenderWindow& window=net_statelayer.getWindow();
	std::thread th(net_threadEngine, &engine);
	
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if (event.type == sf::Event::KeyPressed){
				engine.setNextKeyCommand(event);
			}
		}
		if (net_updated){
			if (engine.getState().getPlayers().size()>1){
				playerAI->run(engine);
				net_updated = false;
			}
			engine.getState().notifyObservers(engine.getState());
		}
		
		window.display();
	}
	
	th.join();
}

void NetworkClient::engineUpdated(){}

void NetworkClient::engineUpdating()
{
    /*for (size_t i = 0; i < engine.getCurrentCommands().size(); i++)
    {
        Command *c = &(*engine.getCurrentCommands()[i]);
        putServerCommand(c);
    }*/
    cout << "engine updating" << endl;
    engine.update();
    usleep(150000);
}

engine::Engine& NetworkClient::getEngine(){
	return engine;
}
