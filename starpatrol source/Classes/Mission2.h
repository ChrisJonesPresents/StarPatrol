#ifndef MISSION2
#define MISSION2

#include "MissionNode.h"

class Mission2 
{
protected:
	MissionNode *m_root;
	MissionNode *m_current;
	int m_dc;
public:
	Mission2(){};
	Mission2(int dc, MissionNode *root);
	~Mission2();
	void Transition(int path);
	MissionNode *getCurrentNode();
	void Reset();
	int getDC();
	void AttachNode(MissionNode *node);
	int m_uid;
	virtual void Save(FILE *file);
	Mission2(CBReader *reader);
	static Mission2 *LoadMission(CBReader *reader);
	void GenNodeUIDS();
};

class MissionThreat : public Mission2
{
	int m_timelimit, m_strength;

public:
	
	MissionThreat(int dc, MissionNode *root, int strength, int timelimit);
	~MissionThreat();
	bool TimeReduce(int value);
	int getTime();
	int getStrength();
	std::string m_prompts[3];
	
	void Save(FILE *file);
	MissionThreat(CBReader *reader);

};

#endif