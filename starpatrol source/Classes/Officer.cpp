#include "Officer.h"
#include "ReferenceLibrary.h"

Officer::Officer(OfficerDef *definition)
{
	m_name = "Bob";
	m_definition = definition;
	m_experience = 0;
	m_level = 0;

	for (int i = 0; i < 4; i++)
	{
		m_abilities[i] = NULL;
		m_upgradelevels[i] = 0;
	}
	m_upgradelevels[4] = 0;

	m_abilities[0] = definition->getAbility(0);
}

std::string Officer::getName()
{
	return m_definition->getName() + " " + m_name;
}

std::string *Officer::getNamePtr()
{
	return &m_name;
}

void Officer::setName(std::string name)
{
	m_name = name;
}
Ability_Away* Officer::getAbility(int i)
{
	return m_abilities[i];
}
int Officer::getLevel()
{
	return m_level;
}
int Officer::getExperience()
{
	return m_experience;
}

OfficerDef *Officer::getDefinition()
{
	return m_definition;
}

bool Officer::CanLevel()
{
	int v = (m_level + 1)* 2;
	if (m_experience >= v*v)
	{
		return true;
	}
	return false;
}

void Officer::LevelUp(int choice)
{
	m_level++;
	if (m_level == 1)
	{
		m_abilities[1] = m_definition->getAbility(1 + choice);
	}
	if (m_level == 3)
	{
		m_abilities[2] = m_definition->getAbility(3 + choice);
	}
	if (m_level == 6)
	{
		m_abilities[3] = m_definition->getAbility(5);
	}
}

void Officer::addEXP(int value)
{
	m_experience += value;
}

int Officer::getStat(int i)
{
	return m_definition->getStat(i) + (m_definition->getStat(i + 5)*m_level) + (m_upgradelevels[i] * 2);
}


FieldAttack Officer::getAttack()
{
	return m_definition->getAttackType();
}

byte Officer::getUpgradeLevel(int i)
{
	return m_upgradelevels[i];
}

bool Officer::UpgradeStat(int i)
{
	if (m_upgradelevels[i] < 9)
	{
		m_upgradelevels[i]++;
		return true;
	}
	return false;
}

void Officer::Save(FILE *file)
{
	SaveLongString(m_name, file);
	fwrite((void*)&m_level, sizeof(short), 1, file);
	fwrite((void*)&m_experience, sizeof(short), 1, file);
	int type = m_definition->getUID();
	fwrite((void*)&type, sizeof(int), 1, file);

	for (int i = 0; i < 5; i++)
	{
		fwrite((void*)&m_upgradelevels[i], sizeof(byte), 1, file);
	}
	for (int i = 0; i < 4; i++)
	{
		if (m_abilities[i] != NULL)
		{
			int id = m_abilities[i]->getUID();
			fwrite((void*)&id, sizeof(int), 1, file);
		}
		else
		{
			int id = -1;
			fwrite((void*)&id, sizeof(int), 1, file);
		}
	}
	
}

Officer::Officer(CBReader *reader)
{
	m_name = LoadLongString(reader);
	m_level = reader->ReadBShort();
	m_experience = reader->ReadBShort();
	m_definition = ReferenceLibrary::getInstance()->getOfficer(reader->ReadBInt());
	for (int i = 0; i < 5; i++)
	{
		m_upgradelevels[i] = reader->ReadChar();
	}
	for (int i = 0; i < 4; i++)
	{
		int id = reader->ReadBInt();
		if (id == -1)
		{
			m_abilities[i] = NULL;
		}
		else
		{
			m_abilities[i] = ReferenceLibrary::getInstance()->getActorAbility(id);
		}
	}
}