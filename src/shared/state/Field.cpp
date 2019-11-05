#include "Field.h"

using namespace state;
using namespace std;

Field::Field(){
	fieldType = PLAIN;
	fieldStatus.push_back({RAIN, 0});
	fieldStatus.push_back({SNOW, 0});
	fieldStatus.push_back({MIST, 0});
	fieldStatus.push_back({BLOCKATTACK, 0});
	fieldStatus.push_back({BLOCKMOVE, 0});
	fieldStatus.push_back({BURNING, 0});
	fieldStatus.push_back({POISON, 0});
	fieldStatus.push_back({GRAVITY, 0});
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
