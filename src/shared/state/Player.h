// Generated by dia2code
#ifndef STATE__PLAYER__H
#define STATE__PLAYER__H

#include <vector>

namespace state {
  class Skill;
}

#include "DirectionId.h"
#include "Skill.h"
#include "CharStatusId.h"

namespace state {

  /// class Player - 
  class Player {
    // Associations
    // Attributes
  private:
    bool Ia;
  protected:
    int x;
    int y;
    int PlayerId;
    int hp;
    int movement;
    int initiative;
    int skillCount;
    std::vector<std::pair<CharStatusId,int>> status;
    // Operations
  public:
    int getX ();
    int getY ();
    DirectionId getDirectionId ();
    int getPlayerId ();
    int getHp ();
    int getMovement ();
    int getInitiative ();
    int getSkillCount ();
    std::vector<std::pair<CharStatusId,int>> getStatus ();
    bool getIa ();
    void setX (int newX);
    void setY (int newY);
    void setDirectionId (DirectionId id);
    void setPlayerId (int newId);
    void setHp (int newHp);
    void setMovement (int newMove);
    void setInitiative (int newInit);
    void setSkillCount (int newSC);
    void addStatus (std::pair<CharStatusId,int> newStatus);
    bool setIa (bool IA);
    // Setters and Getters
    void setStatus(const std::vector<std::pair<CharStatusId,int>>& status);
  };

};

#endif
