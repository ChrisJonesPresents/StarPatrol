#include "Location.h"
#include "ReferenceLibrary.h"
#include "Quest_Engine.h"
#include "GameManager.h"
LocationType ReadType(CBReader *Reader)
{
	std::string str = Reader->ReadString();
	if (str.find("SPACE") != std::string::npos)
	{
		return LT_SPACE;
	}
	if (str.find("STAR") != std::string::npos)
	{
		return LT_STAR;
	}
	if (str.find("PLANET") != std::string::npos)
	{
		return LT_PLANET;
	}
	return LT_OMNI;
}
bytepos::bytepos()
{
	m_x = 0; m_y = 0;
}

bytepos::bytepos(byte x, byte y)
{
	m_x = x;
	m_y = y;
}

Location::Location(LocationType t, bytepos position)
{
	m_p = position;
	m_type = t;

	m_mission = NULL;
	m_colony = NULL;
	m_whatsprite = rand() % 4;
}

void Location::Save(FILE *file)
{
	fwrite((void*)&m_p.m_y, sizeof(byte), 1, file);
	fwrite((void*)&m_p.m_x, sizeof(byte), 1, file);
	fwrite((void*)&m_type, sizeof(int), 1, file);
	fwrite((void*)&m_whatsprite, sizeof(byte), 1, file);
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
	if (m_colony != NULL)
	{
		char v = 42;
		fwrite((void*)&v, sizeof(char), 1, file);
		m_colony->Save(file);
	}
	else
	{
		char v = 17;
		fwrite((void*)&v, sizeof(char), 1, file);
	}

}

Location::Location(CBReader *reader)
{
	char y = reader->ReadChar();
	m_p = bytepos(reader->ReadChar(), y);
	m_type = (LocationType)reader->ReadBInt();
	m_whatsprite = reader->ReadChar();
	char v = reader->ReadChar();
	if (v == 42)
	{
		m_mission = Mission2::LoadMission(reader);
	}
	else
	{
		m_mission = NULL;
	}
	v = reader->ReadChar();
	if (v == 42)
	{
		m_colony = new Colony(reader);
		GameManager::getInstance()->AddColony(this);
	}
	else
	{
		m_colony = NULL;
	}
}



bytepos Location::getPos()
{
	return m_p;
}

LocationType Location::getType()
{
	return m_type;
}

Location::~Location()
{
	if (m_mission != NULL)
	{
		delete m_mission;
	}
	if (m_colony != NULL)
	{
		delete m_colony;
	}
}

Mission2 *Location::getMission()
{
	return m_mission;
}

void Location::setMission(Mission2 *mission)
{
	m_mission = mission;
}

void Location::GenMission(float DC)
{
	m_mission = ReferenceLibrary::getInstance()->getQuestEngine()->genStub(DC);
}

byte Location::getWhatsprite()
{
	return m_whatsprite;
}

Colony *Location::getColony()
{
	return m_colony;
}

void Location::CreateColony(std::string name)
{
	switch (m_type)
	{
	case LT_ICE:
		m_colony = new Colony(CT_ICE, name);
			break;
	case LT_BARREN:
		m_colony = new Colony(CT_BARREN, name);
			break;
	case LT_HELL:
		m_colony = new Colony(CT_HELL, name);
			break;
	case LT_GARDEN:
		m_colony = new Colony(CT_GARDEN, name);
			break;
	}
	
}