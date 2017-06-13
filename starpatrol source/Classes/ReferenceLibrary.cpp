#include "ReferenceLibrary.h"
#include "cocos2d.h"
#include "CBReader.h"
#include "Module_ability.h"
#include "Module_Booster.h"

void ReferenceLibrary::Loadabilities()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/abilities.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	//read array size first
	m_numabilities = cbr->ReadInt();
	m_abilities = new Ability_Space *[m_numabilities];
	for (int i = 0; i < m_numabilities; i++)
	{
		std::string str = cbr->ReadString();
		int id = cbr->ReadInt();
		if (id == i)
		{
			m_abilities[i] = new Ability_Space(cbr,i);
		}
	}
	delete cbr;
}

void ReferenceLibrary::LoadModules()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/modules.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	//read array size first
	m_nummodules = cbr->ReadInt();
	m_modules = new Module *[m_nummodules];
	for (int i = 0; i < m_nummodules; i++)
	{
		std::string str = cbr->ReadString();
		if (str.find("ABILITYMODULE") != std::string::npos)
		{
			m_modules[i] = new Module_Ability(cbr,m_abilities);
		}
		if (str.find("BOOSTERMODULE") != std::string::npos)
		{
			m_modules[i] = new Module_Booster(cbr);
		}
		m_modules[i]->setUID(i);
	}
	delete cbr;
}

void ReferenceLibrary::LoadAwayAbilities()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/officerabilities.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	m_numawayabilities = cbr->ReadInt();
	m_awayabilities = new Ability_Away *[m_numawayabilities];
	for (int i = 0; i < m_numawayabilities; i++)
	{
		std::string str = cbr->ReadString();
		int id = cbr->ReadInt();
		if (id == i)
		{
			m_awayabilities[i] = new Ability_Away(cbr,i);
		}
	}
	delete cbr;
}

void ReferenceLibrary::LoadOfficers()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/officers.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	m_numofficers = cbr->ReadInt();
	m_officers = new OfficerDef *[m_numofficers];

	for (int i = 0; i < m_numofficers; i++)
	{
		std::string str = cbr->ReadString();
		m_officers[i] = new OfficerDef(cbr,m_awayabilities,i);
	}
	delete cbr;
}

void ReferenceLibrary::LoadCrew()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/crew.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	m_numcrew = cbr->ReadInt();
	m_crew = new Crew*[m_numcrew];
	for (int i = 0; i < m_numcrew; i++)
	{
		std::string str = cbr->ReadString();
		m_crew[i] = new Crew();

		m_crew[i]->m_name = "";
		while (true)
		{
			std::string str = cbr->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_crew[i]->m_name.append(str + " ");
			}
		}
		m_crew[i]->m_description = "";
		while (true)
		{
			std::string str = cbr->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_crew[i]->m_description.append(str + " ");
			}
		}
		m_crew[i]->m_type = StrtoFieldType(cbr->ReadString());
		m_crew[i]->m_UID = i;
		m_crew[i]->m_attack = cbr->ReadInt();
		m_crew[i]->m_defence = cbr->ReadInt();
		m_crew[i]->m_requires = cbr->ReadInt();
		m_crew[i]->m_cost = cbr->ReadInt();
		m_crew[i]->m_hard = cbr->ReadInt();
		m_crew[i]->m_heavy = cbr->ReadInt();
		m_crew[i]->m_frontrow = cbr->ReadInt();
		m_crew[i]->m_individual = cbr->ReadInt();
		m_crew[i]->m_graphic = Vec2(cbr->ReadInt(), cbr->ReadInt());
		m_crew[i]->m_size = Vec2(cbr->ReadInt(), cbr->ReadInt());
	}

	delete cbr;
}

void ReferenceLibrary::LoadItem()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/items.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	m_numitems = cbr->ReadInt();
	m_items = new Item*[m_numitems];
	for (int i = 0; i < m_numitems; i++)
	{
		m_items[i] = new Item();
		m_items[i]->m_name = "";
		while (true)
		{
			std::string str = cbr->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_items[i]->m_name.append(str + " ");
			}
		}
		m_items[i]->m_description = "";
		while (true)
		{
			std::string str = cbr->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_items[i]->m_description.append(str + " ");
			}
		}
		m_items[i]->m_type = (ITEMTYPE)cbr->ReadInt();
		m_items[i]->m_cost = cbr->ReadInt();
		m_items[i]->m_uid = i;
	}
	delete cbr;
}

void ReferenceLibrary::LoadFacility()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/facilities.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	m_numfacilities = cbr->ReadInt();
	m_facilities = new Facility*[m_numfacilities];
	for (int i = 0; i < m_numfacilities; i++)
	{
		m_facilities[i] = new Facility(cbr,i);

	}
	delete cbr;
}

void ReferenceLibrary::LoadPotions()
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/potions.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	m_numpotions = cbr->ReadInt();
	m_potions = new Potion*[m_numpotions];
	for (int i = 0; i < m_numpotions; i++)
	{
		m_potions[i] = new Potion(cbr, m_awayabilities,i);

	}
	delete cbr;
}

Item *ReferenceLibrary::getItem(int i)
{
	return m_items[i];
}

ReferenceLibrary::ReferenceLibrary()
{

	Loadabilities();
	LoadModules();
	LoadAwayAbilities();
	LoadOfficers();
	LoadCrew();
	LoadItem();
	LoadFacility();
	LoadPotions();
}
ReferenceLibrary::~ReferenceLibrary()
{
	for (int i = 0; i < m_numabilities; i++)
	{
		delete m_abilities[i];
	}
	delete[] m_abilities;
	for (int i = 0; i < m_nummodules; i++)
	{
		delete m_modules[i];
	}
	delete[] m_modules;
	for (int i = 0; i < m_numawayabilities; i++)
	{
		delete m_awayabilities[i];
	}
	delete[]m_awayabilities;
	for (int i = 0; i < m_numofficers; i++)
	{
		delete m_officers[i];
	}
	delete[]m_officers;
	for (int i = 0; i < m_numcrew; i++)
	{
		delete m_crew[i];
	}
	delete[]m_crew;
	for (int i = 0; i < m_numitems; i++)
	{
		delete m_items[i];
	}
	delete[]m_items;

	for (int i = 0; i < m_numfacilities; i++)
	{
		delete m_facilities[i];
	}
	delete[]m_facilities;

	for (int i = 0; i < m_numpotions; i++)
	{
		delete m_potions[i];
	}
	delete[]m_potions;
}

ReferenceLibrary *ReferenceLibrary::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ReferenceLibrary();
		return m_instance;
	}
	else
	{
		return m_instance;
	}

}

void ReferenceLibrary::FreeInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
	}
}

Module *ReferenceLibrary::getModule(int i)
{
	return m_modules[i];

}

Ability_Space *ReferenceLibrary::getAbility(int i)
{
	return m_abilities[i];
}

std::string ReferenceLibrary::getName()
{
	return m_namegen.getName();
}
OfficerDef *ReferenceLibrary::getOfficer(int i)
{
	return m_officers[i];
}

Quest_Engine *ReferenceLibrary::getQuestEngine()
{
	return &m_questengine;
}

Crew *ReferenceLibrary::getCrew(int i)
{
	return m_crew[i];
}

Ability_Away *ReferenceLibrary::getActorAbility(int i)
{
	return m_awayabilities[i];
}

int ReferenceLibrary::getNumFacility()
{
	return m_numfacilities;
}

Facility *ReferenceLibrary::getFacility(int i)
{
	return m_facilities[i];
}

int ReferenceLibrary::getInventoryModule(short category)
{
	//compile a list of items in the category
	int indexes[6];
	int length = 0;
	int index = 0;
	for (int i = 0; i < m_nummodules; i++)
	{
		if (m_modules[i]->getCategory() == category)
		{
			if (length < 6)
			{
				indexes[index] = i;
				index++;
				length++;
			}
		}
	}
	int r = rand() % length;

	return indexes[r];
}

int ReferenceLibrary::getInventoryPotion(short category)
{
	//compile a list of items in the category
	int indexes[6];
	int length = 0;
	int index = 0;
	for (int i = 0; i < m_numpotions; i++)
	{
		if (m_potions[i]->m_category == category)
		{
			if (length < 6)
			{
				indexes[index] = i;
				index++;
				length++;
			}
		}
	}
	int r = rand() % length;

	return indexes[r];
}

int ReferenceLibrary::getBaseFacilitiesCount()
{
	int count = 0;
	for (int i = 0; i < m_numfacilities; i++)
	{
		if (m_facilities[i]->getUpgradeonly()==false)
		{
			count++;
		}

	}

	return count;
}

Facility **ReferenceLibrary::getBaseFacilitiesList()
{
	Facility **list = new Facility*[getBaseFacilitiesCount()];
	int index = 0;
	for (int i = 0; i < m_numfacilities; i++)
	{
		if (m_facilities[i]->getUpgradeonly() == false)
		{
			list[index] = m_facilities[i];
			index++;
		}
	}
	return list;
}

Facility *ReferenceLibrary::getUpgrade(Facility *facility)
{
	if (m_facilities[facility->getUpgradeTo()]->getUpgradeonly() == true)
	{
		return m_facilities[facility->getUpgradeTo()];
	}
	return NULL;
}


std::string ReferenceLibrary::getName(std::string type)
{
	ssize_t s;
	unsigned long length;
	std::string fname = "data/names/"; fname.append(type); fname.append(".txt");
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fname);
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);

	int count = cbr->ReadInt();
	std::string *list = new std::string[count];
	for (int i = 0; i < count; i++)
	{
		list[i] = cbr->ReadString();
	}
	delete cbr;

	std::string returnstring = list[rand() % count];

	delete [] list;
	return returnstring;
}

Potion* ReferenceLibrary::getPotion(int i)
{
	return m_potions[i];
}
int ReferenceLibrary::getNumPotions()
{
	return m_numpotions;
}

ReferenceLibrary *ReferenceLibrary::m_instance = NULL;