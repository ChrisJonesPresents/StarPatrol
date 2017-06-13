#ifndef MISSIONHANDLER
#define MISSIONHANDLER

#include "Mission2.h"
#include "cocos2d.h"
#include "SaveLayer.h"

class MissionHandler
{
	Mission2 *m_mission;
	int m_completioncount;

	cocos2d::Scene*ProcessStage(MissionNode *stage);
	cocos2d::Scene*ReturnToSolar();
	void CompleteMission();
	
	Scene *BuildBriefing(MissionNode *stage,std::string tag);
	Scene *BuildSpaceBattle(MissionNode *stage);
	Scene *BuildReward(MissionNode *stage);
	Scene *BuildAwayteam(MissionNode *stage);
	Scene *BuildExplore(MissionNode *stage);
	Scene *BuildTrade(MissionNode *stage);
	SaveLayer *m_layer;

	Scene *LoadMidMission(CBReader *reader);
public:

	MissionHandler();
	~MissionHandler();
	void setMission(Mission2 *mission);

	cocos2d::Scene *getScene(int result);

	bool getInMission();

	void Save(FILE *file);
	void Load(CBReader *reader);
	void setLayer(SaveLayer *layer);
	void MissionGameOver();
};

#endif