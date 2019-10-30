#include "CharacterFactory.h"

using namespace state;
//skill count égal pour tout le personnage pour l'instant
int skill_count = 2;

//Stat chevalier kiza/ Knight
int HP_Knight = 60;
int MP_Knight = 12;
int initi_Knight = 7;
int atk_Knight = 6;
MovementId MT_Knight = RIDING;//Monture
ClassId class_Knight = KNIGHT;

//Stat dreaming frog/ frog
int HP_Frog = 45;
int MP_Frog = 6;
int initi_Frog = 8;
int atk_Frog = 8;
MovementId MT_Frog = NATURE;
ClassId class_Frog = FROG;

//Stat Hope arc/ Archer
int HP_Archer = 30;
int MP_Archer = 10;
int initi_Archer = 5;
int atk_Archer = 5;
MovementId MT_Archer = AGILE;
ClassId class_Archer = ARCHER;

//Stat nain des monts/ Dwarf
int HP_Dwarf = 45;
int MP_Dwarf = 6;
int initi_Dwarf = 3;
int atk_Dwarf = 6;
MovementId MT_Dwarf = NORMAL;
ClassId class_Dwarf = DWARF;


Character* CharacterFactory::createCharacter(ClassId id){
	Character* chara = new Character();
	
	if (id==class_Knight){	//classe Knight
		chara->setHpBase(HP_Knight);
		chara->setMovementBase(MP_Knight);
		chara->setSkillCountBase(skill_count);
		chara->setInitiativeBase(initi_Knight);
		chara->setMovementType(MT_Knight);
		chara->setAttackBase(atk_Knight);
		chara->setClassId(id);
		
	}else if (id==class_Frog){	//classe Frog
		chara->setHpBase(HP_Frog);
		chara->setMovementBase(MP_Frog);
		chara->setSkillCountBase(skill_count);
		chara->setInitiativeBase(initi_Frog);
		chara->setMovementType(MT_Frog);
		chara->setAttackBase(atk_Frog);
		chara->setClassId(id);
		
	}else if (id==class_Archer){	//classe Archer
		chara->setHpBase(HP_Archer);
		chara->setMovementBase(MP_Archer);
		chara->setSkillCountBase(skill_count);
		chara->setInitiativeBase(initi_Archer);
		chara->setMovementType(MT_Archer);
		chara->setAttackBase(atk_Archer);
		chara->setClassId(id);
		
	}else if (id==class_Dwarf){	//classe Dwarf
		chara->setHpBase(HP_Dwarf);
		chara->setMovementBase(MP_Dwarf);
		chara->setSkillCountBase(skill_count);
		chara->setInitiativeBase(initi_Dwarf);
		chara->setMovementType(MT_Dwarf);
		chara->setAttackBase(atk_Dwarf);
		chara->setClassId(id);
		
	}
    return chara;
}
