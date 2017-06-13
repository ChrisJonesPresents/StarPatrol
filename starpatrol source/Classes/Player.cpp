#include "Player.h"
#include "ReferenceLibrary.h"
#include "Module_commodity.h"
#include "GameManager.h"
Player::Player()
{

	m_transit = 0;
	for (int i = 0; i < 3; i++)
	{
		m_items[i] = NULL;
	}
	m_exp = 0;
	m_level = 1;

}

Player::~Player()
{
	delete m_ship;

	for (int i = 0; i < m_numofficers; i++)
	{
		if (m_officers[i] != NULL)
		{
			delete m_officers[i];
		}
	}
	delete[]m_officers;

	for (int i = 0; i < m_numpotions; i++)
	{
		if (m_potions[i] != NULL)
		{
			delete m_potions[i];
		}

	}
	delete [] m_potions;
}
Vec2 Player::getPos0()
{
	return Vec2(m_x0, m_y0);
} 
Vec2 Player::getPos1()
{
	return Vec2(m_x1, m_y1);
}
float Player::getTransit()
{
	return m_transit;
}

Playership *Player::getShip()
{
	return m_ship;

}

void Player::setPos(Vec2 p)
{
	m_x0 = p.x;
	m_y0 = p.y;
}

PotionStack::PotionStack(char stack, Potion *potion)
{
	m_stack = stack;
	m_potion = potion;
}

void Player::NewCaptain(int x, int y, std::string capname, std::string shipname, bool early, int timestamp)
{
	m_x0 = x;
	m_y0 = y;

	m_money = 250;
	//new ship
	int v = 0;
	m_ship = new Playership(v);

	//install laser
	m_ship->InstallModule(ReferenceLibrary::getInstance()->getModule(0), 0);
	//install missile launcher
	m_ship->InstallModule(ReferenceLibrary::getInstance()->getModule(4), 1);
	//install 20 units of ordinance
	m_ship->InstallModule(new Module_commodity(CT_ORDINANCE, 20), 2);


	m_numofficers = 6;
	m_officers = new Officer *[m_numofficers];
	for (int i = 0; i < m_numofficers; i++)
	{
		m_officers[i] = NULL;
	}
	m_numpotions = 6;
	m_potions = new PotionStack *[m_numpotions];
	for (int i = 0; i < 6; i++)
	{
		m_potions[i] = NULL;
	}

	m_officers[0] = new Officer(ReferenceLibrary::getInstance()->getOfficer(1));
	m_officers[0]->setName(capname);

	for (int i = 0; i < 3; i++)
	{
		m_items[i] = NULL;
	}
	if (early == true)
	{
		m_items[0] = ReferenceLibrary::getInstance()->getItem(0);
		m_logbook.AddEntry(true, timestamp, "Captain, welcome to the sector. The warp gate around the black hole is your connection to civilized space and will provide basic supplies until you can provide for yourself");
		m_logbook.AddEntry(true, timestamp, "As part of the galactic settlement act you have been provided with a colony license. Please be aware that it can only be used in a star without threats or anomalies.");



	}


	m_ship->setCrewCount();

	m_ship->setName(shipname);
}

void Player::CompactPotions()
{

	for (int i = 0; i < 5; i++)
	{
		if (m_potions[i] != NULL)
		{
			for (int j = i+1; j < 6; j++)
			{
				if (m_potions[j] != NULL && i != j)
				{
					if (m_potions[i]->m_potion == m_potions[j]->m_potion && m_potions[i]->m_stack>=m_potions[j]->m_stack && m_potions[i]->m_stack<9)
					{
						int max = 9 - m_potions[i]->m_stack;
						int move = m_potions[j]->m_stack;
						//move potions from stack j to stack i 
						if (max >= move)
						{
							delete m_potions[j];
							m_potions[i]->m_stack += move;
							m_potions[j] = NULL;
						}
						else
						{
							m_potions[i]->m_stack = 9;
							m_potions[j]->m_stack -= max;
						}
					}
				}
			}
		}
	}
}

Item *Player::getItem(int i)
{
	return m_items[i];
}

int Player::getMoney()
{
	return m_money;
}

void Player::setItem(int i, Item *item)
{
	m_items[i] = item;
}

Officer *Player::getOfficer(int i)
{
	return m_officers[i];
}
int Player::getNumOfficers()
{
	return m_numofficers;
}
void Player::setOfficer(Officer *officer, int slot)
{
	m_officers[slot] = officer;
}

void Player::setMoney(int money)
{
	m_money = money;
}

int Player::getXP()
{
	return m_exp;
}

void Player::addXP(int v)
{
	m_exp += v;
}

int Player::getLevel()
{
	return m_level;
}
void Player::addLevel(int v)
{
	m_level += v;
}

Logbook *Player::getLogbook()
{
	return &m_logbook;
}

void Player::Save(FILE *file)
{
	//save location
	fwrite((void*)&m_money, sizeof(short), 1, file);
	fwrite((void*)&m_x0, sizeof(short), 1, file);
	fwrite((void*)&m_y0, sizeof(short), 1, file);
	//save level
	fwrite((void*)&m_level, sizeof(short), 1, file);
	//save exp
	fwrite((void*)&m_exp, sizeof(short), 1, file);
	//save num officers
	fwrite((void*)&m_numofficers, sizeof(short), 1, file);
	//save officers
	for (int i = 0; i < m_numofficers; i++)
	{
		if (m_officers[i] != NULL)
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
			m_officers[i]->Save(file);
		}
		else
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
	}

	//save potions
	fwrite((void*)&m_numpotions, sizeof(short), 1, file);
	for (int i = 0; i < m_numpotions; i++)
	{
		if (m_potions[i] != NULL)
		{
			char v = m_potions[i]->m_potion->m_uid;;
			fwrite((void*)&v, sizeof(char), 1, file);
			v = m_potions[i]->m_stack;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
		else
		{
			char v = -1;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
	}

	//save ship
	m_ship->Save(file);
	//save items
	for (int i = 0; i < 3; i++)
	{
		if (m_items[i] != NULL)
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
			int val = m_items[i]->m_uid;
			fwrite((void*)&val , sizeof(int), 1, file);
		}
		else
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
	}
	m_logbook.Save(file);
}

Player::Player(CBReader *reader)
{
	m_money = reader->ReadBShort();
	//load location
	m_x0 = reader->ReadBShort();
	m_y0 = reader->ReadBShort();
	//load level
	m_level = reader->ReadBShort();
	//load exp
	m_exp = reader->ReadBShort();
	//load num officers
	m_numofficers = reader->ReadBShort();
	//load officers
	m_officers = new Officer*[m_numofficers];
	for (int i = 0; i < m_numofficers; i++)
	{
		char v = reader->ReadChar();
		if (v == 42)
		{
			m_officers[i] = new Officer(reader);
		}
		else
		{
			m_officers[i] = NULL;
		}
	}

	//load potions
	m_numpotions = reader->ReadBShort();
	m_potions = new PotionStack *[m_numpotions];
	for (int i = 0; i < m_numpotions; i++)
	{
		char v = reader->ReadChar();
		PotionStack *scan;
		if (v != -1)
		{
			
			scan = new PotionStack(reader->ReadChar(), ReferenceLibrary::getInstance()->getPotion(v));
		}
		else
		{
			scan = NULL;
		}
		m_potions[i] = scan;

	}

	//load ship
	m_ship = new Playership(reader);
	//load items
	for (int i = 0; i < 3; i++)
	{
		char v = reader->ReadChar();
		if (v == 42)
		{
			m_items[i] = ReferenceLibrary::getInstance()->getItem(reader->ReadBInt());
		}
		else
		{
			m_items[i] = NULL;
		}
	}

	m_logbook = Logbook(reader);

	m_transit = 0;
	
}

PotionStack *Player::getPotion(int i)
{
	return m_potions[i];
}

int Player::getNumPotion()
{
	return m_numpotions;
}

void Player::setPotion(PotionStack *stack, int slot)
{
	m_potions[slot] = stack;
}

void Player::ResetPotions(Potion *potions[3])
{
	for (int i = 0; i < 3; i++)
	{
		if (potions[i] != NULL)
		{
			for (int j = 0; j < 6; j++)
			{
				if (m_potions[i] != NULL)
				{
					if (m_potions[j]->m_potion == potions[i] && m_potions[j]->m_stack<9)
					{
						m_potions[j]->m_stack++;
						potions[i] = NULL;
						break;
					}
				}
			}
		}

		if (potions[i] != NULL)
		{
			//find new slot
			for (int j = 0; j < 6; j++)
			{
				if (m_potions[j] == NULL)
				{
					m_potions[j] = new PotionStack(1, potions[i]);
					potions[i] = NULL;
					break;
				}
			}
		}

	}
}

void Player::PotionCheck()
{
	for (int i = 0; i < m_numpotions; i++)
	{
		if (m_potions[i] != NULL)
		{
			if (m_potions[i]->m_stack < 1)
			{
				delete m_potions[i];
				m_potions[i] = NULL;
			}
		}
	}
}