#include "Module_Booster.h"

Module_Booster::Module_Booster(CBReader *cbr)
{
	CommonLoad(cbr);
	m_type = MT_BOOSTER;
	//load type
	std::string str = cbr->ReadString();
	m_modifier = StrtoShipmod(str);
	//load effect
	m_value = cbr->ReadInt();
	//load graphic
	m_defencegraphic = Vec2(cbr->ReadInt(), cbr->ReadInt());
}

int Module_Booster::getValue()
{
	return m_value;
}
Shipmod Module_Booster::getModier()
{
	return m_modifier;
}

Vec2 Module_Booster::getGraphic()
{
	return m_defencegraphic;
}