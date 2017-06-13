#include "Ability_Space.h"

AS_damageeffect::AS_damageeffect(CBReader *cbr)
{
	m_type = AE_damage;

	//read damage
	m_damage = cbr->ReadInt();
	//read shots
	m_shots = cbr->ReadInt();
	//read penetration
	m_penetration = cbr->ReadFloat();
	//read graphicpositions
	m_graphicpos[0] = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
	m_graphicpos[1] = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
}

AS_buffeffect::AS_buffeffect(CBReader *cbr)
{
	m_type = AE_buff;
	m_modifies = StrtoShipmod(cbr->ReadString());
	m_percentage = cbr->ReadInt();
	m_effect = cbr->ReadInt();
	m_duration = cbr->ReadInt();
	m_graphicpos[0] = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
	m_graphicpos[1] = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
}