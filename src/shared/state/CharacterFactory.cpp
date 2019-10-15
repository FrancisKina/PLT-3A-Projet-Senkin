#include "CharacterFactory.h"

using namespace state;
//skill count égal pour tout le personnage pour l'instant
int skill_count = 2;

//Stat chevalier kiza/ Knight
int HP_Knight = 60;
int MP_Knight = 12;
int initi_Knight = 7;
int atk_Knight = 6;
MovementId MT_Knight = 3;//Monture/riding

//Stat dreaming frog/ frog
int HP_Frog = 45;
int MP_Frog = 6;
int initi_Frog = 8;
int atk_Frog = 8;
MovementId MT_Frog = 4;//nature

//Stat Hope arc/ Archer
int HP_Archer = 30;
int MP_Archer = 10;
int initi_Archer = 5;
int atk_Archer = 5;
MovementId MT_Archer = 2;//agile

//Stat nain des monts/ Dwarf
int HP_Dwarf = 45;
int MP_Dwarf = 6;
int initi_Dwarf = 3;
int atk_Dwarf = 6;
MovementId MT_Dwarf = 1;//normal


Character CharacterFactory::createCharacter(ClassId id){
	Character chara;
	if (id=1){	//classe Knight
		setHpBase(HP_Knight);
		setMovementBase(MP_Knight);
		setSkillCountBase(skill_count);
		setInitiativeBase(initi_Knight);
		setMovementType(MT_Knight);
		setAttack(atk_Knight);
	}else if (id=2){	//classe Frog
		setHpBase(HP_Frog);
		setMovementBase(MP_);
		setSkillCountBase(skill_count);
		setInitiativeBase(initi_Frog);
		setMovementType(MT_Frog);
		setAttack(atk_Frog);
	}else if (id=3){	//classe Archer
		setHpBase(HP_Archer);
		setMovementBase(MP_Archer);
		setSkillCountBase(skill_count);
		setInitiativeBase(initi_Archer);
		setMovementType(MT_Archer);
		setAttack(atk_Archer);
	}else if (id=4){	//classe Dwarf
		setHpBase(HP_);
		setMovementBase(MP_Dwarf);
		setSkillCountBase(skill_count);
		setInitiativeBase(initi_Dwarf);
		setMovementType(MT_Dwarf);
		setAttack(atk_Dwarf);
	}
    return chara;
}
