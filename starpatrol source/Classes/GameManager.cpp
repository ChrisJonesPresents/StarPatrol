#include "GameManager.h"
#include "ReferenceLibrary.h"
#include "ModeMenu.h"

USING_NS_CC;

GameManager::GameManager()
{
	for (int i = 0; i < 64; i++)
	{
		m_colonies[i] = NULL;
	}
	m_active = false;
	m_colonycount = 0;
	m_starheight = 64;
	m_starwidth = 64;
	m_stars = new Star **[m_starheight];

	for (int i = 0; i < m_starheight; i++)
	{
		m_stars[i] = new Star *[m_starwidth];
		for (int j = 0; j < m_starwidth; j++)
		{
			m_stars[i][j] = NULL;
		}
	}
	m_player = NULL;
	//Stargen();
//	Explore(3, 3);


//	m_player = new Player(3,3);
	m_timeplayed = 100;
	m_synctime = 360;
	m_threattime = 0;
	m_clockstart = 100;
	m_saved = false;
	m_sector = 0;

}

Vec2 getPos(int i, int j)
{
	int x = j * 64; int y = i * 64;
	x = x - 16; y = y - 16;
	x = x + (rand() % 32); y = y + (rand() % 32);

	return Vec2(x * 2, y * 2);
}

void GameManager::Explore(int i, int j)
{

	if (m_stars[i][j] == NULL)
	{
		//randomly displace from coordinates
		m_stars[i][j] = new Star(getPos(i,j));

	}
	if (m_stars[i][j] != NULL)
	{
		float d = 1+(m_stars[3][3]->getPosition().getDistance(m_stars[i][j]->getPosition())/100) + (m_timeplayed / 360);
		m_stars[i][j]->Explore(d);
	}
	if (i > 0)
	{
		if (m_stars[i-1][j] == NULL)
		{
			//randomly displace from coordinates
			m_stars[i-1][j] = new Star(getPos(i-1, j));
		}
	}
	if (i < m_starheight - 1)
	{
		if (m_stars[i+1][j] == NULL)
		{
			m_stars[i+1][j] = new Star(getPos(i+1, j));
		}
	}
	
	if (j > 0)
	{
		if (m_stars[i][j-1] == NULL)
		{
			m_stars[i][j-1] = new Star(getPos(i, j-1));
		}
	}
	if (j < m_starwidth - 1)
	{
		if (m_stars[i][j+1] == NULL)
		{
			m_stars[i][j+1] = new Star(getPos(i, j+1));
		}
	}
	
}

void GameManager::Stargen()
{
	for (int i = 0; i < m_starheight; i++)
	{
		for (int j = 0; j < m_starwidth; j++)
		{
			//randomly displace from coordinates
			int x = j * 64; int y = i * 64;
			x = x - 25; y = y - 25;
			x = x + (rand() % 50); y = y + (rand()%50);
			m_stars[i][j] = new Star(Vec2(x*2, y*2));
		}
	}

}

int GameManager::getDays()
{
	return (m_clockstart+m_timeplayed) % 30;
}
int GameManager::getMonths()
{
	return ((m_clockstart + m_timeplayed) / 30) % 12;
}
int GameManager::getYears()
{
	return 2115 + ((m_clockstart + m_timeplayed) / 360);
}
GameManager::~GameManager()
{

	Save();

	for (int i = 0; i < m_starheight; i++)
	{
		for (int j = 0; j < m_starwidth; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				delete m_stars[i][j];
			}
		}
		delete[] m_stars[i];
	}
	delete[] m_stars;

	if (m_player != NULL)
	{
		delete m_player;
	}

}

GameManager *GameManager::getInstance()
{
	if (m_instance == 0)
	{
		m_instance = new GameManager();
		return m_instance;
	}
	else
	{
		return m_instance;
	}

}


void GameManager::FreeInstance()
{
	if (m_instance != NULL)
	{
		delete m_instance;
	}
}

Star ***GameManager::getStars()
{
	return m_stars;
}
int GameManager::getWidth()
{
	return m_starwidth;
}
int GameManager::getHeight()
{
	return m_starheight;
}

Player *GameManager::getPlayer()
{
	return m_player;
}

void GameManager::Newgame()
{
	m_active = false;

	if (m_player != NULL)
	{
		delete m_player;
		m_player = NULL;
	}
	for (int i = 0; i < m_starheight; i++)
	{

		for (int j = 0; j < m_starwidth; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				delete m_stars[i][j];
			}
			m_stars[i][j] = NULL;
		}
	}
	//place warpgate
	m_stars[3][3] = new Star(getPos(3, 3), ST_BLACK);
	//explore adjacent stars
	Explore(3, 3);
	//place player at warpgate
//	m_player = new Player();
//	m_player->Newgame(3, 3);


}




void GameManager::ThreatRunout(Location *location)
{
	Colony *colony = location->getColony();
	MissionThreat *threat = (MissionThreat*)location->getMission();
	int defence = colony->getDefence(0) + colony->getDefenceBonus();
	int defroll = 0;
	if (defence > 0)
	{
		defroll = rand() % defence;
	}
	if (defroll >= threat->getStrength())
	{
		m_player->getLogbook()->AddEntry(true, m_timeplayed + m_clockstart, threat->m_prompts[2]);
	}
	else
	{
		std::string str = "nothing";
		if (colony->HasFacilities())
		{
			int r = rand() % 8;
			while (colony->getFacility(r) == NULL)
			{
				r = rand() % 8;
			}
			str = colony->getFacility(r)->getName();
			colony->DestroyFacility(r);
		}
		m_player->getLogbook()->AddEntry(true, m_timeplayed + m_clockstart, myreplace(threat->m_prompts[1],"DAMAGE",str));
	}
	delete location->getMission();
	location->setMission(NULL);
}

bool GameManager::IncrementTime(int time)
{
	m_timeplayed += time;
	m_synctime -= time;
	if (m_synctime < 0)
	{
		WorldSeed();
		m_synctime = 360;
	}
	for (int i = 0; i < m_colonycount; i++)
	{
		if (m_colonies[i]->getMission() != NULL)
		{
			MissionThreat *missionthreat = (MissionThreat*)m_colonies[i]->getMission();
			if (missionthreat->TimeReduce(time))
			{
				ThreatRunout(m_colonies[i]);
				return true;
			}
		}
	}

	if (m_timeplayed / 10>m_threattime)
	{

		m_threattime++;
		return ThreatTrigger();
	}
	return false;
}

bool GameManager::ThreatTrigger()
{
	int r = rand() % 30;
	if (m_colonycount == 0)
	{
		return false;
	}
	if (r == 0)
	{
		//find a colony and attach a threat mission
		r = rand() % m_colonycount;
		if (m_colonies[r]->getMission() == NULL && m_colonies[r]->getColony()->HasFacilities())
		{
			//build new mission
			MissionThreat *mission=(MissionThreat*)ReferenceLibrary::getInstance()->getQuestEngine()->AddThreat(m_timeplayed/360,m_clockstart+m_timeplayed, *m_colonies[r]->getColony()->getName()); 
			//report message
			m_player->getLogbook()->AddEntry(true, m_timeplayed + m_clockstart, mission->m_prompts[0]);
			m_colonies[r]->setMission(mission);

			return true;
		}
	}
	return false;
}

void GameManager::WorldSeed()
{
	for (int i = 0; i < m_starwidth; i++)
	{
		for (int j = 0; j < m_starheight; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				float d = 1 + (m_stars[3][3]->getPosition().getDistance(m_stars[i][j]->getPosition()) / 6) + (m_timeplayed / 360);
				m_stars[i][j]->MissionGen(d);
			}
		}
	}
	for (int i = 0; i < m_colonycount; i++)
	{
		m_colonies[i]->getColony()->Restock();
	}

}

MissionHandler *GameManager::getHandler()
{
	return &m_handler;
}

Colony *GameManager::findColony(int x, int y)
{
	Star *star = m_stars[x][y];
	if (star != NULL)
	{
		for (int i = 0; i < star->getNumLoc(); i++)
		{
			if (star->getLocations()[i] != NULL)
			{
				if (star->getLocations()[i]->getColony() != NULL)
				{
					return star->getLocations()[i]->getColony();
				}
			}
		}
	}
	return NULL;

}

void GameManager::ColonyConnector(int x, int y, Colony *colony)
{
	if (x > 0)
	{
		Colony *c = findColony(x - 1, y);
		if (c != NULL)
		{
			colony->AddColony(c);
			c->AddColony(colony);
		}
	}
	if (y > 0)
	{
		Colony *c = findColony(x, y-1);
		if (c != NULL)
		{
			colony->AddColony(c);
			c->AddColony(colony);
		}
	}
	if (x < m_starwidth-1)
	{
		Colony *c = findColony(x +1, y);
		if (c != NULL)
		{
			colony->AddColony(c);
			c->AddColony(colony);
		}
	}
	if (y < m_starheight-1)
	{
		Colony *c = findColony(x , y+1);
		if (c != NULL)
		{
			colony->AddColony(c);
			c->AddColony(colony);
		}
	}



}

void GameManager::AddColony(Location *star)
{
	m_colonies[m_colonycount] = star;
	m_colonycount++;

}

bool GameManager::getActive()
{
	return m_active;
}

void GameManager::setActive()
{
	m_active = true;
}

void GameManager::Resume()
{
	m_saved = false;
}
void GameManager::Save()
{
	m_saved = true;

	BackupSave();
	//get first path
	char buffer[3];
	itoa(m_sector, &buffer[0], 10);
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "save"+buffer+".sav";
	CCLOG("path = %s", path.c_str());

	//create a file point 
	FILE* file = fopen(path.c_str(), "w");
	if (file)
	{
		//start writing
		SaveData(file);
		InsertSafety(file);
		SaveStars(file);
		InsertSafety(file);
		if (m_player != NULL)
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
			m_player->Save(file);
			InsertSafety(file);
			m_handler.Save(file);
		}
		else
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}


		//save tail sequence
		char buffer[4];
		buffer[0] = 1;
		buffer[1] = 2;
		buffer[2] = 3;
		buffer[3] = 4;
		fwrite((void*)&buffer[0], sizeof(char), 4, file);
		fclose(file);

	}


}

void  GameManager::SaveData(FILE*file)
{
	fwrite((void*)&m_clockstart, sizeof(int), 1, file);
	fwrite((void*)&m_timeplayed, sizeof(int), 1, file);
	fwrite((void*)&m_synctime, sizeof(int), 1, file);
	fwrite((void*)&m_threattime, sizeof(int), 1, file);
	ReferenceLibrary::getInstance()->getQuestEngine()->Save(file);
}

void GameManager::LoadData(CBReader *cbr)
{
	m_clockstart = cbr->ReadBInt();
	m_timeplayed = cbr->ReadBInt();
	m_synctime = cbr->ReadBInt();
	m_threattime = cbr->ReadBInt();
	int v = cbr->ReadBInt();
	ReferenceLibrary::getInstance()->getQuestEngine()->setUID(v);
}
void  GameManager::SaveStars(FILE *file)
{
	for (int i = 0; i < m_starwidth; i++)
	{
		for (int j = 0; j < m_starheight; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				char value = 42;
				fwrite((void*)&value, sizeof(char), 1, file);
				m_stars[i][j]->Save(file);
			}
			else
			{
				char value = 17;
				fwrite((void*)&value, sizeof(char), 1, file);
			}
		}

	}
}

bool GameManager::LoadStars(CBReader*cbr)
{
	int c = 0;
	int stars = 0;
	for (int i = 0; i < m_starwidth; i++)
	{
		for (int j = 0; j < m_starheight; j++)
		{
			c++;
			if (m_stars[i][j] != NULL)
			{
				delete m_stars[i][j];
			}
			char v = cbr->ReadChar();
			if (v == 42)
			{
				m_stars[i][j] = new Star();
				if (m_stars[i][j]->Load(cbr)==false)
				{
					return false;
				}
				stars++;
				
				Colony *colony = findColony(i, j);
				if (colony != NULL)
				{
					ColonyConnector(i, j, colony);
				}
			}
			else
			{
				m_stars[i][j] = NULL;
			}

		}
	}
	return true;
}

bool GameManager::BackupSave()
{
	//open existing save if any
	char buffer[3];
	itoa(m_sector, &buffer[0], 10);
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "save" + buffer + ".sav";
	CCLOG("path = %s", path.c_str());

	//create a file point 
	FILE* file = fopen(path.c_str(), "r");

	if (file) {
		char* buf;  //get string  
		int len;    //get string length 

		fseek(file, 0, SEEK_END);   //move to end 
		len = ftell(file);          //get length 
		//check end of file for tail
		fseek(file, -4, SEEK_END);
		char buffer[4];
		fread(buffer, sizeof(char), 4, file);
		for (int i = 0; i < 4; i++)
		{
			if (buffer[i] != i + 1)
			{
				fclose(file);
				return false;
			}
		}
		rewind(file);               //return to begin 

		CCLOG("count the file content len = %d", len);
		//distribute buff space
		buf = new char[len + 1];
		if (!buf) {
			CCLOG("malloc space is not enough.");

		}

		//read file 

		int rLen = fread(buf, sizeof(char), len, file);
		buf[rLen] = '\0';
		CCLOG("has read Length = %d", rLen);
		CCLOG("has read content = %s", buf);
		fclose(file);

		//now write the file back into the backup
		itoa(m_sector, &buffer[0], 10);
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "savebackup" + buffer + ".sav";
		file = fopen(path.c_str(), "w");
		fwrite(buf, sizeof(char), len, file);
		fclose(file);
	}
	else
		CCLOG("open file error.");
	
	return true;
}

bool GameManager::Load()
{
	//open existing save if any
	char buffer[3];
	itoa(m_sector, &buffer[0], 10);
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "save" + buffer + ".sav";
	CCLOG("path = %s", path.c_str());

	//create a file point 
	FILE* file = fopen(path.c_str(), "r");
	if (file)
	{
		char* buf;  //get string  
		int len;    //get string length 

		fseek(file, 0, SEEK_END);   //move to end 
		len = ftell(file);          //get length 
		//check end of file for tail
		fseek(file, -4, SEEK_END);
		char buffer[4];
		fread(buffer, sizeof(char), 4, file);
		for (int i = 0; i < 4; i++)
		{
			if (buffer[i] != i + 1)
			{
				fclose(file);
				return LoadBackup();			
			}
		}
		rewind(file);               //return to begin 

		CCLOG("count the file content len = %d", len);
		//distribute buff space
		buf = new char[len + 1];
		if (!buf) {
			CCLOG("malloc space is not enough.");

		}

		//read file 

		int rLen = fread(buf, sizeof(char), len, file);
		buf[rLen] = '\0';
		CCLOG("has read Length = %d", rLen);
		CCLOG("has read content = %s", buf);
		fclose(file);

		CBReader *cbr = new CBReader(buf, rLen);
		LoadData(cbr);
		if (CheckSafety(cbr) == false)
		{
			return false;
		}
		if (LoadStars(cbr) == false)
		{
			return false;
		}
		if (CheckSafety(cbr) == false)
		{
			return false;
		}
		if (m_player != NULL)
		{
			delete m_player;
		}
		char v = cbr->ReadChar();
		if (v == 42)
		{
			m_player = new Player(cbr);
			if (CheckSafety(cbr) == false)
			{
				return false;
			}
			m_handler.Load(cbr);
		}
		else
		{
			RunInitScreen();
			m_player = NULL;
		}

		m_active = true;
		delete cbr;
		return true;
	}
	else
	{
		return LoadBackup();
	}
}


bool GameManager::LoadBackup()
{
	//open existing save if any
	char buffer[3];
	itoa(m_sector, &buffer[0], 10);
	std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "savebackup" + buffer + ".sav";
	CCLOG("path = %s", path.c_str());

	//create a file point 
	FILE* file = fopen(path.c_str(), "r");
	if (file)
	{
		char* buf;  //get string  
		int len;    //get string length 

		fseek(file, 0, SEEK_END);   //move to end 
		len = ftell(file);          //get length 
		//check end of file for tail
		fseek(file, -4, SEEK_END);
		rewind(file);               //return to begin 

		CCLOG("count the file content len = %d", len);
		//distribute buff space
		buf = new char[len + 1];
		if (!buf) {
			CCLOG("malloc space is not enough.");

		}

		//read file 

		int rLen = fread(buf, sizeof(char), len, file);
		buf[rLen] = '\0';
		CCLOG("has read Length = %d", rLen);
		CCLOG("has read content = %s", buf);
		fclose(file);

		CBReader *cbr = new CBReader(buf, rLen);
		LoadData(cbr);
		if (CheckSafety(cbr) == false)
		{
			return false;
		}
		if (LoadStars(cbr) == false)
		{
			return false;
		}
		if (CheckSafety(cbr) == false)
		{
			return false;
		}
		if (m_player != NULL)
		{
			delete m_player;
		}
		char v = cbr->ReadChar();
		if (v == 42)
		{
			m_player = new Player(cbr);
			if (CheckSafety(cbr) == false)
			{
				return false;
			}
			m_handler.Load(cbr);
		}
		else
		{
			//RunInitScreen();
			m_player = NULL;
		}
		delete cbr;
		return true;
	}
	else
	{
		return false;
	}


}

bool GameManager::getSaved()
{
	return m_saved;
}
int GameManager::getSector()
{
	return m_sector;

}

Mission2 *GameManager::getMission(int id)
{
	Star *star = m_stars[(int)m_player->getPos0().y][(int)m_player->getPos0().x];

	if (star->getMission() != NULL)
	{
		if (star->getMission()->m_uid == id)
		{
			return star->getMission();
		}
	}
	for (int i = 0; i < star->getNumLoc(); i++)
	{
		if (star->getLocations()[i] != NULL)
		{
			if (star->getLocations()[i]->getMission() != NULL)
			{
				if (star->getLocations()[i]->getMission()->m_uid == id)
				{
					return star->getLocations()[i]->getMission();
				}
			}
		}
	}

	return NULL;
}

void GameManager::InsertSafety(FILE *file)
{
	unsigned char buffer[4];
	for (int i = 0; i < 4; i++)
	{
		buffer[i] = i + 32;

	}
	fwrite((void*)&buffer[0], sizeof(char), 4, file);
}
bool GameManager::CheckSafety(CBReader *reader)
{
	unsigned char buffercheck[4];
	for (int i = 0; i < 4; i++)
	{
		buffercheck[i]= reader->ReadChar();

	}
	for (int i = 0; i < 4; i++)
	{
		if (buffercheck[i] != i + 32)
		{
			return false;
		}
	}
	return true;
}

void GameManager::RunInitScreen()
{
	m_player = new Player();
	Scene *scene = ModeMenu::createScene();
	if (Director::sharedDirector()->getRunningScene() != NULL)
	{
		Director::sharedDirector()->replaceScene(scene);
	}
	else
	{
		Director::sharedDirector()->runWithScene(scene);
	}
}

void GameManager::GameOver()
{
	if (m_colonycount > 0)
	{
		for (int i = 0; i < m_colonycount; i++)
		{
			if (m_colonies[i]->getMission() != NULL)
			{
				ThreatRunout(m_colonies[i]);
			}
		}
	}
	delete m_player;
	m_player = NULL;
	WorldSeed();


	Save();

}

void GameManager::NewCaptain(std::string names[2])
{

	m_player = new Player();
	bool b = false;
	if (m_colonycount == 0)
	{
		b = true;
	}
	m_player->NewCaptain(3, 3, names[0],names[1],b,m_timeplayed+m_clockstart);

}

GameManager *GameManager::m_instance = 0;