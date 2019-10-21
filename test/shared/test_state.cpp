#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/State.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestState)
{

	State s {};
	bool done = s.initGrid("maptest.txt");
	//BOOST_CHECK_EQUAL(s.getGrid()[0][0].first, static_cast<FieldTypeId>(4) );
	BOOST_CHECK_EQUAL(done, 0);
}
