#include "SkillFactory.h"
#include <tuple>

using namespace state;

//Stat FRAPPE
int damage_FRAPPE=5;
std::pair<int,int> range_FRAPPE=std::make_pair(1,1);
std::vector<std::pair<int,int>> area_FRAPPE={std::make_pair(0,0)};
int precision_FRAPPE=90;
std::vector<std::tuple<EffectId,int,int>> effect_FRAPPE;
int cooldown_FRAPPE=NULL;
bool special_FRAPPE=0;//attaque physique
/*
//Stat SAUTPIQUE
int damage_SAUTPIQUE=8;
std::pair<int,int> range_SAUTPIQUE=std::make_pair(2,3);
std::vector<std::pair<int,int>> area_SAUTPIQUE={std::make_pair(0,0)};
int precision_SAUTPIQUE=30;
std::vector<std::tuple<EffectId,int,int>> effect_SAUTPIQUE
={std::tuple(,,),};
int cooldown_=2;
bool special_=0;
/*
//Stat 
int damage_=;
std::pair<int,int> range_=std::make_pair(,);
std::vector<std::pair<int,int>> area_={std::make_pair(,),};
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_={std::tuple(,,),};
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;

//Stat 
int damage_=;
std::pair<int,int> range_=;
std::vector<std::pair<int,int>> area_=<<0;0>>;
int precision_=;
std::vector<std::tuple<EffectId,int,int>> effect_;
int cooldown_=;
bool special_=;
*/
Skill SkillFactory::createSkill(SkillId id){
	Skill move;
	if (id==SKILL1){
		move.setDamage(damage_FRAPPE);
		move.setRange(range_FRAPPE);
		move.setArea(area_FRAPPE);
		move.setPrecision(precision_FRAPPE);
		move.setEffect(effect_FRAPPE);
		move.setCooldown(cooldown_FRAPPE);
		move.setSpecial(special_FRAPPE);
	}/*else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}else if (id==){
		move.setDamage(damage_);
		move.setRange(range_);
		move.setArea(area_);
		move.setPrecision(precision_);
		move.setEffect(effect_);
		move.setCooldown(cooldown_);
		move.setSpecial(special_);
	}*/
	return move;
}
	
