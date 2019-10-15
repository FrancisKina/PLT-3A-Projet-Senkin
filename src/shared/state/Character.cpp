#include "Character.h"

using namespace state;

int Character::getHpBase(){
    return hpBase;
}

int Character::getMovementBase(){
    return movementBase;
}

MovementId Character::getMovementType(){
    return movement_type;
}

int Character::getInitiativeBase(){
    return initiativeBase;
}

int Character::getSkillCountBase(){
    return skillCountBase;
}

int Character::setHpBase(int new_hpBase){
    hpBase=new_hpBase;
}

int Character::setMovementBase(int new_movementBase){
    movementBase=new_movementBase;
}

MovementId Character::setMovementType(MovementId new_movement_type){
    movement_type=new_movement_type;
}

int Character::setInitiativeBase(int new_initiativeBase){
    initiativeBase=new_initiativeBase;
}

int Character::setSkillCountBase(int new_skillCountBase){
    skillCountBase=new_skillCountBase;
}
