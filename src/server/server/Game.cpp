#include "../server.h"
#include <memory>
#include <map>
#include <vector>
#include <set>
#include "state.h"
#include "ai.h"
#include "engine.h"

using namespace state;
using namespace engine;
using namespace server;
using namespace std;
using std::unique_ptr;
using std::map;
using std::vector;


Game::Game(): state(state){
	idseq=1;  
	//Initialisation générale
		//Initialisation de la grille par le moteur
	Engine engine;
	State& state = engine.getState();
	state.initGrid("res/maptest.txt");
	state.initPlayers(2);
	/*state.getGrid()[12][11]->updateFieldStatus({GRAVITY,999});
	state.getGrid()[10][10]->updateFieldStatus({BURNING,999});
	state.getGrid()[10][10]->updateFieldStatus({POISON,999});
	//state.getGrid()[10][10]->updateFieldStatus({SNOW,999});
	state.getGrid()[11][11]->updateFieldStatus({TENT,999});
	state.getGrid()[13][11]->updateFieldStatus({FORT,999});
	state.getGrid()[13][13]->updateFieldStatus({TOWER,999});*/
			engine.startGame(state);
}

std::map<int, std::unique_ptr<server::Player>>& Game::getPlayersList(){
	std::map<int, std::unique_ptr<server::Player>>& ref=players;
	return ref;
}

const server::Player* Game::getPlayer (int id) const {
//server::Player* Game::getPlayer (int id) {
    auto ite = players.find(id);
    if (ite == players.cend())
        return nullptr;
    return ite->second.get();
}

int Game::addPlayer (std::unique_ptr<server::Player> player) {
    int id = idseq++;
    players.insert(std::make_pair(id, std::move(player)));
    return id;
}

void Game::setPlayer (int id, std::unique_ptr<server::Player> player) {
    players[id] = std::move(player);
    if (id > idseq) {
        idseq = id;
    }
}

void Game::removePlayer (int id) {
    auto ite = players.find(id);
    if (ite == players.end())
        return;
    players.erase(ite);
}

int Game::getIDseq(){
	return idseq;
}

engine::Engine& Game::getEngine(){
	return engine;
}

state::State Game::getState(){
	return state;
}
