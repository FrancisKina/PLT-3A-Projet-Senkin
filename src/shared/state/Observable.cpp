#include "Observable.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace state;
using namespace std;

void Observable::registerObserver (IObserver* observer){
	observers.push_back(observer);
}

void Observable::notifyObservers(State& state){
	//for(auto observer : observers){
	for(size_t i = 0; i<observers.size(); i++){
		observers[i]->stateChanged(state);
	}
}
