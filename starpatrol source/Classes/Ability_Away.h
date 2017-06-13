#ifndef ABILITYAWAY
#define ABILITYAWAY

#include "Common.h"
#include "CBReader.h"


AwayAbilityType TypefromString(std::string str);
AwayAbilityTargetRule TargetfromString(std::string str);
AwayModType ModTypefromString(std::string str);
enum SpecialEffect{SE_ESCAPE};

SpecialEffect EffectfromString(std::string str);

class AA_Effect
{
public:
	FieldType m_targettype;
	AwayAbilityTargetRule m_targetrule;
	AwayAbilityType m_type;
	Vec2 m_graphic;
	AA_Effect(CBReader*cbr);
};

class AA_Attack : public AA_Effect
{
public:
	bool m_special;
	float m_multiplier;
	FieldAttack m_attacktype;
	AA_Attack(CBReader *cbr);

};


class AA_Buff : public AA_Effect
{
public:
	int m_strength, m_duration;
	AwayModType m_affects;
	AA_Buff(CBReader *cbr) ;
	std::string m_string;
};

class AA_Debuff : public AA_Buff
{
public:
	int m_chance;
	AA_Debuff(CBReader *cbr) ;

};

class AA_Immunize : public AA_Effect
{
public:
	AwayModType m_affects;
	int strength, M_duration;
	AA_Immunize(CBReader *cbr);
};

class AA_Recover : public AA_Effect
{
public:
	AwayModType m_affects;
	int m_strength;
	AA_Recover(CBReader *cbr);
};

class Ability_Away 
{
	std::string m_name;
	std::string m_description;
	AA_Effect *m_effects[2];
	short m_cooldown, m_cost;
	int m_uid;
	public:
		Ability_Away(CBReader *cbr, int uid);
		~Ability_Away();
		int getCooldown();
		int getCost();
		AA_Effect *getEffect(int i);
		std::string getName();
		std::string getDescription();
		int getUID();
};

#endif