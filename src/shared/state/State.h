// Generated by dia2code
#ifndef STATE__STATE__H
#define STATE__STATE__H

#include <vector>

namespace state {
  class Player;
  class Observable;
  class Cursor;
}

#include "Player.h"
#include "Observable.h"
#include "Cursor.h"
#include "FieldStatusId.h"
#include "FieldTypeId.h"

namespace state {

  /// class State - 
  class State : public state::Observable {
    // Associations
    state::FieldStatusId fieldStatusId;
    state::FieldTypeId fieldTypeId;
    // Attributes
  private:
    std::vector<std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>>> grid;
    std::vector<Player> players;
    int round;
    // Operations
  public:
    std::vector<std::vector<std::pair<FieldTypeId, std::pair<FieldStatusId,int>>>> getGrid ();
    std::vector<Player> getPlayers ();
    int getRound ();
    void setRound (int newRound);
    void initGrid ();
    void initPlayers ();
    void initCursor ();
    // Setters and Getters
    FieldStatusId getFieldStatusId() const;
    void setFieldStatusId(FieldStatusId fieldStatusId);
    FieldTypeId getFieldTypeId() const;
    void setFieldTypeId(FieldTypeId fieldTypeId);
  };

};

#endif
