#ifndef OFFICER
#define OFFICER

#include "OfficerDef.h"
#include "Ability_Away.h"

class Officer
{
	std::string m_name;
	OfficerDef *m_definition;
	short m_level;
	short m_experience;

	/*experience cost is:
	64 for level 2
	128 for level 3
	256 for level 4*/
	byte m_upgradelevels[5];
	Ability_Away *m_abilities[4];

public:
	Officer(OfficerDef *definition);
	std::string getName();
	std::string *getNamePtr();
	void setName(std::string name);
	Ability_Away* getAbility(int i);
	int getLevel();
	int getExperience();
	OfficerDef *getDefinition();
	bool CanLevel();
	void LevelUp(int choice);
	void addEXP(int value);
	int getStat(int i);
	byte getUpgradeLevel(int i);
	bool UpgradeStat(int i);
	FieldAttack getAttack();
	void Save(FILE *file);
	Officer(CBReader *reader);
};

#endif