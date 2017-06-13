#ifndef MISSION_STAGE
#define MISSION_STAGE
#include <ui/UIButton.h>
#include "cocos2d.h"

#include "Common.h"

class Mission_Stage
{
public:
	std::string m_tag;
	short m_variables[7];
	MStageType m_type;

};

#endif