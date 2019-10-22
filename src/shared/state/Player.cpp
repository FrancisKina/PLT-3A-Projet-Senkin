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

int Player::getPlayerId(){
    return playerId;
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

void Player::setPlayerId (int newId){
	playerId = newId;
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

void Player::addStatus (std::pair<CharStatusId,int> newStatus){
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
	return classPlayer;
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
    status={};
    playerId=0;
    ia=0;
}
