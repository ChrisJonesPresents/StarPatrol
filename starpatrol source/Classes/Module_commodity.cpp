#include "Module_commodity.h"


Module_commodity::Module_commodity(CargoType type, unsigned char stack)
{
	m_cargotype = type;
	m_type = MT_COMMODITY;
	m_stack = stack;
	m_graphicpos = cocos2d::Vec2(type, 0);
	switch (m_cargotype)
	{
	case CT_FUEL:
		m_description = "radioactive fuel";
		m_cost = 2 * m_stack;
		break;
	case CT_ORDINANCE:
		m_description = "spaceship ordinance and ammunition";
		m_cost = 4 * m_stack;
		break;
	}
	
}

unsigned char Module_commodity::getStack()
{
	return m_stack;

}

CargoType Module_commodity::getCargoType()
{
	return m_cargotype;
}

void Module_commodity::setStack(unsigned char stack)
{
	m_stack = stack;
	switch (m_cargotype)
	{
	case CT_FUEL:
		m_cost = 2 * m_stack;
		break;
	case CT_ORDINANCE:
		m_cost = 4 * m_stack;
		break;
	}
}