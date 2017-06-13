#include "OfficerDef.h"

OfficerDef::OfficerDef(CBReader *cbr, Ability_Away **abilities,int uid)
{
	m_uid = uid;
	//name
	m_classname = cbr->ReadString();
	m_cost = cbr->ReadInt();
	//portrait
	m_portrait = Vec2(cbr->ReadInt(), cbr->ReadInt());

	//stat loading
	for (int i = 0; i < 10; i++)
	{
		m_stats[i] = cbr->ReadInt();
	}
	//ability links
	for (int i = 0; i < 6; i++)
	{
		m_abilities[i] = abilities[cbr->ReadInt()];
	}
	//read basic attack type
	m_baseattack = StrotoFieldattack(cbr->ReadString());

}

Ability_Away *OfficerDef::getAbility(int i)
{
	return m_abilities[i];
}
std::string OfficerDef::getName()
{
	return m_classname;

}

Vec2 OfficerDef::getPortrait()
{
	return m_portrait;
}

int OfficerDef::getCost()
{
	return m_cost;
}

int OfficerDef::getStat(int i)
{
	return m_stats[i];
}
FieldAttack OfficerDef::getAttackType()
{
	return m_baseattack;
}

int OfficerDef::getUID()
{
	return m_uid;
}