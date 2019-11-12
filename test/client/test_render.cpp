#include <boost/test/unit_test.hpp>

#include <SFML/Graphics.hpp>

#include "../../src/shared/state.h"
#include "../../src/client/render.h"

using namespace state;
using namespace render;

BOOST_AUTO_TEST_CASE(TestRender)
{
	state::State s {};
	StateLayer sl {};
	
	s.registerObserver(&sl);
	
	sl.initLayers(s,35);
	sl.getTilesets();
	
}
