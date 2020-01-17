#include <boost/test/unit_test.hpp>

#include "../../src/shared/state.h"
#include "../../src/shared/engine/Engine.h"

using namespace state;
using namespace engine;

BOOST_AUTO_TEST_CASE(TestEngine)
{

	Engine engine;
	State& state = engine.getState();

	state.setGrid({{new Field}});
	
	CharacterFactory cf;

}

