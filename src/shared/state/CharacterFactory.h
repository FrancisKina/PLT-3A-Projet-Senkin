// Generated by dia2code
#ifndef STATE__CHARACTERFACTORY__H
#define STATE__CHARACTERFACTORY__H


namespace state {
  class Character;
}

#include "Character.h"
#include "ClassId.h"

namespace state {

  /// class CharacterFactory - 
  class CharacterFactory {
    // Associations
    // Operations
  public:
    Character createCharacter (ClassId id);
    // Setters and Getters
  };

};

#endif