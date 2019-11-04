// Generated by dia2code
#ifndef ENGINE__DEPLACEMENT__H
#define ENGINE__DEPLACEMENT__H

#include <vector>

namespace state {
  class State;
};
namespace engine {
  class Commande;
}

#include "Commande.h"

namespace engine {

  /// class Deplacement - 
  class Deplacement : public engine::Commande {
    // Attributes
  private:
    state::Personnage& cible;
    std::tuple<int,int> destination;
    // Operations
  public:
    Deplacement (state::Personnage& cible, std::vector<int,int> destination, bool joueur);
    void execute (state::State& state);
    // Setters and Getters
  };

};

#endif
