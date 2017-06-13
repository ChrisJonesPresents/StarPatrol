#ifndef STAR
#define STAR

#include "Location.h"
#include "Event.h"
#include "Mission2.h"

USING_NS_CC;

enum StarType{ST_BLACK,ST_GREY,ST_YELLOWGIANT,ST_YELLOW,ST_BLUEGIANT,ST_BLUE,ST_REDGIANT,ST_RED};

class Star
{
	cocos2d::Vec2 m_position;
	cocos2d::Vec2 m_graphic;
	StarType m_type;
	std::string m_name;
	bool m_explored;
	Location **m_locations;
	unsigned short m_numloc;

	void Generate(float DC);
	LocationType TypeGen();
	bytepos VecGen(int index);
	Mission2 *m_mission;
public:
	Star(cocos2d::Vec2 p);
	Star(cocos2d::Vec2 p, StarType type);
	~Star();
	cocos2d::Vec2 getPosition();

	cocos2d::Vec2 getGraphic();

	void Explore(float DC);

	std::string getName();

	bool getExplored();

	StarType getType();

	int getNumLoc();
	Location **getLocations();

	Mission2 *getMission();
	void setMission(Mission2 *mission);

	void MissionGen(int d);

	void Save(FILE *file);
	Star();
	bool Load(CBReader *reader);
};
#endif