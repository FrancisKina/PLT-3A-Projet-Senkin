#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Character.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestCharacter)
{
  {
    Character chara {};
    BOOST_CHECK_EQUAL(chara.getHpBase(), 0);
    chara.setHpBase(99);
    BOOST_CHECK_EQUAL(chara.getHpBase(), 99);
  }

  {
    Character chara {};
    chara.setHpBase(99);
    BOOST_CHECK_LE(chara.getHpBase(), 100);
    BOOST_CHECK_GT(chara.getHpBase(), 50);
  }
}

