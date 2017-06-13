#ifndef REFERENCELIBRARY
#define REFERENCELIBRARY

#include "Module.h"
#include "Ability_Space.h"
#include "NameGenerator.h"
#include "ReferenceLibrary.h"
#include "Quest_Engine.h"
#include "Ability_Away.h"
#include "OfficerDef.h"
#include "Crew.h"
#include "Item.h"
#include "Facility.h"
#include "Potion.h"

class ReferenceLibrary
{
	static ReferenceLibrary *m_instance;
	Ability_Space **m_abilities;
	int m_numabilities;
	Module **m_modules;
	int m_nummodules;

	Ability_Away **m_awayabilities;
	int m_numawayabilities;
	OfficerDef **m_officers;
	int m_numofficers;
	Crew **m_crew;
	int m_numcrew;
	Item **m_items;
	int m_numitems;
	Facility **m_facilities;
	int m_numfacilities;
	Potion **m_potions;
	int m_numpotions;

	void Loadabilities();
	void LoadModules();

	void LoadAwayAbilities();
	void LoadOfficers();

	void LoadCrew();
	void LoadItem();
	void LoadFacility();
	void LoadPotions();
	ReferenceLibrary();
	~ReferenceLibrary();

	NameGenerator m_namegen;
	Quest_Engine m_questengine;
public:



	static ReferenceLibrary *getInstance();

	static void FreeInstance();

	Module *getModule(int i);

	Ability_Space *getAbility(int i);

	std::string getName();

	OfficerDef *getOfficer(int i);
	Ability_Away *getActorAbility(int i);
	Crew *getCrew(int i);

	Item *getItem(int i);

	int getNumFacility();
	Facility *getFacility(int i);

	Quest_Engine *getQuestEngine();

	int getInventoryModule(short category);
	int getInventoryPotion(short category);
	int getBaseFacilitiesCount();
	Facility **getBaseFacilitiesList();
	Facility *getUpgrade(Facility *facility);

	static std::string getName(std::string type);
	Potion* getPotion(int i);
	int getNumPotions();
};



#endif