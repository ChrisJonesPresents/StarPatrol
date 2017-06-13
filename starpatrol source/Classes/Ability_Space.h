#ifndef ABILITYSPACE
#define ABILITYSPACE

enum AbilityType{AT_beam,AT_missile,AT_projectile,AT_other,AT_buff};
enum AbilityCost{AC_fuel,AC_ordinance,AC_crew};
enum AbilityEffect{ AE_damage,AE_buff };
#include <string>
#include "CBReader.h"
#include "cocos2d.h"
#include "Common.h"

class AS_effect
{
public:
	AbilityEffect m_type;
	cocos2d::Vec2 m_graphicpos[2];
};

class AS_damageeffect : public AS_effect
{
public:
	AS_damageeffect(CBReader *cbr);
	
	int m_damage, m_shots;
	float m_penetration;


};

class AS_buffeffect : public AS_effect
{
public:
	AS_buffeffect(CBReader *cbr);
	int m_effect, m_duration;
	Shipmod m_modifies;
	bool m_percentage;

};

class Ability_Space
{
	std::string m_name;
	AbilityType m_type;
	int m_commandcost;
	AbilityCost m_costtype;
	int m_costvalue;
	AS_effect *m_effect;

	void LoadType(CBReader *cbr);
	void LoadCost(CBReader *cbr);
	void LoadEffect(CBReader *cbr);
	int m_id;
public:
	Ability_Space(CBReader *cbr,int id);
	Ability_Space();
	std::string getName();
	AbilityType getAbilityType();
	AbilityCost getCostType();
	int getCost();
	int getCommandcost();
	AS_effect *getEffect();
	int getID();

};

#endif