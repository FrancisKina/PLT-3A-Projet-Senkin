#include "SkillFactory.h"
#include <tuple>

using namespace state;

//type de zone d'application : decrit dans le cas d'une attque vers le heut
//CROIX ALLONGÉ
std::vector<std::pair<int,int>> area_type_long_cross={std::make_pair(0,0),std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1),std::make_pair(0,2)};

//CROIX
std::vector<std::pair<int,int>> area_type_cross={std::make_pair(0,0),std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};

//LIGNE 5 CASES
std::vector<std::pair<int,int>> area_type_line5{std::make_pair(0,-2),std::make_pair(0,-1),std::make_pair(0,0),std::make_pair(0,1),std::make_pair(0,2)};

//Stat FRAPPE
int damage_FRAPPE=5;
std::pair<int,int> range_FRAPPE=std::make_pair(1,1);
std::vector<std::pair<int,int>> area_FRAPPE={std::make_pair(0,0)};
int precision_FRAPPE=95;
std::vector<std::tuple<EffectId,int,int>> effect_FRAPPE={};
int cooldown_FRAPPE=-1;//<0 donc peut etre lancé plusieur fois le meme tour
bool special_FRAPPE=0;//attaque physique

//Stat SAUTPIQUE
int damage_SAUTPIQUE=9;
std::pair<int,int> range_SAUTPIQUE=std::make_pair(2,3);
std::vector<std::pair<int,int>> area_SAUTPIQUE={std::make_pair(0,0)};
int precision_SAUTPIQUE=30;
std::vector<std::tuple<EffectId,int,int>> effect_SAUTPIQUE={std::make_tuple(MOVE_USER,0,100),std::make_tuple(MOVE_FOE,0,100)};
int cooldown_SAUTPIQUE=2;
bool special_SAUTPIQUE=0;

//Stat ARC
int damage_ARC=5;
std::pair<int,int> range_ARC=std::make_pair(2,5);
std::vector<std::pair<int,int>> area_ARC={std::make_pair(0,0)};
int precision_ARC=90;
std::vector<std::tuple<EffectId,int,int>> effect_ARC={};
int cooldown_ARC=0;
bool special_ARC=0;

//Stat ARC_EN_CIEL
int damage_ARC_EN_CIEL=7;
std::pair<int,int> range_ARC_EN_CIEL=std::make_pair(3,3);
std::vector<std::pair<int,int>> area_ARC_EN_CIEL=area_type_line5;
int precision_ARC_EN_CIEL=90;
std::vector<std::tuple<EffectId,int,int>> effect_ARC_EN_CIEL={std::make_tuple(BOOST_ATK_RAIN,0,100)};
int cooldown_ARC_EN_CIEL=2;
bool special_ARC_EN_CIEL=0;

//Stat SOIN
int damage_SOIN=0;
std::pair<int,int> range_SOIN=std::make_pair(0,1);
std::vector<std::pair<int,int>> area_SOIN={std::make_pair(0,0)};
int precision_SOIN=80;
std::vector<std::tuple<EffectId,int,int>> effect_SOIN={std::make_tuple(HEAL_MEDIUM,0,100),std::make_tuple(HEAL_STATUS,0,50)};
int cooldown_SOIN=0;
bool special_SOIN=1;

//Stat SOIN_LEGER
int damage_SOIN_LEGER=0;
std::pair<int,int> range_SOIN_LEGER=std::make_pair(0,1);
std::vector<std::pair<int,int>> area_SOIN_LEGER={std::make_pair(0,0)};
int precision_SOIN_LEGER=75;
std::vector<std::tuple<EffectId,int,int>> effect_SOIN_LEGER={std::make_tuple(HEAL_LOW,0,100),std::make_tuple(HEAL_STATUS,0,30)};
int cooldown_SOIN_LEGER=-1;
bool special_SOIN_LEGER=1;

//Stat ENTAILLE
int damage_ENTAILLE=5;
std::pair<int,int> range_ENTAILLE=std::make_pair(1,1);
std::vector<std::pair<int,int>> area_ENTAILLE={std::make_pair(0,0)};
int precision_ENTAILLE=90;
std::vector<std::tuple<EffectId,int,int>> effect_ENTAILLE={};
int cooldown_ENTAILLE=0;
bool special_ENTAILLE=0;

//Stat TRANCHARGE
int damage_TRANCHARGE=6;
std::pair<int,int> range_TRANCHARGE=std::make_pair(1,1);
std::vector<std::pair<int,int>> area_TRANCHARGE={std::make_pair(0,0)};
int precision_TRANCHARGE=80;
std::vector<std::tuple<EffectId,int,int>> effect_TRANCHARGE={};
int cooldown_TRANCHARGE=0;
bool special_TRANCHARGE=0;

//Stat PIETINEMENT
int damage_PIETINEMENT=4;
std::pair<int,int> range_PIETINEMENT=std::make_pair(1,1);
std::vector<std::pair<int,int>> area_PIETINEMENT={std::make_pair(0,0)};
int precision_PIETINEMENT=90;
std::vector<std::tuple<EffectId,int,int>> effect_PIETINEMENT={std::make_tuple(FEAR_CHAR,1,50)};
int cooldown_PIETINEMENT=0;
bool special_PIETINEMENT=0;

//Stat FEU_D_ENFER
int damage_FEU_D_ENFER=8;
std::pair<int,int> range_FEU_D_ENFER=std::make_pair(2,4);
std::vector<std::pair<int,int>> area_FEU_D_ENFER=area_type_long_cross;
int precision_FEU_D_ENFER=80;
std::vector<std::tuple<EffectId,int,int>> effect_FEU_D_ENFER={std::make_tuple(BURN_FIELD,3,95),std::make_tuple(BURN_CHAR,3,75)};
int cooldown_FEU_D_ENFER=3;
bool special_FEU_D_ENFER=1;

//Stat DOUBLE_LAME
int damage_DOUBLE_LAME=5;
std::pair<int,int> range_DOUBLE_LAME=std::make_pair(0,0);
std::vector<std::pair<int,int>> area_DOUBLE_LAME=area_type_cross;
int precision_DOUBLE_LAME=65;
std::vector<std::tuple<EffectId,int,int>> effect_DOUBLE_LAME={};
int cooldown_DOUBLE_LAME=1;
bool special_DOUBLE_LAME=0;

//Stat FOUDRE
int damage_FOUDRE=6;
std::pair<int,int> range_FOUDRE=std::make_pair(2,5);
std::vector<std::pair<int,int>> area_FOUDRE={std::make_pair(0,0)};
int precision_FOUDRE=75;
std::vector<std::tuple<EffectId,int,int>> effect_FOUDRE={std::make_tuple(DAZZLE_CHAR,2,50),std::make_tuple(BURN_FIELD,3,3),std::make_tuple(BOOST_ATK_RAIN,0,50)};
int cooldown_FOUDRE=1;
bool special_FOUDRE=1;

//Stat BOULE_DE_FEU
int damage_BOULE_DE_FEU=6;
std::pair<int,int> range_BOULE_DE_FEU=std::make_pair(1,3);
std::vector<std::pair<int,int>> area_BOULE_DE_FEU={std::make_pair(0,0)};
int precision_BOULE_DE_FEU=60;
std::vector<std::tuple<EffectId,int,int>> effect_BOULE_DE_FEU={std::make_tuple(BURN_CHAR,2,95)};
int cooldown_BOULE_DE_FEU=0;
bool special_BOULE_DE_FEU=1;

//Stat LANCER_DE_HACHE
int damage_LANCER_DE_HACHE=7;
std::pair<int,int> range_LANCER_DE_HACHE=std::make_pair(2,2);
std::vector<std::pair<int,int>> area_LANCER_DE_HACHE={std::make_pair(0,0)};
int precision_LANCER_DE_HACHE=40;
std::vector<std::tuple<EffectId,int,int>> effect_LANCER_DE_HACHE={};
int cooldown_LANCER_DE_HACHE=-1;
bool special_LANCER_DE_HACHE=0;

//Stat COUP_DE_BOULE
int damage_COUP_DE_BOULE=5;
std::pair<int,int> range_COUP_DE_BOULE=std::make_pair(1,1);
std::vector<std::pair<int,int>> area_COUP_DE_BOULE={std::make_pair(0,0)};
int precision_COUP_DE_BOULE=30;
std::vector<std::tuple<EffectId,int,int>> effect_COUP_DE_BOULE={std::make_tuple(FEAR_CHAR,1,90)};
int cooldown_COUP_DE_BOULE=0;
bool special_COUP_DE_BOULE=0;

Skill SkillFactory::createSkill(SkillId id){
	Skill move;
	if (id==FRAPPE){
		move.setDamage(damage_FRAPPE);
		move.setRange(range_FRAPPE);
		move.setArea(area_FRAPPE);
		move.setPrecision(precision_FRAPPE);
		move.setEffect(effect_FRAPPE);
		move.setCooldown(cooldown_FRAPPE);
		move.setSpecial(special_FRAPPE);
	}else if (id==SAUT_PIQUE){
		move.setDamage(damage_SAUTPIQUE);
		move.setRange(range_SAUTPIQUE);
		move.setArea(area_SAUTPIQUE);
		move.setPrecision(precision_SAUTPIQUE);
		move.setEffect(effect_SAUTPIQUE);
		move.setCooldown(cooldown_SAUTPIQUE);
		move.setSpecial(special_SAUTPIQUE);
	}
	else if (id==ARC){
		move.setDamage(damage_ARC);
		move.setRange(range_ARC);
		move.setArea(area_ARC);
		move.setPrecision(precision_ARC);
		move.setEffect(effect_ARC);
		move.setCooldown(cooldown_ARC);
		move.setSpecial(special_ARC);
	}else if (id==ARC_EN_CIEL){
		move.setDamage(damage_ARC_EN_CIEL);
		move.setRange(range_ARC_EN_CIEL);
		move.setArea(area_ARC_EN_CIEL);
		move.setPrecision(precision_ARC_EN_CIEL);
		move.setEffect(effect_ARC_EN_CIEL);
		move.setCooldown(cooldown_ARC_EN_CIEL);
		move.setSpecial(special_ARC_EN_CIEL);
	}else if (id==SOIN){
		move.setDamage(damage_SOIN);
		move.setRange(range_SOIN);
		move.setArea(area_SOIN);
		move.setPrecision(precision_SOIN);
		move.setEffect(effect_SOIN);
		move.setCooldown(cooldown_SOIN);
		move.setSpecial(special_SOIN);
	}else if (id==SOIN_LEGER){
		move.setDamage(damage_SOIN_LEGER);
		move.setRange(range_SOIN_LEGER);
		move.setArea(area_SOIN_LEGER);
		move.setPrecision(precision_SOIN_LEGER);
		move.setEffect(effect_SOIN_LEGER);
		move.setCooldown(cooldown_SOIN_LEGER);
		move.setSpecial(special_SOIN_LEGER);
	}else if (id==ENTAILLE){
		move.setDamage(damage_ENTAILLE);
		move.setRange(range_ENTAILLE);
		move.setArea(area_ENTAILLE);
		move.setPrecision(precision_ENTAILLE);
		move.setEffect(effect_ENTAILLE);
		move.setCooldown(cooldown_ENTAILLE);
		move.setSpecial(special_ENTAILLE);
	}else if (id==TRANCHARGE){
		move.setDamage(damage_TRANCHARGE);
		move.setRange(range_TRANCHARGE);
		move.setArea(area_TRANCHARGE);
		move.setPrecision(precision_TRANCHARGE);
		move.setEffect(effect_TRANCHARGE);
		move.setCooldown(cooldown_TRANCHARGE);
		move.setSpecial(special_TRANCHARGE);
	}else if (id==PIETINEMENT){
		move.setDamage(damage_PIETINEMENT);
		move.setRange(range_PIETINEMENT);
		move.setArea(area_PIETINEMENT);
		move.setPrecision(precision_PIETINEMENT);
		move.setEffect(effect_PIETINEMENT);
		move.setCooldown(cooldown_PIETINEMENT);
		move.setSpecial(special_PIETINEMENT);
	}else if (id==FEU_D_ENFER){
		move.setDamage(damage_FEU_D_ENFER);
		move.setRange(range_FEU_D_ENFER);
		move.setArea(area_FEU_D_ENFER);
		move.setPrecision(precision_FEU_D_ENFER);
		move.setEffect(effect_FEU_D_ENFER);
		move.setCooldown(cooldown_FEU_D_ENFER);
		move.setSpecial(special_FEU_D_ENFER);
	}else if (id==DOUBLE_LAME){
		move.setDamage(damage_DOUBLE_LAME);
		move.setRange(range_DOUBLE_LAME);
		move.setArea(area_DOUBLE_LAME);
		move.setPrecision(precision_DOUBLE_LAME);
		move.setEffect(effect_DOUBLE_LAME);
		move.setCooldown(cooldown_DOUBLE_LAME);
		move.setSpecial(special_DOUBLE_LAME);
	}else if (id==FOUDRE){
		move.setDamage(damage_FOUDRE);
		move.setRange(range_FOUDRE);
		move.setArea(area_FOUDRE);
		move.setPrecision(precision_FOUDRE);
		move.setEffect(effect_FOUDRE);
		move.setCooldown(cooldown_FOUDRE);
		move.setSpecial(special_FOUDRE);
	}else if (id==BOULE_DE_FEU){
		move.setDamage(damage_BOULE_DE_FEU);
		move.setRange(range_BOULE_DE_FEU);
		move.setArea(area_BOULE_DE_FEU);
		move.setPrecision(precision_BOULE_DE_FEU);
		move.setEffect(effect_BOULE_DE_FEU);
		move.setCooldown(cooldown_BOULE_DE_FEU);
		move.setSpecial(special_BOULE_DE_FEU);
	}else if (id==LANCER_DE_HACHE){
		move.setDamage(damage_LANCER_DE_HACHE);
		move.setRange(range_LANCER_DE_HACHE);
		move.setArea(area_LANCER_DE_HACHE);
		move.setPrecision(precision_LANCER_DE_HACHE);
		move.setEffect(effect_LANCER_DE_HACHE);
		move.setCooldown(cooldown_LANCER_DE_HACHE);
		move.setSpecial(special_LANCER_DE_HACHE);
	}else if (id==COUP_DE_BOULE){
		move.setDamage(damage_COUP_DE_BOULE);
		move.setRange(range_COUP_DE_BOULE);
		move.setArea(area_COUP_DE_BOULE);
		move.setPrecision(precision_COUP_DE_BOULE);
		move.setEffect(effect_COUP_DE_BOULE);
		move.setCooldown(cooldown_COUP_DE_BOULE);
		move.setSpecial(special_COUP_DE_BOULE);
	}
	return move;
}
	
