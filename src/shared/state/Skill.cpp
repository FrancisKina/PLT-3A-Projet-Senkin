#include "Skill.h"
#include <tuple>

using namespace state;

int Skill::getDamage(){
    return damage;
}

int Skill::getRange(){
    return range;
}

int Skill::getCooldown(){
    return cooldown;
}

bool Skill::getSpecial(){
    return special;
}

std::vector<std::pair<int,int>>  Skill::getArea(){
    return area;
}

std::vector<std::tuple<EffectId,int,int>> Skill::getEffect(){
    return effect;
}

int Skill::getPrecision(){
	return precision;
}

