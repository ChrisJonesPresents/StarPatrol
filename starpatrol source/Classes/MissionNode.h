#ifndef MISSIONNODE
#define MISSIONNODE

#include "MissionNode_Template.h"
#include "Common.h"

class MissionNode
{
public:
	int m_variables[7];
	int m_dc;
	int m_uid;
	std::string m_tag;
	MStageType m_type;
	MissionNode *m_choices[2];

	MissionNode();
	MissionNode(MissionNode_Template *nodetemplate, int DC);

	void Save(FILE *file);
	MissionNode(CBReader *reader);
	MissionNode *FindNode(int id);

};

#endif