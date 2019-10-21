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

int Character::getAttackBase(){
    return attackBase;
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

void Character::setAttackBase(int new_attackBase){
    attackBase=new_attackBase;
}

void Character::setClassId(ClassId new_classId){
    classPlayer=new_classId;
}

ClassId Character::getClassId(){
	return classPlayer;
}

Character::Character(){
	hpBase=0;
    movementBase=0;
    skillCountBase=0;
    initiativeBase=0;
    movementType=NORMAL;
    attackBase=0;
    classPlayer=DWARF;
}
