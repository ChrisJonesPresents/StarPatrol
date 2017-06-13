#ifndef MISSION
#define MISSION

#include "cocos2d.h"
#include "Mission_Stage.h"
#include "Common.h"


USING_NS_CC;

class Mission
{
	Mission_Stage **m_stages;
	short m_numstages;
	std::string m_tag;
	short m_uid;

public:
	Mission();
	Mission(std::string tag, int numstages, Mission_Stage **stages);
	~Mission();
	int getUID();
	void setUID(int uid);
	int getnumStages();
	std::string getTag();
	Mission_Stage *getStage(int i);

};

#endif