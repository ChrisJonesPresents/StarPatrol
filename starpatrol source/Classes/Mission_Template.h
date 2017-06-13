#ifndef MISSIONTEMPLATE
#define MISSIONTEMPLATE

#include "Common.h"
#include "CBReader.h"
class Template_Stage
{
public:
	std::string m_tag;
	short m_variables[7];
	MStageType m_type;
	//for away battles, 3 variable pairs for which enemy to pickup and how many hp each, 1 variable for background
	//for space battles, 3 variable pairs for which enemy ships to spawn and a strength multiplier, last value is whether its impossible to escape
	//for choice, 1 value to select text message, a trio of variables for each of the choices
	//for reward, each is type, then module or officer number or amount of exp/credits, can have up to three things
	Template_Stage(CBReader *cbr);
};


class Mission_Template
{
public:
	std::string *m_tags;
	int m_numtags;
	int m_numstages; 
	int *m_numstagechoices;
	int m_difficulty;
	Template_Stage ***m_stages;

	Mission_Template(CBReader *cbr);
	~Mission_Template();



};


#endif