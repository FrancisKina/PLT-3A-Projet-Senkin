#include "Character.h"

using namespace state;

int Character::getHpBase(){
    return hpBase;
}

int Character::getMovementBase(){
    return movementBase;
}

MovementId Character::getMovementType(){
    return movementType;
}

int Character::getInitiativeBase(){
    return initiativeBase;
}

int Character::getSkillCountBase(){
    return skillCountBase;
}

void Character::setHpBase(int new_hpBase){
    hpBase=new_hpBase;
}

void Character::setMovementBase(int new_movementBase){
    movementBase=new_movementBase;
}

void Character::setMovementType(MovementId new_movement_type){
    movementType=new_movement_type;
}

void Character::setInitiativeBase(int new_initiativeBase){
    initiativeBase=new_initiativeBase;
}

void Character::setSkillCountBase(int new_skillCountBase){
    skillCountBase=new_skillCountBase;
}
