#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Skill.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestSkill)
{

	Skill s {};
	BOOST_CHECK_EQUAL(s.getDamage(), 0);
	s.setDamage(99);
	BOOST_CHECK_EQUAL(s.getDamage(), 99);
	
	std::pair<int,int> testRange = {1,9};
	s.setRange(testRange);
	BOOST_CHECK_EQUAL(s.getRange().first, testRange.first );
	BOOST_CHECK_EQUAL(s.getRange().second, testRange.second );
	
	std::vector<std::pair<int,int>> testArea = {{0,1},{0,2},{2,3}};
	s.setArea(testArea);
	for(size_t i=0; i<testArea.size(); i++){
		BOOST_CHECK_EQUAL(s.getArea()[i].second, testArea[i].second );
		BOOST_CHECK_EQUAL(s.getArea()[i].first, testArea[i].first );
	}
	
	/*std::vector<std::tuple<EffectId,int,int>> testEffect = {{BURN_CHAR,1,90},{HEAL_LOW,2,100},{MOVE_FOE,-1,20}};
	s.setEffect(testEffect);
	for(size_t i=0; i<testArea.size(); i++){
		BOOST_CHECK_EQUAL(std::get<0>(s.getEffect()[i]), std::get<0>(testEffect[i]));
		BOOST_CHECK_EQUAL(std::get<1>(s.getEffect()[i]), std::get<1>(testEffect[i]));
		BOOST_CHECK_EQUAL(std::get<2>(s.getEffect()[i]), std::get<2>(testEffect[i]));
	}*/
	
	BOOST_CHECK_EQUAL(s.getPrecision(), 0);
	s.setPrecision(99);
	BOOST_CHECK_EQUAL(s.getPrecision(), 99);
	
	BOOST_CHECK_EQUAL(s.getCooldown(), 0);
	s.setCooldown(99);
	BOOST_CHECK_EQUAL(s.getCooldown(), 99);
	
	BOOST_CHECK_EQUAL(s.getSpecial(), 0);
	s.setSpecial(1);
	BOOST_CHECK_EQUAL(s.getSpecial(), 1);
}
