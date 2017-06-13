#include "CrewFieldUnit.h"

void CrewFieldUnit::Setup(ShipCrew *crew,int health)
{
	m_damage = 0;
	m_crew = crew;
	m_name = crew->m_crew->m_name;
	if (m_crew->m_crew->m_individual == true)
	{
		FieldUnit::Setup(crew->m_crew->m_graphic, crew->m_crew->m_size, crew->m_crew->m_individual, crew->m_crew->m_hard, crew->m_crew->m_heavy, 10, m_crew->m_crew->m_attack, m_crew->m_crew->m_defence, m_crew->m_crew->m_type);

	}
	else
	{
		FieldUnit::Setup(crew->m_crew->m_graphic, crew->m_crew->m_size, crew->m_crew->m_individual, crew->m_crew->m_hard, crew->m_crew->m_heavy, health, m_crew->m_crew->m_attack, m_crew->m_crew->m_defence, m_crew->m_crew->m_type);
	}
}

bool CrewFieldUnit::Damage(int damage)
{
	m_damage += damage;
	return FieldUnit::Damage(damage);
}
bool CrewFieldUnit::Damage(int damage, bool heavy)
{
	m_damage += damage;
	return FieldUnit::Damage(damage,heavy);
}
void CrewFieldUnit::Cleanup()
{
	if (m_individual == true)
	{
		if (m_hp == 0)
		{
			m_crew->m_count = 0;
		}
	}
	else
	{
		m_crew->m_count -= m_damage;

	}
}