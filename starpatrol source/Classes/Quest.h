#ifndef QUEST
#define QUEST

#include <string>
#include "Mission2.h"

enum QUESTSTATE {QS_INACTIVE,QS_ACTIVE,QS_COMPLETE};

class Quest
{
public:
	Mission2 *m_mission;
	
	int m_category;
	int m_timelimit;
	QUESTSTATE m_state;
	short m_x0, m_y0, m_x1,m_y1;
	//rewards go here
	int m_rewards[3][2];
	std::string m_briefing;



};

#endif