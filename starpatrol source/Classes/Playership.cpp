#include "Playership.h"
#include "Module_commodity.h"
#include "Module_ability.h"
#include "Module_Booster.h"
#include "ReferenceLibrary.h"

Playership::Playership(byte s, std::string name)
{
	m_name = name;
	m_graphicsize = cocos2d::Vec2(32, 32);

	m_fuel = 40; m_fuelmax = 40;
	m_hp = 80; m_hpmax = 80;
	m_crewmax = 40;
	
	m_s = s;
	switch (s)
	{
	case 0:
		m_nummodules = 6;

		break;
	case 1:
		m_nummodules = 9;

		break;

	case 2:
		m_nummodules = 12;

		break;
	}

	m_modules = new Module *[m_nummodules];
	for (int i = 0; i < m_nummodules; i++)
	{
		m_modules[i] = NULL;
	}
	CalcStats();
	for (int i = 0; i < 4; i++)
	{
		m_resources[i] = 0;
	}
}

void Playership::Setbasestats(byte size)
{
	for (int i = 0; i < 3; i++)
	{
		m_offences[i] = 100;
		m_defences[i] = 0;
	}
	switch (size)
	{
	case 0:
		m_graphicpos = cocos2d::Vec2(0, 0);
		m_hpmax = 80;
		m_crewmax = 40;
		m_fuelmax = 40;

		m_speed = 4;
		break;

	case 1:
		m_graphicpos = cocos2d::Vec2(32, 0);
		m_hpmax = 120;
		m_crewmax = 60;
		m_fuelmax = 60;

		m_speed = 4;
		break;

	case 2:
		m_graphicpos = cocos2d::Vec2(64, 0);
		m_hpmax = 160;
		m_crewmax = 80;
		m_fuelmax = 80;

		m_speed = 4;
		break;

	}
}

void Playership::CalcStats()
{
	Setbasestats(m_s);
	for (int i = 0; i < m_nummodules; i++)
	{
		if (m_modules[i] != NULL)
		{
			if (m_modules[i]->getType() == MT_BOOSTER)
			{
				Module_Booster *mb = (Module_Booster*)m_modules[i];
				switch (mb->getModier())
				{
				case SM_fuelcap:
					m_fuelmax += mb->getValue();
					break;
				case SM_hpcap:
					m_hpmax += mb->getValue();
					break;
				case SM_crewcap:
					m_crewmax += mb->getValue();
					break;
				case SM_speed:
					m_speed += mb->getValue();
					break;
				case SM_deflect:
					m_defences[0] += mb->getValue();
					m_defencegraphic[0][0] = mb->getGraphic();
					break;
				case SM_intercept:
					m_defences[1] += mb->getValue();
					m_defencegraphic[0][1] = mb->getGraphic();
					break;
				case SM_dodge:
					m_defences[2] += mb->getValue();
					break;
				case SM_BEAM:
					m_offences[0] += mb->getValue();
					break;
				case SM_PROJECTILE:
					m_offences[2] += mb->getValue();
				case SM_MISSILE:
					m_offences[1] += mb->getValue();
				}

			}
		}
	}
}

Playership::~Playership()
{
	for (int i = 0; i < m_nummodules; i++)
	{
		if (m_modules[i] != NULL)
		{
			if (m_modules[i]->getType() == MT_COMMODITY)
			{
				delete m_modules[i];
			}
		}
	}
	delete[] m_modules;
}

float Playership::Refuel(float fuel)
{
	if (m_fuelmax - m_fuel>fuel)
	{
		m_fuel += fuel;
		return 0;
	}
	else
	{
		fuel = fuel - (m_fuelmax - m_fuel);
		m_fuel = m_fuelmax;
		return fuel;
	}
}

int Playership::getFuel()
{
	return m_fuel;
}

int Playership::getMaxFuel()
{
	return m_fuelmax;
}

int  Playership::getFreeCrew()
{
	return m_crewmax - m_crewcount;

}

void Playership::setHP(int v)
{
	m_hp = v;
}

int Playership::getOrdinance()
{
	int T = 0;
	for (int i = 0; i < m_nummodules; i++)
	{
		if (m_modules[i] != NULL)
		{
			if (m_modules[i]->getType() == MT_COMMODITY)
			{
				Module_commodity *mc = (Module_commodity*)m_modules[i];
				if (mc->getCargoType() == CT_ORDINANCE)
				{
					T += mc->getStack();
				}
			}
		}
		
	}
	return T;
}

int Playership::getNumModules()
{
	return m_nummodules;
}

Module *Playership::getModule(int i)
{
	return m_modules[i];
}

void Playership::setFuel(int v)
{
	m_fuel = v;
}

void Playership::modFuel(float d)
{
	m_fuel -= d;

}

float Playership::getSpeed()
{
	return m_speed;

}


std::string Playership::getName()
{
	return m_name;
}

byte Playership::getS()
{
	return m_s;
}

Vector <ShipCrew*> *Playership::getCrew()
{
	return &m_crew;
}

void Playership::setCrewCount()
{
	int c = 0;
	for (int i = 0; i < m_crew.size(); i++)
	{
		if (m_crew.at(i) != NULL)
		{
			ShipCrew *shipcrew = (ShipCrew*)m_crew.at(i);
			c += shipcrew->m_count;
		}
	}
	m_crewcount = c;
}

int Playership::getMaxCrew()
{
	return m_crewmax; 
}

bool Playership::InstallModule(Module *module, int slot, bool erase)
{
	if (module != NULL)
	{
		if (m_modules[slot] == NULL)
		{
			m_modules[slot] = module;
			Setbasestats(m_s);
			CalcStats();
			return true;
		}
		else
		{
			if (module->getType() == MT_COMMODITY && m_modules[slot]->getType() == MT_COMMODITY)
			{
				Module_commodity *mc0 = (Module_commodity*)module;
				Module_commodity *mc1 = (Module_commodity*)m_modules[slot];
				if (mc0->getCargoType() == mc1->getCargoType())
				{
					if (mc1->getStack() < 20)
					{
						char d = 20-mc1->getStack();
						if (d>mc0->getStack())
						{
							mc1->setStack(mc1->getStack() + mc0->getStack());

						}
						else
						{
							mc1->setStack(20);
							mc0->setStack(mc0->getStack() - d);
						}
						if (mc0->getStack() == 0)
						{
							return true;
						}
						else
						{
							return false;
						}

					}
				}
			}
			
			return false;
		}
	}
	else
	{
		if (erase==true && m_modules[slot]->getType() == MT_COMMODITY)
		{
			
			delete m_modules[slot];
		}
		Setbasestats(m_s);
		CalcStats();
		m_modules[slot] = NULL;

		return true;
	}
}

void Playership::ModResource(int i, int amount)
{
	m_resources[i] =m_resources[i]+ amount;
}

int Playership::getResource(int i)
{
	return m_resources[i];
}

int Playership::getCrewCount()
{
	return m_crewcount;
}

bool Playership::HireCrew(Crew *crew, int count)
{
	for (int i = 0; i < m_crew.size(); i++)
	{
		ShipCrew *crewc = (ShipCrew*)m_crew.at(i);
		if (crewc->m_crew == crew)
		{
			if (crewc->m_crew->m_individual == true)
			{
				return false;
			}
			else
			{
				crewc->m_count += count;
				return true;
			}
		}
	}
	ShipCrew *newcrew = new ShipCrew();
	newcrew->m_count = count;
	newcrew->m_crew = crew;
	
	m_crew.pushBack(newcrew);
	return true;
}

void Playership::Save(FILE *file)
{
	SaveLongString(m_name, file);
	//save class
	fwrite((void*)&m_s, sizeof(byte), 1, file);
	//save hp
	fwrite((void*)&m_hp, sizeof(int), 1, file);
	fwrite((void*)&m_hpmax, sizeof(int), 1, file);
	//save fuel
	fwrite((void*)&m_fuel, sizeof(float), 1, file);
	fwrite((void*)&m_fuelmax, sizeof(short), 1, file);
	//save speed
	fwrite((void*)&m_speed, sizeof(float), 1, file);
	//save module slots
	fwrite((void*)&m_nummodules, sizeof(short), 1, file);
	for (int i = 0; i < m_nummodules; i++)
	{
		if (m_modules[i] != NULL)
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
			if (m_modules[i]->getType() == MT_COMMODITY)
			{
				int id = -1;
				fwrite((void*)&id, sizeof(int), 1, file);
				Module_commodity *mc = (Module_commodity*)m_modules[i];
				char stack = mc->getStack();
				char type = mc->getCargoType();
				fwrite((void*)&type, sizeof(char), 1, file);
				fwrite((void*)&stack, sizeof(char), 1, file);
			}
			else
			{
				int id = m_modules[i]->getUID();
				fwrite((void*)&id, sizeof(int), 1,file);
			}

		}
		else
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
	}
	//save graphics
	fwrite((void*)&m_graphicpos.x, sizeof(float), 1, file);
	fwrite((void*)&m_graphicpos.y, sizeof(float), 1, file);
	fwrite((void*)&m_graphicsize.x, sizeof(float), 1, file);
	fwrite((void*)&m_graphicsize.y, sizeof(float), 1, file);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			fwrite((void*)&m_defencegraphic[i][j].x, sizeof(float), 1, file);
			fwrite((void*)&m_defencegraphic[i][j].y, sizeof(float), 1, file);
		}
	}
	
	//save stats
	for (int i = 0; i < 3; i++)
	{
		fwrite((void*)&m_offences[i], sizeof(int), 1, file);
		fwrite((void*)&m_defences[i], sizeof(int), 1, file);
	}
	
	for (int i = 0; i < 4; i++)
	{
		fwrite((void*)&m_resources[i], sizeof(short), 1, file);
	}

}

Playership::Playership(CBReader *reader)
{
	m_name = LoadLongString(reader);
	//load class
	m_s = reader->ReadChar();
	//load hp
	m_hp = reader->ReadBInt();
	m_hpmax = reader->ReadBInt();
	//load fuel
	m_fuel = reader->ReadBFloat();
	m_fuelmax = reader->ReadBShort();
	//load speed
	m_speed = reader->ReadBFloat();
	//load module slots
	m_nummodules = reader->ReadBShort();
	m_modules = new Module*[m_nummodules];
	for (int i = 0; i < m_nummodules; i++)
	{
		char v = reader->ReadChar();
		if (v == 42)
		{
			int id = reader->ReadBInt();
			if (id == -1)
			{

				CargoType type = (CargoType)reader->ReadChar();
				unsigned char stack = reader->ReadChar();
				m_modules[i] = new Module_commodity(type,stack);
			}
			else
			{
				m_modules[i] = ReferenceLibrary::getInstance()->getModule(id);
			}
		}
		else
		{
			m_modules[i] = NULL;
		}
	}
	//load graphics
	m_graphicpos = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
	m_graphicsize = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_defencegraphic[i][j] = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
		}
	}
	//load stats
	for (int i = 0; i < 3; i++)
	{
		m_offences[i] = reader->ReadBInt();
		m_defences[i] = reader->ReadBInt();
	}

	for (int i = 0; i < 4; i++)
	{
		m_resources[i] = reader->ReadBShort();
	}
}

void Playership::setName(std::string name)
{
	m_name = name;
}