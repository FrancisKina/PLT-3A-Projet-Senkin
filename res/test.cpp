#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string.h>
#include "../src/shared/state.h"

using namespace std;
using namespace state;
void initGrid(std::string);
std::vector<std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>>> grid;
std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>> cases;
std::pair<FieldTypeId, std::pair<FieldStatusId,int>> stat;

int main(int argc,char* argv[]){
	initGrid("maptest.txt");
	return 0;
}

void initGrid(std::string map_txt){
	std::ifstream fichier(map_txt, ios::in);
	std::string strnombre, ligne;
	int nombre, ncolonne = 0, nligne = 0;
	
	// Lecture Fichier
    if (fichier){
    	while (getline(fichier, ligne)){
			for(size_t i = 0; i < ligne.size();i++){
				if(isdigit(ligne.at(i)) != 0){
					strnombre+=ligne.at(i);
				}
				else{
					stringstream ssnombre(strnombre);
					ssnombre >> nombre;
					//std::cout << nombre << " ";
					//grid[nligne][ncolonne].first
					stat.first = static_cast<FieldTypeId>(nombre);
					std::cout << stat.first << "";
					cases.push_back(stat);
					std::cout << cases[ncolonne].first << " ";
					//std::cout << ncolonne << "c ";
					ncolonne++;
					i++;
					strnombre = "";
				}
			}
			std::cout << "\n";
			//std::cout << cases[4].first << " ";
			grid.push_back(cases);
			//std::cout << grid[nligne][1].first << " ";
			nligne++;
			cases = {};
			ncolonne = 0;
			strnombre = "";
		}
		fichier.close();
		
		for(size_t i = 0; i < 8;i++){
			for(size_t j = 0; j < 16;j++){
				std::cout << grid[i][j].first << " ";
			}
			std::cout << "\n";
		}
		std::cout << grid[0].size();
		//std::cout << grid[7][1].first << " ";
    }
    else {return;}
}
