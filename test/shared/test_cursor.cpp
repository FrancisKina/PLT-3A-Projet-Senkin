#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Cursor.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestCursor)
{

	Cursor c {};
	BOOST_CHECK_EQUAL(c.getCursorX(), 0);
	c.setCursorX(99);
	BOOST_CHECK_EQUAL(c.getCursorX(), 99);
	
	BOOST_CHECK_EQUAL(c.getCursorY(), 0);
	c.setCursorY(99);
	BOOST_CHECK_EQUAL(c.getCursorY(), 99);
}
