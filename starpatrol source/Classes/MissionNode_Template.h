#ifndef MISSIONNODE_TEMPLATE
#define MISSIONNODE_TEMPLATE
#include <string>
#include "CBReader.h"
#include "Common.h"
class MissionNode_Template
{
public:
	int m_numchoice[2];	
	std::string m_tag;
	short m_variables[7];
	//for away battles, 3 variable pairs for which enemy to pickup and how many hp each, 1 variable for background
	//for space battles, 3 variable pairs for which enemy ships to spawn and a strength multiplier, last value is whether its impossible to escape
	//for choice, 1 value to select text message, 1 value for image, 2 for choices
	//for reward, each is type, then module or officer number or amount of exp/credits, can have up to three things
	//for gameover, 1 value for the text what happened, 1 for what image to use
	//for punish, two sets of 3 values. 1st value what type of punishment, 2nd min punish 3rd max punish
	//for explore,
	int m_DC;
	MStageType m_type;

	MissionNode_Template **m_choices[2];

	MissionNode_Template(CBReader *reader);
	~MissionNode_Template();

};

#endif