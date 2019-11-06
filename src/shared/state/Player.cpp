#include "Player.h"
#include <cstddef>

using namespace state;


int Player::getX(){
    return x;
}

int Player::getY(){
    return y;
}

DirectionId Player::getDirection(){
    return direction;
}

int Player::getHp(){
    return hp;
}

int Player::getMovement(){
    return movement;
}

int Player::getInitiative(){
    return initiative;
}

int Player::getSkillCount(){
    return skillCount;
}


std::vector<std::pair<CharStatusId,int>> Player::getStatus(){
	return status;
}

bool Player::getIa(){
    return ia;
}

void Player::setX (int newX){
	x = newX;
}

void Player::setY (int newY){
	y = newY;
}

void Player::setDirection (DirectionId id){
	direction = id;
}

void Player::setHp (int newHp){
	hp = newHp;
}

void Player::setMovement (int newMove){
	movement = newMove;
}

void Player::setInitiative (int newInit){
	initiative = newInit;
}

void Player::setSkillCount (int newSC){
	skillCount = newSC;
}

void Player::updateStatus (std::pair<CharStatusId,int> newStatus){
	for(size_t i=0;i<status.size();i++){
		if (newStatus.first == status[i].first){
			status[i].second = newStatus.second;
		}
	}
}

void Player::setStatus(std::vector<std::pair<CharStatusId,int>> newStatus){
	status = newStatus;
}

void Player::setIa (bool IA){
	ia = IA;
}

Character* Player::getCharacter () {
	return character;
}

void Player::setCharacter (Character* newcharacter) {
	character = newcharacter;
	hp = character->getHpBase();
	movement = character->getMovementBase();
	initiative = character->getInitiativeBase();
	skillCount = character->getSkillCountBase();
}

std::vector<Skill*> Player::getSkills (){
	return skills;
}

void Player::setSkills (std::vector<Skill*> newSkills){
	skills=newSkills;
}

void Player::setSkill (Skill* newSkill){
	skills.push_back(newSkill);
}

Player::Player(){
	x=0;
    y=0;
    direction=SOUTH;
    hp=0;
    movement=0;
    initiative=0;
    skillCount=0;
    status={{POISONED,0},{BURNED,0},{FEAR,0},{DAZZLED,0}};
    ia=0;
}
