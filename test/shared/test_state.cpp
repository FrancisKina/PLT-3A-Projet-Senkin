#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/State.h"

#include "../../src/shared/state/Field.h"

using namespace state;

BOOST_AUTO_TEST_CASE(TestState)
{

	State s {};
	bool done = s.initGrid("../../res/maptest.txt");
	BOOST_CHECK_EQUAL(done, 0);
	
	std::vector<std::vector<Field*>> grid;
	grid = s.getGrid();
	
	s.initCursor();
	BOOST_CHECK_EQUAL(s.getCursor()->getCursorX(),0);
	
	s.getCursor()->setCursorX(5);
	BOOST_CHECK_EQUAL(s.getCursor()->getCursorX(),5);
	BOOST_CHECK_EQUAL(s.getCursor()->getCursorY(),0);
	
	s.initPlayers(3);
	BOOST_CHECK_EQUAL(s.getPlayers()[0]->getX(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[0]->getY(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[1]->getX(),0);
	BOOST_CHECK_EQUAL(s.getPlayers()[1]->getY(),0);
	
	s.setRound(8);
	BOOST_CHECK_EQUAL(s.getRound(),8);
	
	s.getPlayers()[0]->setInitiative(8);
	s.getPlayers()[1]->setInitiative(1);
	s.getPlayers()[2]->setInitiative(5);
	s.sortPlayers();
	BOOST_CHECK_EQUAL(s.getPlaying(), s.getPlayers()[0]);
	
	/*Field*/
	Field f {};
	
	BOOST_CHECK_EQUAL(f.getFieldType(),PLAIN);
	f.setFieldType(SAND);
	BOOST_CHECK_EQUAL(f.getFieldType(),SAND);
	
	std::vector<std::pair<FieldStatusId,int>> testfieldstatus = {{RAIN, 0}, {SNOW, 0}, {MIST, 0}};
	f.setFieldStatus(testfieldstatus);
	for(size_t i=0; i<testfieldstatus.size(); i++){
		BOOST_CHECK_EQUAL(f.getFieldStatus()[i].first, testfieldstatus[i].first);
		BOOST_CHECK_EQUAL(f.getFieldStatus()[i].second, testfieldstatus[i].second);
	}
	
	f.updateFieldStatus({SNOW,8});
	BOOST_CHECK_EQUAL(f.getFieldStatus()[1].second, 8);
}
