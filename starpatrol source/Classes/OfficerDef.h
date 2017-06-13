#ifndef OFFICERDEF
#define OFFICERDEF

#include "CBReader.h"
#include "Ability_Away.h"
#include "Common.h"

class OfficerDef
{
	std::string m_classname;
	Vec2 m_portrait;
	Ability_Away *m_abilities[6];
	int m_cost;
	FieldAttack m_baseattack;
	unsigned int m_stats[10];
	int m_uid;
public:
	OfficerDef(CBReader *cbr,Ability_Away **abilities,int uid);

	Ability_Away *getAbility(int i);
	std::string getName();
	Vec2 getPortrait();

	int getCost();
	int getStat(int i);
	FieldAttack getAttackType();
	int getUID();

};

#endif