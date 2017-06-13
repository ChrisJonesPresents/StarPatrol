#include "Potion.h"

Potion::Potion(CBReader *cbr, Ability_Away **abilities, int uid)
{
	std::string str=cbr->ReadString();
	m_category = cbr->ReadInt();
	m_uid = uid;
	m_name = LoadLongString(cbr);
	m_description = LoadLongString(cbr);
	m_cost = cbr->ReadInt();
	m_graphicpos = Vec2(cbr->ReadInt(), cbr->ReadInt());
	m_effect = abilities[cbr->ReadInt()];
}