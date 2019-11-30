#include "Field.h"
#include <iostream>

using namespace state;
using namespace std;

Field::Field(){
	fieldType = PLAIN;
	fieldStatus.push_back({RAIN, 0}); //1
	fieldStatus.push_back({SNOW, 0}); //2
	fieldStatus.push_back({MIST, 0}); //3
	fieldStatus.push_back({BLOCKATTACK, 0}); //4
	fieldStatus.push_back({BLOCKATTACK, 0}); //5
	fieldStatus.push_back({BLOCKATTACK, 0}); //6
	fieldStatus.push_back({BLOCKATTACK, 0}); //7
	fieldStatus.push_back({BLOCKATTACK, 0}); //8
	fieldStatus.push_back({BLOCKATTACK, 0}); //9
	fieldStatus.push_back({BLOCKATTACK, 0}); //10
	fieldStatus.push_back({BLOCKATTACK, 0}); //11
	fieldStatus.push_back({BLOCKMOVE, 0}); //12
	fieldStatus.push_back({BURNING, 0}); //13
	fieldStatus.push_back({POISON, 0}); //14
	fieldStatus.push_back({GRAVITY, 0});//15
	fieldStatus.push_back({FORT, 0});//16
	fieldStatus.push_back({TENT, 0});//17
	fieldStatus.push_back({TOWER, 0});//18
}

FieldTypeId Field::getFieldType(){
	return fieldType;
}

void Field::setFieldType(FieldTypeId newFieldType){
	fieldType = newFieldType;
}

std::vector<std::pair<FieldStatusId,int>> Field::getFieldStatus(){
	return fieldStatus;
}

void Field::setFieldStatus(std::vector<std::pair<FieldStatusId,int>> newFieldStatus){
	fieldStatus = newFieldStatus;
}

void Field::updateFieldStatus(std::pair<FieldStatusId,int> newStatus){
	for(std::pair<FieldStatusId,int>& status : fieldStatus){
		if(newStatus.first == status.first){
			status.second = newStatus.second;
		}
	}
}
