#ifndef QUEST_ENGINE
#define QUEST_ENGINE

#include "Location.h"
#include "Mission.h"
#include "Mission_Template.h"
#include "Mission2.h"
#include "MissionTemplate2.h"
#include "MissionNode_Template.h"
#include "Quest_Template.h"

class Quest_Engine
{
	int m_counts[16];
	MissionTemplate2 **m_templates[15];
	Quest_Template **m_quests;
	//Mission *RetrieveMission(int i, float DC);

	//Mission *BuildMission(Mission_Template *missiontemplate);
	//Mission_Stage *buildStage(Template_Stage *stage);
	void AddMission(int listnum, int DC, Mission2 *mission);
	int m_uid;
public:
	Quest_Engine();
	~Quest_Engine();

	//Mission *genMission(Location *location, float DC);
	Mission2 *genStub(int DC);
	void Convertstub(Mission2 *mission, Location *location);
	Mission2 *AddThreat(int DC, int Timestamp, std::string location);
	Mission2 *AddWait(int DC);
	void Save(FILE *file);
	void setUID(int value);
};

#endif