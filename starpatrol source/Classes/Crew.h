#ifndef CREW
#define CREW
#include "Common.h"
#include "cocos2d.h"
class Crew
{
public:
	FieldType m_type;
	boolean m_individual, m_hard, m_heavy,m_frontrow;
	short m_attack, m_defence, m_UID, m_requires,m_cost;
	std::string m_name;
	std::string m_description;
	Vec2 m_graphic, m_size;
};



#endif