#ifndef CREWFIELDUNIT
#define CREWFIELDUNIT

#include "FieldUnit.h"
#include "Playership.h"

class CrewFieldUnit : public FieldUnit
{
	ShipCrew *m_crew;
	int m_damage;
public:
	CREATE_FUNC(CrewFieldUnit);
	void Setup(ShipCrew *crew,int health);

	bool Damage(int damage);
	bool Damage(int damage, bool heavy);
	void Cleanup();
};

#endif