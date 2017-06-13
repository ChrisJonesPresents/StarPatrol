#ifndef QUEST_TEMPLATE
#define QUEST_TEMPLATE

#include "MissionTemplate2.h"
#include "Location.h"
#include "CBReader.h"
#include "Quest.h"

class Quest_Template
{
public:
	int m_category;
	MissionTemplate2 *m_mission;
	int m_DC, m_timelimit;
	std::string m_briefing[3];
	int m_rewards[3][2];
	LocationType m_type;
	Quest_Template(CBReader *reader);
	~Quest_Template();
	Quest *getQuest();

};


#endif