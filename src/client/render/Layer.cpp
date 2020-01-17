#include "Layer.h"
#include <state.h>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace render;
using namespace state;
using namespace std;


bool Layer::loadInfosBack(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
	
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
		
		// création de tous les quads
		for(size_t i= 0; i<(state.getGrid().size()); i++){
			for(size_t j= 0; j<state.getGrid()[0].size(); j++){
				sf::Vertex* quad = &quads[(i * state.getGrid()[0].size()+ j) * 4];
				
				//Position des 4 coins du quad
				quad[0].position = sf::Vector2f(tileSize * j * 4, tileSize * i * 4);
				quad[1].position = sf::Vector2f(tileSize * (j + 1) * 4, tileSize * i * 4);
				quad[2].position = sf::Vector2f(tileSize * (j + 1) * 4, tileSize * (i + 1) * 4);
				quad[3].position = sf::Vector2f(tileSize * j * 4, tileSize * (i + 1) * 4);
				
				//Position de la texture
				quad[0].texCoords = sf::Vector2f(0, textSize.y);
				quad[1].texCoords = sf::Vector2f(textSize.x * 2, textSize.y);
				quad[2].texCoords = sf::Vector2f(textSize.x * 2, textSize.y * 3);
				quad[3].texCoords = sf::Vector2f(0, textSize.y * 3);
			
			}
		}
				
		return true;				
}

bool Layer::loadField(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
	
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
		
		// création de tous les quads
		for(size_t i=0; i<state.getGrid().size(); i++){
			for(size_t j=0; j<state.getGrid()[0].size(); j++){
				sf::Vertex* quad = &quads[(i * state.getGrid()[0].size()+ j) * 4];
				
				//Position des 4 coins du quad
				quad[0].position = sf::Vector2f(tileSize * j, tileSize * i);
				quad[1].position = sf::Vector2f(tileSize * (j + 1), tileSize * i);
				quad[2].position = sf::Vector2f(tileSize * (j + 1), tileSize * (i + 1));
				quad[3].position = sf::Vector2f(tileSize * j, tileSize * (i + 1));
				
				//Position de la texture
				quad[0].texCoords = sf::Vector2f(textSize.x * (state.getGrid()[i][j]->getFieldType() - 1), 0);
				quad[1].texCoords = sf::Vector2f(textSize.x * state.getGrid()[i][j]->getFieldType(), 0);
				quad[2].texCoords = sf::Vector2f(textSize.x * state.getGrid()[i][j]->getFieldType(), textSize.y);
				quad[3].texCoords = sf::Vector2f(textSize.x * (state.getGrid()[i][j]->getFieldType() - 1), textSize.y);
			
			}
		}
		
				
		return true;				
}

bool Layer::loadFieldEffect(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize, int mode){
	
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
	   	size_t quadsize = 0;
       	quads.resize(quadsize * 4);
		
		unsigned int effectbegin, effectend, shiftbuilding;
		if(mode == 1) effectbegin = 0, effectend = 10, shiftbuilding = 0; //Meteo
		else if (mode == 0) effectbegin = 10, effectend = state.getGrid()[0][0]->getFieldStatus().size(), shiftbuilding = 0 ; //Terrain
		else if (mode == 2) effectbegin = 10, effectend = state.getGrid()[0][0]->getFieldStatus().size(), shiftbuilding = 1 ; //Batiment
		
		// création de tous les quads
		for(size_t i=0; i<state.getGrid().size(); i++){
			for(size_t j=0; j<state.getGrid()[0].size(); j++){
				for(size_t k=effectbegin; k<effectend; k++){
					std::pair<FieldStatusId, int> status = state.getGrid()[i][j]->getFieldStatus()[k];
					if(status.second > 0){
						quadsize++;
						quads.resize(quadsize * 4);
						sf::Vertex* quad = &quads[(quadsize-1) * 4];
						
						//Position des 4 coins du quad
						quad[0].position = sf::Vector2f(tileSize * j, tileSize * (i - shiftbuilding));
						quad[1].position = sf::Vector2f(tileSize * (j + 1), tileSize * (i - shiftbuilding));
						quad[2].position = sf::Vector2f(tileSize * (j + 1), tileSize * (i + 1 - shiftbuilding));
						quad[3].position = sf::Vector2f(tileSize * j, tileSize * (i + 1 - shiftbuilding));
						
						//Position de la texture
						
						quad[0].texCoords = sf::Vector2f(textSize.x * (status.first - 1), textSize.y * shiftbuilding);
						quad[1].texCoords = sf::Vector2f(textSize.x * status.first, textSize.y * shiftbuilding);
						quad[2].texCoords = sf::Vector2f(textSize.x * status.first, textSize.y * (1 + shiftbuilding));
						quad[3].texCoords = sf::Vector2f(textSize.x * (status.first - 1), textSize.y * (1 + shiftbuilding));
					}
				}
			}
		}			
		return true;				
}


bool Layer::loadPlayer(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
    		
		texture = textureTileset;
		clock_t mov = 2;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4 + 4); //Joueurs en jeu + le joueur qui joue dans la partie infos
       	
        // on remplit le tableau de vertex, avec un quad par tuile
        
        for (size_t i = 0; i < width; i++){
              	sf::Vertex* quad = &quads[i * 4];
				
				//Position des 4 coins du quad (x, y)
				quad[0].position = sf::Vector2f(tileSize * state.getPlayers()[i]->getX() + tileSize/6.66, tileSize * state.getPlayers()[i]->getY()); //Haut gauche
				quad[1].position = sf::Vector2f(tileSize * (state.getPlayers()[i]->getX() + 1) - tileSize/6.66, tileSize * state.getPlayers()[i]->getY()); //Haut droite
				quad[2].position = sf::Vector2f(tileSize * (state.getPlayers()[i]->getX() + 1) - tileSize/6.66, tileSize * (state.getPlayers()[i]->getY() + 1) - tileSize/10); //Bas droite
				quad[3].position = sf::Vector2f(tileSize * state.getPlayers()[i]->getX() + tileSize/6.66, tileSize * (state.getPlayers()[i]->getY() + 1) - tileSize/10); //Bas gauche
				
				//Position de la texture;
				
				if(state.getPlayers()[i] == state.getPlaying()){
					if (state.getCommandMode() == SKILLTARGET) mov = clock()%3 + 4;
					else mov = clock()%3 + 1;
				}
				else mov = 2;
				
				int classId = state.getPlayers()[i]->getCharacter()->getClassId() - 1;		
				quad[0].texCoords = sf::Vector2f(textSize.x * (mov - 1 + 9 * classId), (state.getPlayers()[i]->getDirection() - 1) * textSize.y);
				quad[1].texCoords = sf::Vector2f(textSize.x * (mov + 9 * classId), (state.getPlayers()[i]->getDirection() - 1) * textSize.y);
				quad[2].texCoords = sf::Vector2f(textSize.x * (mov + 9 * classId), state.getPlayers()[i]->getDirection() * textSize.y);
				quad[3].texCoords = sf::Vector2f(textSize.x * (mov - 1 + 9 * classId), state.getPlayers()[i]->getDirection() * textSize.y);
		}
		
		//INFOS
		sf::Vertex* quad = &quads[width * height * 4];
		
		//Position des 4 coins du quad (x, y)
		quad[0].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 4) + 3*tileSize/6.66, tileSize); //Haut gauche
		quad[1].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 7) - 3*tileSize/6.66, tileSize); //Haut droite
		quad[2].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 7) - 3*tileSize/6.66, tileSize * 4 - 3*tileSize/10); //Bas droite
		quad[3].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 4) + 3*tileSize/6.66, tileSize * 4 - 3*tileSize/10); //Bas gauche
		
		//Position de la texture;
		int classId = state.getPlaying()->getCharacter()->getClassId() - 1;	
		for(state::Player* player : state.getPlayers()){
			if ((player->getX() == state.getCursor()->getCursorX() && player->getY() == state.getCursor()->getCursorY()) && player!=state.getPlaying()){
				classId = player->getCharacter()->getClassId() - 1;
			}
		}	
		quad[0].texCoords = sf::Vector2f(textSize.x * (mov - 1 + 9 * classId), (state.getPlaying()->getDirection() - 1) * textSize.y);
		quad[1].texCoords = sf::Vector2f(textSize.x * (mov + 9 * classId), (state.getPlaying()->getDirection() - 1) * textSize.y);
		quad[2].texCoords = sf::Vector2f(textSize.x * (mov + 9 * classId), state.getPlaying()->getDirection() * textSize.y);
		quad[3].texCoords = sf::Vector2f(textSize.x * (mov - 1 + 9 * classId), state.getPlaying()->getDirection() * textSize.y);

		return true;
}

bool Layer::loadPlayerEffect(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
    		
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex 
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize((width * height + 1) * 4 * state.getPlayers()[0]->getStatus().size()); // +1 -> Affichage en infos
       	
        // on remplit le tableau de vertex, avec un quad par tuile
        for (size_t i = 0; i < width; i++){
				int it = i * 4 * state.getPlayers()[0]->getStatus().size(); //Décalage vers le ième joueur
				int neffect = 0;
				
				
				for(size_t k=0; k<state.getPlayers()[i]->getStatus().size(); k++){
					sf::Vertex* quad = &quads[it + k * 4];
					std::pair<CharStatusId, int> status = state.getPlayers()[i]->getStatus()[k];
					
					//Position des 4 coins du quad (x, y)
					quad[0].position = sf::Vector2f(tileSize * state.getPlayers()[i]->getX() + 14*tileSize/20, tileSize * state.getPlayers()[i]->getY() + 6*tileSize/20 * neffect); //Haut gauche
					quad[1].position = sf::Vector2f(tileSize * (state.getPlayers()[i]->getX() + 1), tileSize * state.getPlayers()[i]->getY() + 6*tileSize/20 * neffect); //Haut droite
					quad[2].position = sf::Vector2f(tileSize * (state.getPlayers()[i]->getX() + 1), tileSize * (state.getPlayers()[i]->getY() + 1) - 14*tileSize/20 + 6*tileSize/20 * neffect); //Bas droite
					quad[3].position = sf::Vector2f(tileSize * state.getPlayers()[i]->getX() + 14*tileSize/20, tileSize * (state.getPlayers()[i]->getY() + 1) - 14*tileSize/20 + 6*tileSize/20 * neffect); //Bas gauche
					
					//Position de la texture
					if(status.second > 0){
						quad[0].texCoords = sf::Vector2f(textSize.x * (status.first - 1), 0);
						quad[1].texCoords = sf::Vector2f(textSize.x * status.first, 0);
						quad[2].texCoords = sf::Vector2f(textSize.x * status.first, textSize.y);
						quad[3].texCoords = sf::Vector2f(textSize.x * (status.first - 1), textSize.y);
						neffect++;
					}
				}
		}
		
		//INFOS
		int it = width * 4 * state.getPlaying()->getStatus().size();
		int neffect = 0;
		for(size_t k=0; k<state.getPlaying()->getStatus().size(); k++){
			sf::Vertex* quad = &quads[it + k * 4];
			std::pair<CharStatusId, int> status = state.getPlaying()->getStatus()[k];
			
			//Position des 4 coins du quad (x, y)
			quad[0].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 4.5) + 3*14*tileSize/20, tileSize + 3*6*tileSize/20 * neffect); //Haut gauche
			quad[1].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 7.5), tileSize + 3*6*tileSize/20 * neffect); //Haut droite
			quad[2].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 7.5), tileSize * 4 - 3*14*tileSize/20 + 3*6*tileSize/20 * neffect); //Bas droite
			quad[3].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 4.5) + 3*14*tileSize/20, tileSize * 4 - 3*14*tileSize/20 + 3*6*tileSize/20 * neffect); //Bas gauche
			
			//Position de la texture
			if(status.second > 0){
				quad[0].texCoords = sf::Vector2f(textSize.x * (status.first - 1), 0);
				quad[1].texCoords = sf::Vector2f(textSize.x * status.first, 0);
				quad[2].texCoords = sf::Vector2f(textSize.x * status.first, textSize.y);
				quad[3].texCoords = sf::Vector2f(textSize.x * (status.first - 1), textSize.y);
				neffect++;
			}
		}

		return true;
}

bool Layer::loadCursor(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){
    			
		texture = textureTileset;
		
      	// on redimensionne le tableau de vertex pour qu'il puisse contenir tout le niveau
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(width * height * 4);
       	
        // on remplit le tableau de vertex, avec un quad par tuile
		sf::Vertex* quad = &quads[0];
		
		//Position des 4 coins du quad (x, y)
		quad[0].position = sf::Vector2f(tileSize * state.getCursor()->getCursorX(), tileSize * state.getCursor()->getCursorY()); //Haut gauche
		quad[1].position = sf::Vector2f(tileSize * (state.getCursor()->getCursorX() + 1), tileSize * state.getCursor()->getCursorY()); //Haut droite
		quad[2].position = sf::Vector2f(tileSize * (state.getCursor()->getCursorX() + 1), tileSize * (state.getCursor()->getCursorY() + 1)); //Bas droite
		quad[3].position = sf::Vector2f(tileSize * state.getCursor()->getCursorX(), tileSize * (state.getCursor()->getCursorY() + 1)); //Bas gauche
		
		//Position de la texture;
		quad[0].texCoords = sf::Vector2f(0, 0);
		quad[1].texCoords = sf::Vector2f(textSize.x, 0);
		quad[2].texCoords = sf::Vector2f(textSize.x, textSize.y);
		quad[3].texCoords = sf::Vector2f(0, textSize.y);
		
		if (state.getCommandMode() == COMMAND || state.getCommandMode() == SKILL || state.getCommandMode() == SKILLTARGET){
			//CURSEUR INFO
			quad = &quads[4];
			
			//Position des 4 coins du quad (x, y)
			quad[0].position = sf::Vector2f(tileSize * (state.getGrid()[0].size()), tileSize * (state.getCursorInfo()->getCursorY() - 0.25)); //Haut gauche
			quad[1].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 8), tileSize * (state.getCursorInfo()->getCursorY() - 0.25)); //Haut droite
			quad[2].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + 8), tileSize * (state.getCursorInfo()->getCursorY()+0.75)); //Bas droite
			quad[3].position = sf::Vector2f(tileSize * (state.getGrid()[0].size()), tileSize * (state.getCursorInfo()->getCursorY()+0.75)); //Bas gauche
			
			//Position de la texture;
			quad[0].texCoords = sf::Vector2f(0, textSize.y);
			quad[1].texCoords = sf::Vector2f(textSize.x * 8, textSize.y);
			quad[2].texCoords = sf::Vector2f(textSize.x * 8, textSize.y * 2);
			quad[3].texCoords = sf::Vector2f(0, textSize.y * 2);
		}	
		
		//Curseur skill
		if (state.getCommandMode() == SKILLTARGET){
			state::Player* playing = state.getPlaying();
			state::Skill* skill = playing->getSkills()[state.getCursorInfo()->getCursorY() - 6];
			
			quads.resize(width * height * 4 + (skill->getRange().second - skill->getRange().first + 1) * 4 * 4);
			
			int dx,dy;
			for(int range = skill->getRange().first; range <= skill->getRange().second; range++){
				for(int d = 0; d < 4; d++){
					if (d==0){
						dx = range;
						dy = 0;
					}
					else if (d==1){
						dx = -range;
						dy = 0;
					}
					else if (d==2){
						dx = 0;
						dy = range;
					}
					else if (d==3){
						dx = 0;
						dy = -range;
					}
				
					quad = &quads[8 + ((range - skill->getRange().first) * 4 + d) * 4];
					
					//Position des 4 coins du quad (x, y)
					quad[0].position = sf::Vector2f(tileSize * (state.getPlaying()->getX() + dx), tileSize * (state.getPlaying()->getY() + dy)); //Haut gauche
					quad[1].position = sf::Vector2f(tileSize * (state.getPlaying()->getX() + dx + 1), tileSize * (state.getPlaying()->getY() + dy)); //Haut droite
					quad[2].position = sf::Vector2f(tileSize * (state.getPlaying()->getX() + dx + 1), tileSize * (state.getPlaying()->getY() + dy + 1)); //Bas droite
					quad[3].position = sf::Vector2f(tileSize * (state.getPlaying()->getX() + dx), tileSize * (state.getPlaying()->getY() + dy + 1)); //Bas gauche
					
					//Position de la texture;
					quad[0].texCoords = sf::Vector2f(textSize.x, 0);
					quad[1].texCoords = sf::Vector2f(textSize.x * 2, 0);
					quad[2].texCoords = sf::Vector2f(textSize.x * 2, textSize.y);
					quad[3].texCoords = sf::Vector2f(textSize.x, textSize.y);
				}
			}
		}

		return true;
}

bool Layer::loadInfos(state::State& state, sf::Texture& textureTileset, sf::Vector2u textSize, unsigned int width, unsigned int height, int tileSize){

		texture = textureTileset;
		state::Player* player=state.getPlaying();
		//info personnage
		std::vector<std::string> infos((state.getGrid().size() + state.getTextInfo().size())*2 , " ");
		for(state::Player* foe : state.getPlayers()){
			if ((foe->getX() == state.getCursor()->getCursorX() && foe->getY() == state.getCursor()->getCursorY()) && foe!=state.getPlaying()){
				player=foe;
			}
		}
		infos[2] += "PV " + to_string(player->getHp());
		infos[4] += "PM " + to_string(player->getMovement());
		infos[6] += "PA " + to_string(player->getSkillCount());
		//info attaques
		infos[10] += "SKILLS";
		size_t skills_size = player->getSkills().size();
		infos[12] += player->getSkills()[0]->getName();
		if(player->getSkills()[0]->getCooldown()>0) infos[12] += " " + to_string(player->getSkills()[0]->getCooldown());
		if(skills_size>1){
			infos[14] += player->getSkills()[1]->getName();
			if(player->getSkills()[1]->getCooldown()>0) infos[14] += " " + to_string(player->getSkills()[1]->getCooldown());
			if(skills_size>2){
				infos[16] += player->getSkills()[2]->getName();
				if(player->getSkills()[2]->getCooldown()>0) infos[16] += " " + to_string(player->getSkills()[2]->getCooldown());
				if(skills_size>3){
					infos[18] += player->getSkills()[3]->getName();
					if(player->getSkills()[3]->getCooldown()>0) infos[18] += " " + to_string(player->getSkills()[3]->getCooldown());
				}
			}
		}
		
		//info terrain
		infos[22] += "TERRAIN";
		std::vector<std::string> fieldName = {"","PLAINE","ROUTE","FORET","MONTAGNE","EAU","SABLE","VILLE","MARECAGE","MUR"};
		std::vector<std::string> fieldStatusName = {"","PLUIE","NEIGE","BRUME","","","","","","","","BLOQUE ATT","BLOQUE MOUV","BRULE","POISON","GRAVITE","","","","","FORT","TENTE","TOUR"};
		state::Field* field = state.getGrid()[state.getCursor()->getCursorY()][state.getCursor()->getCursorX()];
		infos[24] += fieldName[field->getFieldType()];
		int k=0;
		for(size_t i=0; i < field->getFieldStatus().size(); i++){
			if (field->getFieldStatus()[i].second > 0){
				infos[26 + 2*k] += fieldStatusName[field->getFieldStatus()[i].first];
				if (field->getFieldStatus()[i].second < 100){
					infos[26 + 2*k] += " " + to_string(field->getFieldStatus()[i].second);
				}
				k++;
			}
		}
		
		
		//info jeu
		infos[40] += "ROUND " + to_string(state.getRound());
		
		if(state.getCommandMode() == COMMAND){
			infos[42] += "SE DEPLACER ";
			infos[44] += "ATTAQUER ";
			infos[46] += "FIN DE TOUR ";
		}
		
		//Texte Infos
		for(int i = 0; i<(int)state.getTextInfo().size(); i++){
			infos[state.getGrid().size()*2 + 1 + i * 2] = state.getTextInfo()[i];
		}
		
		
		//Nombre de caracteres a afficher
		unsigned int quadsize = 0;
		for(std::string info : infos){
			quadsize+=info.size();
		}
		
	   	quads.setPrimitiveType(sf::Quads);
       	quads.resize(quadsize * 4);
		
		// création de tous les quads
		quadsize = 0;
		for(size_t i=0; i<infos.size(); i++){
			for(size_t j=0; j<infos[i].size(); j++){
				sf::Vertex* quad = &quads[(quadsize+j) * 4];
				
				if (i <= (state.getGrid().size() * 2)){
				//Position des 4 coins du quad
					quad[0].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + j*0.5), tileSize*0.5 * i);
					quad[1].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + (j + 1)*0.5), tileSize*0.5 * i);
					quad[2].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + (j + 1)*0.5), tileSize*0.5 * (i + 1));
					quad[3].position = sf::Vector2f(tileSize * (state.getGrid()[0].size() + j*0.5), tileSize*0.5 * (i + 1));
				}
				else{
					quad[0].position = sf::Vector2f(tileSize * (1 + j*0.5), tileSize*0.5 * i);
					quad[1].position = sf::Vector2f(tileSize * (1 + (j + 1)*0.5), tileSize*0.5 * i);
					quad[2].position = sf::Vector2f(tileSize * (1 + (j + 1)*0.5), tileSize*0.5 * (i + 1));
					quad[3].position = sf::Vector2f(tileSize * (1 + j*0.5), tileSize*0.5 * (i + 1));
				}
				
				//Position de la texture
				int textpos;
				if(infos[i][j]>=int('A') && infos[i][j]<=int('Z')) textpos = infos[i][j] - int('A');
				else if (infos[i][j]>=int('0') && infos[i][j]<=int('9')) textpos = 26 + infos[i][j] - int('0');
				else textpos = -1;
				quad[0].texCoords = sf::Vector2f(textSize.x * textpos, 0);
				quad[1].texCoords = sf::Vector2f(textSize.x * (textpos+1), 0);
				quad[2].texCoords = sf::Vector2f(textSize.x * (textpos+1), textSize.y);
				quad[3].texCoords = sf::Vector2f(textSize.x * textpos, textSize.y);

			}
			quadsize+=infos[i].size();
			if (quadsize * 4 >= quads.getVertexCount()) break;
		}			
		return true;
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // on applique la transformation
        states.transform *= getTransform();

        // on applique la texture du tileset
        states.texture = &texture;

        // et on dessine enfin le tableau de vertex
        target.draw(quads, states);
}

sf::VertexArray Layer::getQuads(){
	return quads;
}

