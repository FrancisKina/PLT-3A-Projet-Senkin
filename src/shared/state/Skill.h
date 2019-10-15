// Generated by dia2code
#ifndef STATE__SKILL__H
#define STATE__SKILL__H

#include <vector>

namespace state {
  class SkillFactory;
}

#include "CharStatusId.h"
#include "SkillFactory.h"

namespace state {

  /// class Skill - 
  class Skill {
    // Associations
    // Attributes
  private:
    int damage;
    int range;
    std::vector<std::pair<int,int>> area;
    int precision;
    std::vector<std::tuple<CharStatusId,int,int>> effect;
    int cooldown;
    bool special;
    // Operations
  public:
    int getDamage ();
    int getRange ();
    std::vector<std::pair<int,int>> getArea ();
    std::vector<std::tuple<CharStatusId,int,int>> getEffect ();
    int getCooldown ();
    bool getSpecial ();
    // Setters and Getters
  };

};

#endif