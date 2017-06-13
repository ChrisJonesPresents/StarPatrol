#include "Star.h"
#include "ReferenceLibrary.h"

Star::Star(cocos2d::Vec2 p)
{
	m_explored = false;
	m_position = p;
	m_name = ReferenceLibrary::getInstance()->getName();

	//random star appearance
	int r = 2+rand() % 6;
	m_graphic.x = r % 2;
	m_graphic.y = (r / 2);
	m_type = (StarType)r;
	m_locations = NULL;
	m_numloc = 0;
	m_mission = NULL;
}

Star::Star(cocos2d::Vec2 p, StarType type)
{
	m_explored = true;
	m_mission = NULL;
	m_position = p;
	m_name = ReferenceLibrary::getInstance()->getName();
	m_type = type;
	int r = m_type;
	m_graphic.x = r % 2;
	m_graphic.y = (r / 2);

	if (m_type == ST_BLACK)
	{
		m_explored = true;
		m_locations = new Location*[1];
		m_numloc = 1;
		//build warpgate
		m_locations[0] = new Location(LT_WARPGATE, bytepos(2, 2));
	}
}

void Star::Save(FILE *file)
{
	fwrite((void*)&m_explored, sizeof(bool), 1, file);
	if (m_mission != NULL)
	{
		char v = 42;
		fwrite((void*)&v, sizeof(char), 1, file);
		m_mission->Save(file);
	}
	else
	{
		char v = 17;
		fwrite((void*)&v, sizeof(char), 1, file);
	}
	//save position
	float x = m_position.x; float y = m_position.y;
	fwrite((void*)&y, sizeof(float), 1, file);
	fwrite((void*)&x, sizeof(float), 1, file);
	//save name
	int check = m_name.length();
	char *buffer = new char[m_name.length() + 1];
	memcpy((void*)&buffer[0], (void*)m_name.c_str(), m_name.length());
	buffer[m_name.length()] = ' ';
	fwrite((void*)&buffer[0], sizeof(char), m_name.length() + 1, file);
	//save type
	fwrite((void*)&m_type, sizeof(int), 1, file);
	//save graphic
	x = m_graphic.x; y = m_graphic.y;
	fwrite((void*)&y, sizeof(float), 1, file);
	fwrite((void*)&x, sizeof(float), 1, file);

	if (m_explored == true)
	{
		//save numloc
		fwrite((void*)&m_numloc, sizeof(int), 1, file);
		//save locations
		for (int i = 0; i < m_numloc; i++)
		{
			if (m_locations[i]!=NULL)
			{
				char v = 42;
				fwrite((void*)&v, sizeof(char), 1, file);
				m_locations[i]->Save(file);
			}
			else
			{
				char v = 17;
				fwrite((void*)&v, sizeof(char), 1, file);
			}
		}
	}

	char v = -64;
	fwrite((void*)&v, sizeof(char), 1, file);
}

Star::Star()
{

}

bool Star::Load(CBReader *reader)
{
	m_explored = reader->ReadChar();
	char v = reader->ReadChar();
	if (v == 42)
	{
		m_mission = Mission2::LoadMission(reader);
	}
	else
	{
		m_mission = NULL;
	}
	m_position = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
	m_name = reader->ReadString();
	m_type = (StarType)reader->ReadBInt();
	float y = reader->ReadBFloat();
	m_graphic = Vec2(reader->ReadBFloat(),y);
	if (m_explored == true)
	{
		m_numloc = reader->ReadBInt();
		m_locations = new Location *[m_numloc];
		for (int i = 0; i < m_numloc; i++)
		{
			v = reader->ReadChar();
			if (v == 42)
			{
				m_locations[i] = new Location(reader);
			}
			else
			{
				m_locations[i] = NULL;
			}
		}
	}
	else
	{
		m_numloc = 0;
		m_locations = NULL;
	}

	char va = reader->ReadChar();
	if (va != -64)
	{
		return false;
	}
	return true;
}

Star::~Star()
{
	if (m_locations != NULL)
	{
		for (int i = 0; i < m_numloc; i++)
		{
			if (m_locations[i] != NULL)
			{
				delete m_locations[i];
			}
		}

		delete[]m_locations;
	}
	if (m_mission != NULL)
	{
		delete m_mission;
	}
}

cocos2d::Vec2 Star::getPosition()
{
	return m_position;
}


void Star::Explore(float DC)
{
	if (m_explored == false)
	{
		m_explored = true;

		//generate contents
		if (m_type == ST_BLUEGIANT || m_type == ST_REDGIANT || m_type == ST_YELLOWGIANT)
		{
			m_numloc = 8;
		}
		else
		{
			m_numloc = 4;
		}
		Generate(DC);
	}
}

void Star::Generate(float DC)
{
	m_locations = new Location *[m_numloc];
	for (int i = 0; i < m_numloc; i++)
	{	
		if (i<m_numloc - 1)
		{
			m_locations[i] = new Location(TypeGen(), VecGen(i));
			m_locations[i]->GenMission(DC);
		}
		else
		{
			m_locations[i] = NULL;
		}
	}
	if (rand() % 3==0)
	{
		m_mission = ReferenceLibrary::getInstance()->getQuestEngine()->genStub(DC);
	}
}

bytepos Star::VecGen(int index)
{
	while (true)
	{
		bool b = false;
		bytepos p = bytepos(rand() % 8, rand() % 8);
		if (p.m_x<3 || p.m_x>4)
		{
			if (p.m_y<3 || p.m_y>4)
			{
				for (int i = 0; i < index; i++)
				{
					if (m_locations[i] != NULL)
					{
						if (p.m_x == m_locations[i]->getPos().m_x && p.m_y == m_locations[i]->getPos().m_y)
						{
							b = true;
							break;
						}
					}
				}
				if (b == false)
				{
					return p;
				}
			}
		}

	}
}

LocationType Star::TypeGen()
{
	int r = rand() % 10;
	switch (r)
	{
	case 0:
		switch (m_type)
		{
		case ST_RED:
			return LT_HELL;
		case ST_REDGIANT:
			return LT_HELL;
		case ST_BLUE:
			return LT_ICE;
		case ST_BLUEGIANT:
			return LT_ICE;
		case ST_YELLOW:
			return LT_GARDEN;
		case ST_YELLOWGIANT:
			return LT_GARDEN;
		}
	case 1:
		return LT_HELL;
	case 2:
		return LT_ICE;
	case 3:
		return LT_GARDEN;
	case 4:
		return LT_GAS;
	case 5:
		return LT_GAS;
	case 6:
		return LT_BARREN;
	case 7:
		return LT_BARREN;
	case 8:
		return LT_ASTEROID;
	case 9:
		return LT_ASTEROID;
	}
	return LT_GAS;
}

std::string Star::getName()
{
	return m_name;
}

bool Star::getExplored()
{
	return m_explored;
}

cocos2d::Vec2 Star::getGraphic()
{
	return m_graphic;
}

StarType Star::getType()
{
	return m_type;
}

int  Star::getNumLoc()
{
	return m_numloc;
}

Location ** Star::getLocations()
{
	return m_locations;
}

Mission2 *Star::getMission()
{
	return m_mission;
}

void Star::setMission(Mission2 *mission)
{
	m_mission = mission;
}

void Star::MissionGen(int d)
{
	if (m_type != ST_BLACK)
	{
		int c = rand() % 4;
		if (c > 0)
		{
			if (m_numloc > 0)
			{
				//build a new location thing
				int r = rand() % m_numloc;
				if (m_locations[r] != NULL)
				{

					m_locations[r]->setMission(new Mission2(d, NULL));
				}
			}
		}
		else
		{
			//build a new star event
			if (m_mission != NULL)
			{
				m_mission = new Mission2(d, NULL);
			}
		}
	}
}