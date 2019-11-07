#include "Skill.h"
#include <tuple>

using namespace state;

Skill::Skill(){
	damage = 0;
	range = {0,0};
	area = {};
	precision = 0;
	effect = {};
	cooldownBase = 0;
	cooldown = 0;
	special = false;
	id = FRAPPE;
}

int Skill::getDamage(){
    return damage;
}

std::pair<int,int> Skill::getRange(){
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


void Skill::setDamage(int newDamage){
    damage = newDamage;
}

void Skill::setRange(std::pair<int,int> newRange){
    range = newRange;
}

void Skill::setCooldown(int newCooldown){
    cooldown = newCooldown;
}

void Skill::setSpecial(bool isSpecial){
    special = isSpecial;
}

void Skill::setArea(std::vector<std::pair<int,int>> newArea){
    area = newArea;
}

void Skill::setEffect(std::vector<std::tuple<EffectId,int,int>> newEffect){
    effect = newEffect;
}

void Skill::setPrecision(int newPrecision){
	precision = newPrecision;
}

SkillId Skill::getSkillId (){
	return id;
}
void Skill::setSkillId (SkillId newSkillId){
	id=newSkillId;
}

int Skill::getCooldownBase(){
	return cooldownBase;
}

void Skill::setCooldownBase(int newCDB){
	cooldownBase =  newCDB;
}
