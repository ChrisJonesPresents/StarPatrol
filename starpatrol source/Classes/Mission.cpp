#include "Mission.h"

Mission::Mission()
{

}

Mission::Mission(std::string tag, int numstages, Mission_Stage **stages)
{
	m_tag = tag;
	m_numstages = numstages;
	m_stages = stages;
}

Mission::~Mission()
{
	delete[] m_stages;
}

int Mission::getUID()
{
	return m_uid;
}

void Mission::setUID(int uid)
{
	m_uid = uid;
}

int  Mission::getnumStages()
{
	return m_numstages;
}

Mission_Stage * Mission::getStage(int i)
{
	return m_stages[i];
}

std::string Mission::getTag()
{
	return m_tag;
}