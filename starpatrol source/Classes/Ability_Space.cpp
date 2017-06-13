#include "Ability_Space.h"


void Ability_Space::LoadType(CBReader *cbr)
{

	std::string str = cbr->ReadString();
	if (str.find("BEAM") != std::string::npos)
	{
		m_type = AT_beam;
	}
	if (str.find("PROJECTILE") != std::string::npos)
	{
		m_type = AT_projectile;
	}
	if (str.find("MISSILE") != std::string::npos)
	{
		m_type = AT_missile;
	}
	if (str.find("BUFF") != std::string::npos)
	{
		m_type = AT_buff;
	}
	if (str.find("OTHER") != std::string::npos)
	{
		m_type = AT_other;
	}
	
}

void Ability_Space::LoadCost(CBReader *cbr)
{
	std::string str = cbr->ReadString();
	if (str.find("COSTFUEL") != std::string::npos)
	{
		m_costtype = AC_fuel;
	}
	if (str.find("COSTORDINANCE") != std::string::npos)
	{
		m_costtype = AC_ordinance;
	}
	if (str.find("COSTCREW") != std::string::npos)
	{
		m_costtype = AC_crew;
	}
	m_costvalue = cbr->ReadInt();

}

void Ability_Space::LoadEffect(CBReader *cbr)
{
	std::string str = cbr->ReadString();
	if (str.find("ATTACK") != std::string::npos)
	{
		m_effect = new AS_damageeffect(cbr);
	}
	if (str.find("BUFF") != std::string::npos)
	{
		m_effect = new AS_buffeffect(cbr);
	}
}

Ability_Space::Ability_Space(CBReader *cbr, int id)
{
	m_id = id;
	m_name = "";
	while (true)
	{
		std::string str = cbr->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_name.append(str+" ");
		}
	}
		
	LoadType(cbr);
	m_commandcost = cbr->ReadInt();
	LoadCost(cbr);
	LoadEffect(cbr);
}

int Ability_Space::getID()
{
	return m_id;
}

Ability_Space::Ability_Space()
{
	m_name = "lasercannon";
	m_type = AT_buff;
}

std::string Ability_Space::getName()
{
	return m_name;
}

AbilityType Ability_Space::getAbilityType()
{
	return m_type;
}

AbilityCost Ability_Space::getCostType()
{
	return m_costtype;
}

int Ability_Space::getCost()
{
	return m_costvalue;
}

int Ability_Space::getCommandcost()
{
	return m_commandcost;
}

AS_effect *Ability_Space::getEffect()
{
	return m_effect;
}
