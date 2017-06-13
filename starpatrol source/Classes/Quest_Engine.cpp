#include "Quest_Engine.h"

std::string FpathfromI(int i)
{
	switch (i)
	{
	case 0:
		return "data/missions/omni_missions.txt";
	case 1:
		return "data/missions/star_missions.txt";
	case 2:
		return "data/missions/space_missions.txt";
	case 3:
		return "data/missions/planet_missions.txt";
	case 4:
		return "data/missions/gas_missions.txt";
	case 5:
		return "data/missions/barren_missions.txt";
	case 6:
		return "data/missions/ice_missions.txt";
	case 7:
		return "data/missions/hell_missions.txt";
	case 8:
		return "data/missions/garden_missions.txt";
	case 9:
		return "data/missions/asteroid_missions.txt";
	case 10:
		return "data/missions/station_missions.txt";
	case 11:
		return "data/missions/derelict_missions.txt";
	case 12:
		return "data/missions/anomaly_missions.txt";
	case 13:
		return "data/missions/threatmissions.txt";
	case 14:
		return "data/missions/waitmissions.txt";
	}

	return "";

}

Quest_Engine::Quest_Engine()
{
	m_uid = 0;
	for (int i = 0; i < 15; i++)
	{
		m_templates[i] = NULL;
		ssize_t s;
		unsigned long length;
		char b[3]; itoa(i, &b[0], 10);

		std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(FpathfromI(i));
		unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

		while (s < 1)
		{
			buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
		}
		length = s;

		CBReader *cbr = new CBReader(buffer, length);
		//determine how many options there are
		m_counts[i] = cbr->ReadInt();
		m_templates[i] = new MissionTemplate2 *[m_counts[i]];
		for (int j = 0; j < m_counts[i]; j++)
		{
			if (i == 13)
			{
				m_templates[i][j] = new MissionThreatTemplate(cbr);
			}
			else
			{
				m_templates[i][j] = new MissionTemplate2(cbr);
			}

		}
		delete cbr;
	}

	m_templates[15] = NULL;
	ssize_t s;
	unsigned long length;
	char b[3];// itoa(i, &b[0], 10);

	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/quests.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s < 1)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);
	//determine how many options there are
	m_counts[15] = cbr->ReadInt();
	m_quests = new Quest_Template *[m_counts[15]];
	for (int j = 0; j < m_counts[15]; j++)
	{
		m_quests[j] = new Quest_Template(cbr);
	}
	delete cbr;

}

Quest_Engine::~Quest_Engine()
{
	for (int i = 0; i < 15; i++)
	{
		if (m_templates[i] != NULL)
		{
			for (int j = 0; j < m_counts[i]; j++)
			{
				delete m_templates[i][j];
			}
			delete [] m_templates[i];
		}
	}
	if (m_counts[15]>0)
	{
		for (int i = 0; i < m_counts[15]; i++)
		{
			delete m_quests[i];
		}
		delete [] m_quests;
	}
}

void Quest_Engine::AddMission(int listnum, int DC, Mission2 *mission)
{
	int length = 0;
	for (int i = 0; i < m_counts[listnum]; i++)
	{
		if (m_templates[listnum][i]->m_DC <= DC)
		{
			length++;
		}
		else
		{
			break;
		}
	}
	if (length > 1)
	{
		int r = rand() % length;
		m_templates[listnum][r]->AddNode(DC, mission);
	}
	else
	{
		m_templates[listnum][0]->AddNode(DC, mission);
	}
}

Mission2 *Quest_Engine::genStub(int DC)
{
	Mission2 *mission = new Mission2(DC, NULL);
	mission->m_uid = m_uid;
	m_uid++;
	return mission;
}

void Quest_Engine::Save(FILE *file)
{
	fwrite((void*)&m_uid, sizeof(int), 1, file);
}
void Quest_Engine::setUID(int value)
{
	m_uid = value;
}

void Quest_Engine::Convertstub(Mission2 *mission, Location *location)
{

	if (location == NULL)
	{
		AddMission(1, mission->getDC(), mission);
	}
	else
	{
		int r = rand() % 4;
		if (r == 0)
		{
			AddMission(0, mission->getDC(), mission);
		}
		if (r == 1)
		{
			if (location->getType() == LT_ASTEROID || location->getType() == LT_GAS || location->getType() == LT_STATION)
			{
				AddMission(2, mission->getDC(), mission);
			}
			else
			{
				AddMission(3, mission->getDC(), mission);
			}
		}
		if (r > 1)
		{
			switch (location->getType())
			{
			case LT_GAS:
				AddMission(4, mission->getDC(), mission);
				break;
			case LT_BARREN:
				AddMission(5, mission->getDC(), mission);
				break;
			case LT_ICE:
				AddMission(6, mission->getDC(), mission);
				break;
			case LT_HELL:
				AddMission(7, mission->getDC(), mission);
				break;
			case LT_GARDEN:
				AddMission(8, mission->getDC(), mission);
				break;
			case LT_ASTEROID:
				AddMission(9, mission->getDC(), mission);
				break;
			case LT_STATION:
				AddMission(10, mission->getDC(), mission);
				break;
			case LT_ANOMALY:
				AddMission(11, mission->getDC(), mission);
				break;
			}
		}
	}

}

Mission2 *Quest_Engine::AddThreat(int DC, int Timestamp, std::string location)
{
	int count = 0;
	for (int i = 0; i < m_counts[13]; i++)
	{
		if (m_templates[13][i]->m_DC <= DC)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	int r = rand() % count;
	MissionThreatTemplate *missiontemplate = (MissionThreatTemplate*)m_templates[13][r];
	MissionThreat *mission = (MissionThreat*)missiontemplate->getMission(DC);
	
	for (int i = 0; i < 3; i++)
	{
		std::string str = missiontemplate->m_prompts[i];
		str = myreplace(str, "TAG", mission->getCurrentNode()->m_tag);
		std::string date = "";
		char buffer[6];
		itoa(Timestamp % 30, &buffer[0], 10);
		date.append(buffer); date.append("/");
		itoa((((Timestamp+mission->getTime()) / 30) % 12)+1, &buffer[0], 10);
		date.append(buffer); date.append("/");
		itoa(2115+(Timestamp / 360), &buffer[0], 10);
		date.append(buffer);
		if (str.find("DATE") != std::string::npos)
		{
			str = myreplace(str, "DATE", date);
		}
		str = myreplace(str, "COLONY", location);
		mission->m_prompts[i] = str;
	}
	mission->m_uid = m_uid;
	m_uid++;
	return mission;
}

Mission2 *Quest_Engine::AddWait(int DC)
{
	int count = 0;
	for (int i = 0; i < m_counts[14]; i++)
	{
		if (m_templates[14][i]->m_DC <= DC)
		{
			count++;
		}
		else
		{
			break;
		}
	}
	int r = rand() % count;
	MissionTemplate2 *missiontemplate = m_templates[14][r];
	Mission2 *mission = missiontemplate->getMission(DC);
	mission->m_uid++;
	m_uid++;

	return mission;




}