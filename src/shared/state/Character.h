// Generated by dia2code
#ifndef STATE__CHARACTER__H
#define STATE__CHARACTER__H


namespace state {
  class Player;
}

#include "MovementId.h"
#include "Player.h"

namespace state {

  /// class Character - 
  class Character {
    // Associations
    // Attributes
  protected:
    int hpBase;
    int movementBase;
    int skillCountBase;
    int initiativeBase;
    MovementId movementType;
    // Operations
  public:
    int getHpBase ();
    int getMovementBase ();
    int getSkillCountBase ();
    int getInitiativeBase ();
    MovementId getMovementType ();
    void setHpBase (int newHp);
    void setMovementBase (int newMove);
    void setSkillCountBase (int newSC);
    void setInitiativeBase (int newInit);
    void setMovementType (MovementId newId);
    // Setters and Getters
  };

};

#endif
