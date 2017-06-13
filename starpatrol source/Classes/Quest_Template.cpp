#include "Quest_Template.h"


Quest_Template::Quest_Template(CBReader *reader)
{
	std::string str = reader->ReadString();
	//load category
	m_category = reader->ReadInt();
	//load location type
	m_type = ReadType(reader);
	//load DC
	m_DC = reader->ReadInt();
	//load timelimits
	m_timelimit = reader->ReadInt();
	//load briefings
	for (int i = 0; i < 3; i++)
	{
		m_briefing[i] = LoadLongString(reader);
	}
	//load rewards
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_rewards[i][j] = reader->ReadInt();
		}
	}

	//load quest mission
	m_mission = new MissionTemplate2(reader);
}

Quest_Template::~Quest_Template()
{
	delete m_mission;
}
Quest *Quest_Template::getQuest()
{
	Quest *quest = new Quest();
	
	


	return quest;
}