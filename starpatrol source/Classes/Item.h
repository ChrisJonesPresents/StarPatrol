#ifndef ITEM
#define ITEM

#include "cocos2d.h"

enum ITEMTYPE{ITYPE_LICENSE};

class Item
{
public:
	ITEMTYPE m_type;
	short m_cost;
	std::string m_description;
	std::string m_name;
	int m_uid;
};

#endif // !ITEM
