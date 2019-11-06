#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/State.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestState)
{

	State s {};
	bool done = s.initGrid("maptest.txt");
	//BOOST_CHECK_EQUAL(s.getGrid()[0][0].first, static_cast<FieldTypeId>(4) );
	s.initCursor();
	BOOST_CHECK_EQUAL(s.getCursor()->getCursorX(),0);
	s.getCursor()->setCursorX(5);
	BOOST_CHECK_EQUAL(s.getCursor()->getCursorX(),5);
	BOOST_CHECK_EQUAL(s.getCursor()->getCursorY(),0);
	s.initPlayers(6);
	BOOST_CHECK_EQUAL(s.getPlayers()[0]->getX(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[3]->getX(),3);
	BOOST_CHECK_EQUAL(s.getPlayers()[2]->getY(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[4]->getDirection(),SOUTH);
	
	//BOOST_CHECK_EQUAL(s.getPlayers()[5]->getX(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[5]->getX(),5);
	BOOST_CHECK_EQUAL(s.getPlayers()[5]->getY(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[5]->getDirection(),SOUTH);
	BOOST_CHECK_EQUAL(done, 0);
}
