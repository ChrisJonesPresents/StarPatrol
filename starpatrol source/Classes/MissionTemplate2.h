#ifndef MISSIONTEMPLATE2
#define MISSIONTEMPLATE2

#include "MissionNode_Template.h"
#include "CBReader.h"
#include "Mission2.h"

class MissionTemplate2
{
public:
	std::string *m_tags;
	int m_numtags;
	MissionNode_Template **m_rootnodes;
	int m_numnodes;
	MissionTemplate2(){};
	MissionTemplate2(CBReader *reader);
	~MissionTemplate2();
	virtual Mission2 *getMission(int DC);
	void AddNode(int DC, Mission2 *mission);
	int m_DC;
};

class MissionThreatTemplate : public MissionTemplate2
{
public:
	MissionThreatTemplate(CBReader *reader);
	~MissionThreatTemplate();
	Mission2 *getMission(int DC);
	int m_strength, m_timelimit;
	std::string m_prompts[3];

};
#endif