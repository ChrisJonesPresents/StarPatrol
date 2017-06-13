#include "MissionHandler.h"
#include "ReferenceLibrary.h"
#include "ModeBriefing.h"
#include "ModeReward.h"
#include "Mode_Starscape.h"
#include "Mode_Awayteam.h"
#include "Mode_PrepMission.h"
#include "Mode_Spacebattle.h"
#include "Mode_Explore.h"
#include "ModeGameOver.h"
#include "ModeShop.h"
#include "SolarView.h"
#include "ModePunish.h"

MissionHandler::MissionHandler()
{
	m_mission = NULL;
	m_layer = NULL;
}

MissionHandler::~MissionHandler()
{
	if (m_layer != NULL)
	{
		m_layer->release();
	}
}
void MissionHandler::setMission(Mission2 *mission)
{
	m_mission = mission;

}
Scene *MissionHandler::BuildBriefing(MissionNode *stage, std::string tag)
{
	ModeBriefing *briefing = ModeBriefing::create();
	briefing->Setup(stage, tag);
	Scene *scene = Scene::create();
	scene->addChild(briefing,0,100);
	return scene;

}

void MissionHandler::MissionGameOver()
{
	if (m_mission != NULL)
	{
		m_mission->Reset();
	}
}

Scene *MissionHandler::BuildSpaceBattle(MissionNode *stage)
{
	int slots = 0;
	for (int i = 0; i < 3; i++)
	{
		if (stage->m_variables[(i * 2) + 1]>0)
		{
			slots++;
		}
	}
	NPCship **ship = new NPCship *[slots];
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/spaceships.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);
	for (int i = 0; i < 3; i++)
	{
		if (stage->m_variables[(i * 2) + 1]>0)
		{
			//find ship
			while (true)
			{
				std::string str = cbr->ReadString();
				if (str.find("SHIP") != std::string::npos)
				{
					int v = cbr->ReadInt();
					if (v == stage->m_variables[(i * 2)])
					{
						break;
					}
				}
			}
			ship[i] = new NPCship(cbr);
		}
		cbr->setReadposition(0);
	}
	delete cbr;
	return 	Mode_Spacebattle::createScene(GameManager::getInstance()->getPlayer()->getShip(),ship,slots,NULL,0);
}


cocos2d::Scene *MissionHandler::ProcessStage(MissionNode *stage)
{
	Scene *scene = NULL;
	Layer *layer = NULL;

	switch (stage->m_type)
	{
	case MST_CHOICE:
		scene = BuildBriefing(stage, stage->m_tag);
		break;
	case MST_TEXT:
		scene = BuildBriefing(stage, stage->m_tag);
		break;

	case MST_REWARD:
		scene = BuildReward(stage);
		break;

	case MST_EXPLORE:
		scene = BuildExplore(stage);
		break;

	case MST_BATTLE:
		scene = BuildSpaceBattle(stage);
		break;

	case MST_AWAYTEAM:
		scene = BuildAwayteam(stage);
		break;
	case MST_COMPLETE:
		CompleteMission();
		scene=ReturnToSolar();
		break;
	case MST_PUNISH:
		scene = ModePunish::createScene(GameManager::getInstance()->getPlayer(), stage);
		break;

	case MST_GAMEOVER:
		m_layer = NULL;
		scene = ModeGameOver::createScene(stage);
		break;

	case MST_TRADE:
		scene = BuildTrade(stage);
		break;

	case MST_FAIL:
		//reset the mission
		m_mission->Reset();
		m_mission = NULL;
		if (m_layer != NULL)
		{
			m_layer->release();
		}
		m_layer = NULL;
		scene=ReturnToSolar();
		break;
	}
	
	if (m_mission != NULL)
	{
		m_layer = (SaveLayer*)scene->getChildByTag(100);
		m_layer->retain();
	}
	else
	{
		m_layer = NULL;
	}


	return scene;
}

Scene *MissionHandler::BuildAwayteam(MissionNode *stage)
{
	Scene *scene = Mode_PrepMission::createScene(stage, GameManager::getInstance()->getPlayer());
	return scene;
}

Scene *MissionHandler::BuildExplore(MissionNode *stage)
{
	Scene *scene = Mode_PrepMission::createScene(stage, GameManager::getInstance()->getPlayer());
	return scene;
}

Scene *MissionHandler::BuildTrade(MissionNode *stage)
{
	ShopDef *def = new ShopDef();
	//read first 3 values for resources available
	def->m_supplies[0][0] = 5;
	def->m_supplies[1][0] = 3;
	def->m_supplies[2][0] = 6;
	for (int i = 0; i < 3; i++)
	{
		def->m_supplies[i][0] = stage->m_variables[i];
	}
	//read next 4 for the up to two items the store can sell
	int v[2][2];
	int count = 0;
	for (int i = 3; i < 7; i = i + 2)
	{
		if (stage->m_variables[i] == 1)
		{
			v[0][count] = 2;
			v[1][count] = ReferenceLibrary::getInstance()->getInventoryModule(stage->m_variables[i+1]);
			count++;
		}
	}

	Scene *scene = ModeShop::createScene(GameManager::getInstance()->getPlayer(), def, true);
	return scene;
}

Scene *MissionHandler::BuildReward(MissionNode *stage)
{
	Scene *scene=ModeReward::createScene(GameManager::getInstance()->getPlayer(), stage);
	return scene;
}

void MissionHandler::CompleteMission()
{
	Star *star = GameManager::getInstance()->getStars()[(int)GameManager::getInstance()->getPlayer()->getPos0().y][(int)GameManager::getInstance()->getPlayer()->getPos0().x];
	//find the mission

	//set the mission to null
	if (star->getMission() == m_mission)
	{
	
		star->setMission(NULL);
	}
	else
	{
		//search system localized mission
		for (int i = 0; i < star->getNumLoc(); i++)
		{
			if (star->getLocations()[i] != NULL)
			{
				if (star->getLocations()[i]->getMission() == m_mission)
				{
					star->getLocations()[i]->setMission(NULL);
				}
			}
		}
	}


	//search system

	//find any mobile threats later
	delete m_mission;
	m_mission = NULL;
	if (m_layer != NULL)
	{
		m_layer->release();
	}
	m_layer = NULL;

}

cocos2d::Scene *MissionHandler::getScene(int result)
{
	switch (result)
	{

	case -2:
		//failure state
		//always show the game over screen if this happens
		if (m_layer != NULL)
		{
			m_layer->release();
		}
		m_layer = NULL;
		return ModeGameOver::createScene(NULL);
		break;

	case -1:

		break;

	case 0:
		m_mission->Transition(0);
		break;
	case 1:
		m_mission->Transition(1);
		break;
	}

	if (m_mission->getCurrentNode() == NULL)
	{
		if (m_layer != NULL)
		{
			m_layer->release();
		}
		m_layer = NULL;
		m_mission->Reset();
		m_mission = NULL;
		return ReturnToSolar();
	}
	return ProcessStage(m_mission->getCurrentNode());
}

cocos2d::Scene* MissionHandler::ReturnToSolar()
{
	Scene *scene = Mode_Starscape::createScene(true);

	//attach solarviewer
	Player *p = GameManager::getInstance()->getPlayer();

	//is this the star the player is currently orbiting?
		//later, zoom in on star contents when this happens

	//	SolarView *view = SolarView::create();
	
		//view->Setup(layer, GameManager::getInstance()->getStars()[(int)p->getPos0().y][(int)p->getPos0().x]);

		//this->getParent()->addChild(view);	

		//scene->addChild(view);

	return scene;
}

bool MissionHandler::getInMission()
{
	if (m_mission == NULL)
	{
		return false;
	}
	return true;

}

void MissionHandler::Save(FILE *file)
{
	if (m_mission != NULL)
	{
		char v = 42;
		fwrite((void*)&v, sizeof(char), 1, file);
		int id = m_mission->m_uid;
		fwrite((void*)&id, sizeof(int), 1, file);
		m_layer->Save(file);
	}
	else
	{
		char v = 17;
		fwrite((void*)&v, sizeof(char), 1, file);
	}
}

void MissionHandler::Load(CBReader *reader)
{
	char v = reader->ReadChar();
	Director *director = Director::sharedDirector();
	if (v == 42)
	{
		int id = reader->ReadBInt();
		//need to find the mission with this id nearby
		m_mission = GameManager::getInstance()->getMission(id);
		Scene *scene = LoadMidMission(reader);
		
		if (director->getRunningScene() == NULL)
		{
			director->runWithScene(scene);
		}
		else
		{
			director->replaceScene(scene);
		}
	}
	else
	{
		m_mission = NULL;
		Scene *scene = Mode_Starscape::createScene();
		if (director->getRunningScene() == NULL)
		{
			director->runWithScene(scene);
		}
		else
		{
			director->replaceScene(scene);
		}


	}
}

Scene *MissionHandler::LoadMidMission(CBReader *reader)
{
	char mode = reader->ReadChar();
	Scene *scene = NULL;
	switch (mode)
	{
	case 1:
		scene = Mode_Explore::createLoadScene(reader, m_mission->getCurrentNode());
		break;
	case 2:
		scene = Mode_Spacebattle::createLoadScene(reader);
		break;
	case 3:
		scene = BuildBriefing(m_mission->getCurrentNode(), m_mission->getCurrentNode()->m_tag);
		break;
	case 4:
		scene = ModeReward::createLoadScene(m_mission->getCurrentNode(), reader);
		break;
	case 5:
		scene = ModeShop::createLoadScene(reader);
		break;
	case 6:
		scene = ModePunish::createSceneLoad(GameManager::getInstance()->getPlayer(), m_mission->getCurrentNode(), reader);
		break;
	case 7:
		scene = Mode_PrepMission::createScene(m_mission->getCurrentNode(), GameManager::getInstance()->getPlayer());
	}
	if (scene != NULL)
	{
		m_layer = (SaveLayer*)scene->getChildByTag(100);
		m_layer->retain();
	}

	return scene;
}

void MissionHandler::setLayer(SaveLayer *layer)
{
	if (m_layer != NULL)
	{
		m_layer->release();
	}
	m_layer = layer;
	m_layer->retain();
}