#include "Facility.h"

Facility::Facility(CBReader *cbr, int index)
{
	//load name
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
			m_name.append(str + " ");
		}
	}
	
	m_index = index; 
	//load upgrade
	m_upgrade = cbr->ReadInt();
	m_upgradeonly = cbr->ReadInt();
	//load description
	m_description = "";
	while (true)
	{
		std::string str = cbr->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_description.append(str + " ");
		}
	}

	//load graphic
	m_graphic = Vec2(cbr->ReadInt(), cbr->ReadInt());
	//load tokencost
	for (int i = 0; i < 4; i++)
	{
		m_tokencost[i] = cbr->ReadInt();
	}
	//load required resources
	for (int i = 0; i < 8; i++)
	{
		m_requires[i] = cbr->ReadInt();
	}
	//load output
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_output[i][j] = cbr->ReadInt();
		}
	}
}

int Facility::getID()
{
	return m_index;
}


Vec2 Facility::getGraphic()
{
	return m_graphic;
}

char Facility::getOutputType(int i)
{
	return m_output[i][0];
}

char Facility::getOutputValue(int i)
{
	return m_output[i][1];
}

int Facility::getCost(int i)
{
	return m_requires[i];
}

int Facility::getTokenCost(int i)
{
	return m_tokencost[i];
}

bool Facility::getUpgradeonly()
{
	return m_upgradeonly;
}

std::string Facility::getName()
{
	return m_name;
}
std::string Facility::getDescription()
{
	return m_description;
}

int Facility::getUpgradeTo()
{
	return m_upgrade; 
}