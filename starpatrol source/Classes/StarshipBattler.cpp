#include "StarshipBattler.h"
#include "NPCship.h"
#include "Playership.h"
#include "Module_commodity.h"

StarshipBattler::StarshipBattler(Starship *ship)
{
	m_ship = ship;
	m_command = 0;

}

Starship *StarshipBattler::getShip()
{
	return m_ship;
}

float StarshipBattler::getCommand()
{
	return m_command;
}

void StarshipBattler::subtractCommand(int c)
{
	m_command -= c;
}

int StarshipBattler::Update(float dt)
{
	int r = -1;
	if (m_command < 100)
	{
		m_command += (dt*3);
		if (m_command>100)
		{
			m_command = 100;
		}
	}
	if (m_status.size()>0)
	{
		for (int i = 0; i < m_status.size(); i++)
		{
			m_status.at(i)->timeleft -= dt;
			if (m_status.at(i)->timeleft <= 0)
			{
				m_ship->RemoveState(*m_status.at(i));

				m_status.eraseObject(m_status.at(i));
				r = i;
				return r;
				break;
			}
		}
	}
	return r;
}

int StarshipBattler::getNumStatus()
{
	return m_status.size();
}
ShipStatus *StarshipBattler::getStatus(int i)
{
	return m_status.at(i);
}

void StarshipBattler::Addstatus(ShipStatus *s, Vec2 p)
{
	m_ship->ApplyState(*s, p);
	m_status.pushBack(s);
}

Ability_Space *StarshipBattler::getAI()
{
	NPCship *npc = (NPCship*)m_ship;
	
	int r = 0;
	if (npc->getNumability() > 1)
	{
		r = rand() % npc->getNumability();
	}

	m_command = m_command - npc->getAbility(r)->getCommandcost();

	return npc->getAbility(r);
}

void StarshipBattler::RandomizeCommand()
{
	m_command = rand() % 50;
}

void  StarshipBattler::ConsumeOrdinance(int ordinance)
{
	Playership *ship = (Playership*)m_ship;
	for (int i = 0; i < ship->getNumModules(); i++)
	{
		if (ship->getModule(i) != NULL)
		{
			if (ship->getModule(i)->getType() == MT_COMMODITY)
			{
				Module_commodity *mc = (Module_commodity*)ship->getModule(i); 
				if (mc->getCargoType() == CT_ORDINANCE)
				{
					if (mc->getStack()>ordinance)
					{
						mc->setStack(mc->getStack() - ordinance);
						ordinance = 0;
					}
					else
					{
						ordinance -= mc->getStack();
						ship->InstallModule(NULL, i, true);
					}
				}
			}
		}
		if (ordinance == 0)
		{
			break;
		}
	}
}

void StarshipBattler::SaveState(FILE *file)
{
	//save command
	fwrite((void*)&m_command, sizeof(float), 1, file);

	int v = m_status.size();
	fwrite((void*)&v, sizeof(int), 1, file);
	for (int i = 0; i < m_status.size(); i++)
	{
		fwrite((void*)&m_status.at(i)->m_type, sizeof(int), 1, file);
		fwrite((void*)&m_status.at(i)->m_value, sizeof(int), 1, file);
		fwrite((void*)&m_status.at(i)->m_duration, sizeof(int), 1, file);
		fwrite((void*)&m_status.at(i)->timeleft, sizeof(float), 1, file);
	}
}

void StarshipBattler::LoadState(CBReader *reader)
{

	//load command
	m_command = reader->ReadBFloat();
	int v = reader->ReadBInt();
	if (v > 0)
	{
		for (int i = 0; i < v; i++)
		{
			ShipStatus *shipstatus = new ShipStatus();
			shipstatus->m_type = (Shipmod) reader->ReadBInt();
			shipstatus->m_value = reader->ReadBInt();
			shipstatus->m_duration = reader->ReadBInt();
			shipstatus->timeleft = reader->ReadBFloat();
			m_status.pushBack(shipstatus);

		}
	}

}
