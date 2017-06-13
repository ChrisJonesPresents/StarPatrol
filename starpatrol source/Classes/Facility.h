#ifndef FACILITY
#define FACILITY

#include "cocos2d.h"
#include "CBReader.h"

USING_NS_CC;
class Facility
{
	std::string m_name;
	std::string m_description;
	short m_index;
	unsigned char m_requires[8];
	unsigned char m_tokencost[4];
	char m_output[3][2];
	//output pairs are type/quantity or index
	//types are 1-8=resources
	//-1 is modules 
	//-2 is repair
	//-3 is fuel
	//-4 is ammo
	//-5 is allyship
	//-6 is officer
	Vec2 m_graphic;
	int m_upgrade;
	bool m_upgradeonly;
public:
	Facility(CBReader *cbr,int index);

	Vec2 getGraphic();

	char getOutputType(int i);
	char getOutputValue(int i);
	int getCost(int i);
	int getTokenCost(int i);
	bool getUpgradeonly();
	int getUpgradeTo();
	std::string getName();
	std::string getDescription();
	int getID();

};

#endif