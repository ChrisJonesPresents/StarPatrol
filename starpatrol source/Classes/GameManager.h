#ifndef GAMEMANAGER
#define GAMEMANAGER

#include "Common.h"
#include "Event.h"
#include "Location.h"
#include "Star.h"
#include "Star.h"
#include "Player.h"
#include "MissionHandler.h"
#include "cocos2d.h"
#include "SaveLayer.h"

class GameManager
{
	static GameManager *m_instance;

	GameManager();

	Player *m_player;

	Star ***m_stars;
	int m_starwidth, m_starheight;

	void Stargen();
	int m_clockstart;
	int m_timeplayed;
	int m_synctime;
	int m_threattime;

	MissionHandler m_handler;

	bool ThreatTrigger();
	void ThreatRunout(Location *location);
	Location *m_colonies[64];
	int m_colonycount,m_sector;
	bool m_active,m_saved;
	bool BackupSave();

	void SaveData(FILE *file);
	void LoadData(CBReader *cbr);
	void SaveStars(FILE *file);
	bool LoadStars(CBReader*cbr);
	bool LoadBackup();
	void InsertSafety(FILE *file);
	bool CheckSafety(CBReader *reader);

public:
	
	~GameManager();

	static GameManager *getInstance();
	static void FreeInstance();
	Star ***getStars();
	int getWidth();
	int getHeight();
	Player *getPlayer();

	void Explore(int i, int j);

	void Newgame();

	bool IncrementTime(int time);
	void WorldSeed();
	int getDays();
	int getMonths();
	int getYears();

	Colony *findColony(int x, int y);

	MissionHandler *getHandler();
	
	void ColonyConnector(int x, int y, Colony *colony);

	void AddColony(Location *star);
	bool getActive();
	void setActive();
	void Resume();
	void Save();
	bool Load();
	bool getSaved();
	int getSector();
	Mission2 *getMission(int id);
	void RunInitScreen();
	void GameOver();
	void NewCaptain(std::string names[2]);

};


#endif