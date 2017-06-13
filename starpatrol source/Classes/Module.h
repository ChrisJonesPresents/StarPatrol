#ifndef MODULE
#define MODULE

#include "cocos2d.h"
#include "CBReader.h"

enum ModuleType {MT_COMMODITY, MT_ABILITY,MT_BOOSTER};

class Module
{
protected:
	std::string m_name;
	std::string m_description;
	short m_cost;
	cocos2d::Vec2 m_graphicpos;
	ModuleType m_type;
	short m_category;
	void CommonLoad(CBReader *reader);
	void Loadtype(CBReader *reader);
	int m_uid;
public:

	Module();

	std::string getName();

	std::string getDesc();

	int getCost();

	ModuleType getType();

	cocos2d::Vec2 getGraphicpos();

	short getCategory();

	int getUID();
	void setUID(int id);
};

#endif