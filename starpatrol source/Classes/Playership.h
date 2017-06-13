#ifndef PLAYERSHIP
#define PLAYERSHIP

#include "Starship.h"
#include "Module.h"
#include "Common.h"
#include "Crew.h"

class ShipCrew : public Object
{
public:
	Crew *m_crew;
	int m_count;
};

class Playership : public Starship
{
	byte m_s;
	float m_fuel;
	unsigned short m_fuelmax;
	float m_speed;
	Module **m_modules;
	unsigned short m_nummodules;
	unsigned short m_crewmax;
	unsigned short m_crewcount;
	std::string m_name;
	unsigned short m_resources[4];

	void Setbasestats(byte size);
	Vector <ShipCrew*> m_crew;

public:
	Playership(byte s=0,std::string string="Intrepid");
	~Playership();

	void modFuel(float d);
	void setHP(int v);
	void setFuel(int v); 
	int getFuel();
	int getMaxFuel();
	int getFreeCrew();
	int getCrewCount();
	int getOrdinance();
	void setCrewCount();
	int getNumModules();
	Module *getModule(int i);
	bool InstallModule(Module *module, int slot,bool erase=true);
	float getSpeed();

	void CalcStats();

	std::string getName();

	byte getS();

	Vector <ShipCrew*> *getCrew();
	int getMaxCrew();
	void ModResource(int i, int amount);
	int getResource(int i);
	bool HireCrew(Crew *crew, int count);

	void Save(FILE *file);
	Playership(CBReader *reader);
	void setName(std::string name);
	float Refuel(float fuel);
};

#endif