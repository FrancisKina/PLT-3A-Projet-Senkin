#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Player.h"

#include "../../src/shared/state/SkillFactory.h"

#include "../../src/shared/state/CharacterFactory.h"

using namespace ::state;

BOOST_AUTO_TEST_CASE(TestPlayer)
{

	Player p {};
	BOOST_CHECK_EQUAL(p.getX(), 0);
	p.setX(99);
	BOOST_CHECK_EQUAL(p.getX(), 99);
	
	BOOST_CHECK_EQUAL(p.getY(), 0);
	p.setY(99);
	BOOST_CHECK_EQUAL(p.getY(), 99);
	
	BOOST_CHECK_EQUAL(p.getDirection(), SOUTH);
	p.setDirection(NORTH);
	BOOST_CHECK_EQUAL(p.getDirection(), NORTH);
	BOOST_CHECK_EQUAL(p.getDirection(), static_cast<DirectionId>(1));
	
	BOOST_CHECK_EQUAL(p.getHp(), 0);
	p.setHp(99);
	BOOST_CHECK_EQUAL(p.getHp(), 99);
	
	BOOST_CHECK_EQUAL(p.getMovement(), 0);
	p.setMovement(99);
	BOOST_CHECK_EQUAL(p.getMovement(), 99);
	
	BOOST_CHECK_EQUAL(p.getInitiative(), 0);
	p.setInitiative(99);
	BOOST_CHECK_EQUAL(p.getInitiative(), 99);
	
	BOOST_CHECK_EQUAL(p.getSkillCount(), 0);
	p.setSkillCount(99);
	BOOST_CHECK_EQUAL(p.getSkillCount(), 99);
	
	p.setSkillCount(0);
	BOOST_CHECK_EQUAL(p.getSkillCount(), 0);
	p.setSkillCount(99);
	BOOST_CHECK_EQUAL(p.getSkillCount(), 99);
	
	BOOST_CHECK_EQUAL(p.getIa(), 0);
	p.setIa(1);
	BOOST_CHECK_EQUAL(p.getIa(), 1);
	
	BOOST_CHECK_EQUAL(p.getName(), "J");
	p.setName("Joueur");
	BOOST_CHECK_EQUAL(p.getName(), "Joueur");
	
	CharacterFactory cf;
	p.setCharacter(cf.createCharacter(FROG));
	BOOST_CHECK_EQUAL(p.getCharacter()->getClassId(), FROG);
	
	p.setCharacter(cf.createCharacter(DWARF));
	BOOST_CHECK_EQUAL(p.getCharacter()->getClassId(), DWARF);
	
	p.setCharacter(cf.createCharacter(ARCHER));
	BOOST_CHECK_EQUAL(p.getCharacter()->getClassId(), ARCHER);
	
	p.setCharacter(cf.createCharacter(KNIGHT));
	BOOST_CHECK_EQUAL(p.getCharacter()->getClassId(), KNIGHT);

	std::vector<std::pair<CharStatusId,int>> testStatus = {{BURNED,7},{POISONED,7}};
	p.setStatus(testStatus);
	for(size_t i=0; i<testStatus.size();i++){
		BOOST_CHECK_EQUAL(p.getStatus()[i].first, testStatus[i].first);
		BOOST_CHECK_EQUAL(p.getStatus()[i].second, testStatus[i].second);
	}
	
	std::pair<CharStatusId,int> testupdate = {BURNED,12};
	p.updateStatus(testupdate);
	for(size_t i=0; i<testStatus.size();i++){
		if (p.getStatus()[i].first == testupdate.first){
			BOOST_CHECK_EQUAL(p.getStatus()[i].second, testupdate.second);
		}
	}
	
	Skill* skill1;
	Skill* skill2;
	SkillFactory skfa;
	skill1 = skfa.createSkill(FRAPPE);
	skill2 = skfa.createSkill(ARC);
	std::vector<Skill*> testSkills= {skill1,skill2};
	p.setSkills(testSkills);
	BOOST_CHECK_EQUAL(p.getSkills()[0]->getDamage(),testSkills[0]->getDamage());
	
	Skill* skill3;
	skill3 = skfa.createSkill(BOULE_DE_FEU);
	p.setSkill(skill3);
	BOOST_CHECK_EQUAL(p.getSkills()[2]->getDamage(), skill3->getDamage());

	
}
