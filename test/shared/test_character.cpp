#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Character.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestCharacter)
{

	Character chara {};
	BOOST_CHECK_EQUAL(chara.getHpBase(), 0);
	chara.setHpBase(99);
	BOOST_CHECK_EQUAL(chara.getHpBase(), 99);
	
	BOOST_CHECK_EQUAL(chara.getMovementBase(), 0);
	chara.setMovementBase(99);
	BOOST_CHECK_EQUAL(chara.getMovementBase(), 99);

	BOOST_CHECK_EQUAL(chara.getSkillCountBase(), 0);
	chara.setSkillCountBase(99);
	BOOST_CHECK_EQUAL(chara.getSkillCountBase(), 99);

	BOOST_CHECK_EQUAL(chara.getInitiativeBase(), 0);
	chara.setInitiativeBase(99);
	BOOST_CHECK_EQUAL(chara.getInitiativeBase(), 99);

	BOOST_CHECK_EQUAL(chara.getAttackBase(), 0);
	chara.setAttackBase(99);
	BOOST_CHECK_EQUAL(chara.getAttackBase(), 99);

	BOOST_CHECK_EQUAL(chara.getMovementType(), 1);
	chara.setMovementType(AGILE);
	BOOST_CHECK_EQUAL(chara.getMovementType(), AGILE);

	BOOST_CHECK_EQUAL(chara.getClassId(), 4);
	chara.setClassId(FROG);
	BOOST_CHECK_EQUAL(chara.getClassId(), FROG);

}

