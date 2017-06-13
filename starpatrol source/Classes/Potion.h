#ifndef POTION
#define POTION

#include "Common.h"
#include "Ability_Away.h"

class Potion
{
public:
	int m_uid;
	short m_category;
	Ability_Away *m_effect;
	std::string m_name;
	std::string m_description;
	unsigned short m_cost;
	Vec2 m_graphicpos;
	Potion(CBReader *cbr, Ability_Away **abilities, int uid);
};


#endif