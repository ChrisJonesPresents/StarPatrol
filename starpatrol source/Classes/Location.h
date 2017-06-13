#ifndef LOCATION
#define LOCATION

#include "Mission2.h"
#include "Colony.h"
enum LocationType{LT_WARPGATE,LT_GAS,LT_BARREN,LT_ICE,LT_HELL,LT_GARDEN,LT_ASTEROID,LT_STATION,LT_SHIP,LT_ANOMALY,LT_STAR,LT_SPACE,LT_PLANET,LT_OMNI};
LocationType ReadType(CBReader *Reader);
class bytepos
{
public:
	byte m_x;
	byte m_y;
	bytepos();
	bytepos(byte x, byte y);
};


class Location
{
	bytepos m_p;
	LocationType m_type;
	Mission2 *m_mission;
	byte m_whatsprite;
	Colony *m_colony;
public:
	Location(LocationType t, bytepos position);
	Location(CBReader *reader);
	void Save(FILE *file);
	~Location();
	bytepos getPos();
	LocationType getType();
	Mission2 *getMission();
	void setMission(Mission2 *mission);

	void GenMission(float DC);
	byte getWhatsprite();
	Colony *getColony();
	void CreateColony(std::string name);
};

#endif