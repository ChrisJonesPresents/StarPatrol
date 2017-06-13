#include "Mode_Explore.h"
#include "ReferenceLibrary.h"
#include "GameManager.h"

cocos2d::Scene* Mode_Explore::createScene(Officer *officers[3], MissionNode *stage, Potion *potions[3])
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Mode_Explore::create();

	layer->Setup(officers,stage,potions);

	// add layer as a child to scene
	scene->addChild(layer,0,100);

	if (layer != NULL)
	{
		GameManager::getInstance()->getHandler()->setLayer(layer);
	}
	// return the scene
	return scene;

}
cocos2d::Scene* Mode_Explore::createDebugScene()
{
	Officer *officers[3];
	officers[0] = new Officer(ReferenceLibrary::getInstance()->getOfficer(1));
	officers[1] = NULL; officers[2] = NULL;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Mode_Explore::create();

	MissionNode *stage = new MissionNode();
	stage->m_variables[3] = 2;
	stage->m_variables[4] = 1;

	layer->Setup(officers, stage,NULL);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

bool Mode_Explore::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_handindex = 0;
	m_mapheight = 0;
	m_mapwidth = 0;
	m_tiles = NULL;
	m_hand = NULL;

	return true;
}

void Mode_Explore::BuildSprites()
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_root = Node::create();
	m_root->setPosition(origin.x + 600 - (m_position.x * 64), origin.y + 128 - (m_position.y * 64));
	m_root->setScale(2);
	this->addChild(m_root);
	m_root->setZOrder(0);


	//lay tiles out
	m_tilesprites = new Sprite **[m_mapwidth];
	for (int i = 0; i < m_mapwidth; i++)
	{
		m_tilesprites[i] = new Sprite *[m_mapheight];
		for (int j = 0; j < m_mapheight; j++)
		{
			if (m_tiles[i][j] != NULL)
			{
				if (m_tiles[i][j]->m_visited == true)
				{
					m_tilesprites[i][j] = Sprite::create("art/explorationsprites.png", Rect(0, 0, 32, 32));
				}
				else
				{
					m_tilesprites[i][j] = Sprite::create("art/explorationsprites.png", Rect(0, 32, 32, 32));
				}
				m_tilesprites[i][j]->setPosition(Vec2(0 + (i * 32), 0 + (j * 32)));
				m_tilesprites[i][j]->setAnchorPoint(Vec2(0.5F, 0.5F));
				m_root->addChild(m_tilesprites[i][j]);
				for (int k = 0; k < 4; k++)
				{
					if (m_tiles[i][j]->m_connections[k] == true)
					{
						Sprite *arrow = Sprite::create("art/explorationsprites.png", Rect(32, 0, 32, 32));
						arrow->setRotation(90 * k);
						arrow->setPosition(16, 16);
						arrow->setAnchorPoint(Vec2(0.5F, 0.5F));
						m_tilesprites[i][j]->addChild(arrow);
					}
				}
			}
		}
	}
	//place player icon
	m_playersprite = Sprite::create("art/explorationsprites.png", Rect(64, 0, 32, 32));
	m_playersprite->setPosition(m_position.x * 32, m_position.y * 32);
	m_root->addChild(m_playersprite);


}

void Mode_Explore::Setup(Officer* officers[3], MissionNode *stage,Potion *potions[3])
{
	m_node = stage;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	for (int i = 0; i < 4; i++)
	{
		m_resources[i] = 0;
	}

	while (Mapgen(stage) == false)
	{
		

	}
	GoalGen(stage);
	EnemyPlacement(stage);
	RewardGen(stage);

	m_tiles[(int)m_position.x][(int)m_position.y]->m_visited = true;

	BuildSprites();
	for (int i = 0; i < 3; i++)
	{
		m_frames[i] = OfficerFrame::create();
		m_frames[i]->Setup(officers[i]);
		this->addChild(m_frames[i]);
		m_frames[i]->setZOrder(5);
		m_frames[i]->setPosition(origin.x + 64 + (128 * i), origin.y + 128);
		m_officers[i] = m_frames[i]->m_officer;
		m_potions[i] = potions[i];
	}
	//set background
	std::string str = "art/background";
	char buffer[3];
	itoa(stage->m_variables[2], &buffer[0], 10);
	str.append(buffer); str.append(".png");
	m_background = Sprite::create(str);
	this->addChild(m_background);
	m_background->setScale(2);
	m_background->setPosition(origin.x + 400, origin.y + 448);
	m_background->setZOrder(5);

	m_grabbable = Sprite::create("art/rewardsprites.png",Rect(0,0,32,32));
	m_grabbable->setVisible(false);
	m_grabbable->setPosition(m_background->getPosition());
	m_grabbable->setZOrder(6);
	this->addChild(m_grabbable);

	this->setTouchEnabled(true);

	for (int i = 0; i < 3; i++)
	{
		m_potionframes[i] = Sprite::create("art/crewpanel.png", Rect(32, 32, 32, 32));
		m_potionframes[i]->setPosition(origin.x + 412, origin.y + 32 + (64 * i));
		this->addChild(m_potionframes[i]);
		m_potionframes[i]->setScale(2);
		if (m_potions[i] != NULL)
		{
			m_potionsprites[i] = Sprite::create("art/potions.png", Rect(m_potions[i]->m_graphicpos.x * 32, m_potions[i]->m_graphicpos.y * 32, 32, 32));
			m_potionsprites[i]->setPosition(m_potionframes[i]->getPosition());
			m_potionsprites[i]->setScale(2);
			m_potionsprites[i]->setZOrder(5);
			this->addChild(m_potionsprites[i]);
		}
		else
		{
			m_potionsprites[i] = NULL;
		}
	}

}

Mode_Explore::~Mode_Explore()
{
	for (int i = 0; i < m_mapwidth; i++)
	{
		for (int j = 0; j < m_mapheight; j++)
		{
			if (m_tiles[i][j] != NULL)
			{
				delete m_tiles[i][j];
			}
		}
		delete[] m_tiles[i];
		delete[]m_tilesprites[i];
	}
	delete[]m_tiles;
	delete[]m_tilesprites;



}

void Mode_Explore::RewardGen(MissionNode *node)
{
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/exploration/rewards.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);

	ReadForward(cbr, node->m_variables[6]);

	int numentries = cbr->ReadInt();
	for (int i = 0; i < numentries; i++)
	{
		//read min, read max
		int min = cbr->ReadInt();
		int max = cbr->ReadInt();
		int type = cbr->ReadInt();
		int count = rand() % max - min; count = count + min;
		for (int j = 0; j < count; j++)
		{
			//find a valid tile, not a goal, not a trap
			int x = rand() % m_mapwidth;
			int y = rand() % m_mapheight;
			while (Goaltest(x, y) == false)
			{
				x = rand() % m_mapwidth;
				y = rand() % m_mapheight;
			}
			//place it
			m_tiles[x][y]->m_traptreasure = type;
		}
	}
	delete cbr;
}

int Mode_Explore::GenMove(int *x, int *y, int r)
{
	int v = 0;
	int xo = *x; 
	int yo = *y;
	switch (r)
	{
	case 0:
		*y=*y+1;
		break;
	case 1:
		*x=*x+1;
		break;
	case 2:
		*y=*y-1;
		break;
	case 3:
		*x=*x-1;
		break;
	}
	bool b = false;
	if (*x < 0){ *x = 0; b = true; }
	if (*x >= m_mapwidth){ *x = m_mapwidth - 1; b = true; }
	if (*y < 0){ *y = 0; b = true; }
	if (*y >= m_mapheight){ *y = m_mapheight - 1; b = true; }
	if (b == false)
	{
		if (m_tiles[*x][*y] == NULL)
		{
			m_tiles[*x][*y] = new ExploreTile();
			v++;
		}
		//add connections
		m_tiles[xo][yo]->m_connections[r] = true;
		int rplus = r + 2;
		if (rplus >= 4)
		{
			rplus = rplus - 4;
		}
		m_tiles[*x][*y]->m_connections[rplus] = true;
	}
	return v;
}

bool Mode_Explore::GenStartPoint(int x, int y)
{
	if (x > 0)
	{
		if (m_tiles[x - 1][y] != NULL)
		{
			return true;
		}
	}
	
	if (y > 0)
	{
		if (m_tiles[x][y - 1] != NULL)
		{
			return true;
		}
	}
	if (x  <m_mapwidth-1)
	{
		if (m_tiles[x + 1][y] != NULL)
		{
			return true;
		}
	}

	if (y < m_mapheight-1)
	{
		if (m_tiles[x][y+1] != NULL)
		{
			return true;
		}
	}
	return false;
}

bool Mode_Explore::IslandCheck()
{
	int x = rand() % m_mapwidth;
	int y = rand() % m_mapheight;
	int empty = 0;
	while (m_tiles[x][y] == NULL)
	{
		x = rand() % m_mapwidth;
		y = rand() % m_mapheight;
	}
	//flood outwards
	bool **grid = new bool *[m_mapwidth];
	for (int i = 0; i < m_mapwidth; i++)
	{
		grid[i] = new bool[m_mapheight];
		for (int j = 0; j < m_mapheight; j++)
		{
			grid[i][j] = false;
		}
	}

	RecursiveIslander(x, y, grid);

	bool b = false;
	for (int i = 0; i < m_mapwidth; i++)
	{
		for (int j = 0; j < m_mapheight; j++)
		{
			if (grid[i][j] == false && m_tiles[i][j] != NULL)
			{
				if (GenConnection(i, j, grid))
				{
					b = true;
				}
			}
			if (b == true)
			{
				break;
			}
		}
		if (b == true)
		{
			break;
		}
	}

	for (int i = 0; i < m_mapwidth; i++)
	{
		for (int j = 0; j < m_mapheight; j++)
		{
			if (grid[i][j] == false && m_tiles[i][j] != NULL)
			{
				empty++;
			}
		}
	}

	for (int i = 0; i < m_mapwidth; i++)
	{
		delete[] grid[i];
	}
	delete[]grid;
	if (empty>0)
	{
		return false;
	}
	return true;
}

bool Mode_Explore::GenConnection(int x, int y,bool **grid)
{
	//check for adjacent flooded tiles
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			if (y>0)
			{
				if (m_tiles[x][y - 1] != NULL && grid[x][y - 1] == true)
				{
					m_tiles[x][y]->m_connections[i] = true;
					return true;
				}
			}
			break;

		case 1:
			if (x>0)
			{
				if (m_tiles[x-1][y] != NULL && grid[x-1][y] == true)
				{
					m_tiles[x][y]->m_connections[i] = true;
					return true;
				}
			}
			break;
		case 2:
			if (y<m_mapheight-1)
			{
				if (m_tiles[x][y + 1] != NULL && grid[x][y + 1] == true)
				{
					m_tiles[x][y]->m_connections[i] = true;
					return true;
				}
			}
			break;

		case 3:
			if (x<m_mapwidth-1)
			{
				if (m_tiles[x + 1][y] != NULL && grid[x + 1][y] == true)
				{
					m_tiles[x][y]->m_connections[i] = true;
					return true;
				}
			}
			break;
		}
	}
	return false;
}

void Mode_Explore::Enemyspawn(int x, int y, int variables[6],CBReader *cbr)
{
	for (int i = 0; i < 6; i++)
	{
		if (variables[i]>-1)
		{
			m_tiles[x][y]->m_enemies[i] = new Enemy_Away(variables[i], cbr);
		}
	}
}

void Mode_Explore::EnemyPlacement(MissionNode *node)
{
	//handle first entry
	//read first entry
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/exploration/encounters.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);
	ReadForward(cbr, node->m_variables[3]);
	int q = cbr->ReadInt();
	int v[6];
	for (int j = 0; j < 6; j++)
	{
		v[j] = cbr->ReadInt();
	}


	fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/exploration/enemies.txt");
	unsigned char *buffera = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffera = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *enemyreader = new CBReader(buffera, length);
	for (int i = 0; i < q; i++)
	{
		if (node->m_variables[0] != 2 && node->m_variables[0] != -2)
		{
			for (int k = 0; k < m_mapwidth; k++)
			{
				for (int l = 0; l < m_mapheight; l++)
				{
					if (m_tiles[k][l] != NULL)
					{
						if (m_tiles[k][l]->m_goal == true)
						{
							Enemyspawn(k, l, v, enemyreader);
						}
					}
				}
			}
		}
		else
		{
			//place anywhere that isnt a goal
			int x = rand() % m_mapwidth;
			int y = rand() % m_mapheight;
			while (EnemyPlacementCheck(x, y, false))
			{
				x = rand() % m_mapwidth;
				y = rand() % m_mapheight;
			}
			Enemyspawn(x, y, v, enemyreader);
		}
	}

	for (int i = 1; i < 3; i++)
	{
		cbr->setReadposition(0);
		if (node->m_variables[3 + i] != 0)
		{
			ReadForward(cbr, node->m_variables[3 + i]);
			q = cbr->ReadInt();
			int v[6];
			for (int j = 0; j < 6; j++)
			{
				v[j] = cbr->ReadInt();
			}
			for (int j = 0; j < q; j++)
			{
				//place anywhere that isn't a goal, trap or reward
				//place anywhere that isnt a goal
				int x = rand() % m_mapwidth;
				int y = rand() % m_mapheight;
				while (EnemyPlacementCheck(x, y, false)==false)
				{
					x = rand() % m_mapwidth;
					y = rand() % m_mapheight;
				}
				Enemyspawn(x, y, v, enemyreader);
			}
		}
	}
	delete enemyreader;
	delete cbr;
}

bool Mode_Explore::EnemyPlacementCheck(int x, int y, bool cangoal)
{
	if (x == m_position.x && y == m_position.y)
	{
		return false;
	}
	if (m_tiles[x][y] != NULL)
	{
		if (cangoal == false && m_tiles[x][y]->m_goal == true)
		{
			return false;
		}
		if (m_tiles[x][y]->m_traptreasure != 0 || m_tiles[x][y]->m_enemies[0] != NULL)
		{
			return false;
		}
		return true;
	}
	return false;
}

void Mode_Explore::RecursiveIslander(int x, int y, bool **grid)
{
	if (grid[x][y] == false)
	{
		grid[x][y] = true;
		//find all nearby locations
		for (int i = 0; i < 4; i++)
		{
			if (m_tiles[x][y]->m_connections[i] == true)
			{
				switch (i)
				{
				case 0:
					if (m_tiles[x][y + 1] != NULL)
					{
						RecursiveIslander(x, y + 1, grid);
					}
					else
					{
						m_tiles[x][y]->m_connections[i] = false;
					}

					break;

				case 1:
					if (m_tiles[x+1][y] != NULL)
					{
						RecursiveIslander(x+1, y, grid);
					}
					else
					{
						m_tiles[x][y]->m_connections[i] = false;
					}
					break;

				case 2:
					if (m_tiles[x][y - 1] != NULL)
					{
						RecursiveIslander(x, y - 1, grid);
					}
					else
					{
						m_tiles[x][y]->m_connections[i] = false;
					}

					break;

				case 3:

					if (m_tiles[x-1][y ] != NULL)
					{
						RecursiveIslander(x-1, y, grid);
					}
					else
					{
						m_tiles[x][y]->m_connections[i] = false;
					}
					break;

				}


			}
		}
	}
	
}

bool Mode_Explore::Mapgen(MissionNode *node)
{
	//open file
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/exploration/maps.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);

	ReadForward(cbr, node->m_variables[1]);

	//read size
	m_mapwidth = cbr->ReadInt(); m_mapheight = cbr->ReadInt();
	//read extent
	int extent = cbr->ReadInt();

	if (m_tiles != NULL)
	{
		for (int i = 0; i < m_mapwidth; i++)
		{
			for (int j = 0; j < m_mapheight; j++)
			{
				if (m_tiles[i][j] != NULL)
				{
					delete m_tiles[i][j];
				}
			}
			delete []m_tiles[i];
		}
		delete [] m_tiles;
	}
	m_tiles = new ExploreTile **[m_mapwidth];
	for (int j = 0; j<m_mapwidth; j++)
	{
		m_tiles[j] = new ExploreTile *[m_mapheight];
		for (int i = 0; i<m_mapheight; i++)
		{
			m_tiles[j][i] = NULL;
		}
	}
	int x=rand()%m_mapwidth, y=rand()%m_mapheight;

	//build chambers
	for (int i = 0; i<extent; i++)
	{

		if (m_tiles[x][y] == NULL)
		{
			m_tiles[x][y] = new ExploreTile();
		}
		int buildto = 8;
		while (buildto > 0)
		{
			int r = rand() % 4;
			buildto=buildto-GenMove(&x, &y, r);

		}
		x = rand() % m_mapwidth; y = rand() % m_mapheight;
		while (GenStartPoint(x, y) == false)
		{
			x = rand() % m_mapwidth; y = rand() % m_mapheight;
		}
	}

	int v = 0;
	while (IslandCheck() == false)
	{
		v++;
		if (v > 5)
		{
			return false;
		}
	}
	//read trap quantity, trap strength
	int traps = cbr->ReadInt();
	int strength = cbr->ReadInt();
	PlaceTraps(traps, strength);

	delete cbr;
	return true;
}

void Mode_Explore::PlaceTraps(int count, int strength)
{
	for (int i = 0; i < count; i++)
	{
		int x = rand() % m_mapwidth;
		int y = rand() % m_mapheight;
		while (m_tiles[x][y] == NULL)
		{
			x = rand() % m_mapwidth;
			y = rand() % m_mapheight;
		}
		m_tiles[x][y]->m_traptreasure = strength*-1;
	}
}

void Mode_Explore::GoalGen(MissionNode *node)
{
	switch (node->m_variables[0])
	{
	case 0:
		//kill 1 unit
		Goaldrop(1);
		break;

	case -1:
		//kill all in 5 locations
		Goaldrop(5);
		break;

	case 1:
		//kill all in 3 locations
		Goaldrop(3);
		break;

	case 2:
		//secure 3 locations
		Goaldrop(3);
		break;

	case -2:
		//secure 5 locations
		Goaldrop(5);
		break;
	}
}

void Mode_Explore::Goaldrop(int count)
{
	for (int i = 0; i < count; i++)
	{
		int x = rand() % m_mapwidth; 
		int y = rand() % m_mapheight;
		while (Goaltest(x, y) == false)
		{
			x = rand() % m_mapwidth;
			y = rand() % m_mapheight;
		}
		m_tiles[x][y]->m_goal = true;
	}
	//place player position
	int x = rand() % m_mapwidth;
	int y = rand() % m_mapheight;
	while (Goaltest(x, y) == false)
	{
		x = rand() % m_mapwidth;
		y = rand() % m_mapheight;
	}
	m_position = Vec2(x, y);

}

bool Mode_Explore::Goaltest(int x, int y)
{
	if (m_tiles[x][y] == NULL)
	{
		return false;
	}
	if (m_tiles[x][y]->m_goal == true)
	{
		return false;
	}
	if (m_tiles[x][y]->m_traptreasure != 0)
	{
		return false;
	}
	for (int i = 0; i < 4; i++)
	{
		if (m_tiles[x][y]->m_connections[i] == true)
		{
			switch (i)
			{
			case 0:
				if (m_tiles[x][y + 1]->m_goal == true)
				{
					return false;
				}
				break;

			case 1:
				if (m_tiles[x+1][y]->m_goal == true)
				{
					return false;
				}
				break;

			case 2:
				if (m_tiles[x][y -1]->m_goal == true)
				{
					return false;
				}
				break;

			case 3:
				if (m_tiles[x-1][y]->m_goal == true)
				{
					return false;
				}
				break;

			}
		}
	}
	return true;
}

void Mode_Explore::ReadForward(CBReader *reader, int value)
{
	while (true)
	{
		std::string str = reader->ReadString();
		if (str.find("ENTRY") != std::string::npos)
		{
			int v = reader->ReadInt();
			if (v == value)
			{
				break;
			}
		}
	}
}

void Mode_Explore::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 p = touch->getLocation();

	Rect top=Rect(origin.x + 600 - 32, origin.y + 128 + 32, 64, 64);
	Rect bottom= Rect(origin.x + 600 - 32, origin.y + 128 - 32-64, 64, 64);
	Rect Left= Rect(origin.x + 600 - 32-64, origin.y + 128 - 32, 64, 64);
	Rect Right= Rect(origin.x + 600 + 32, origin.y + 128 - 32, 64, 64);
	if (top.containsPoint(p) && m_tiles[(int)m_position.x][(int)m_position.y]->m_connections[0] == true)
	{
		//move up
		m_playersprite->runAction(MoveBy::create(0.25F, Vec2(0, 32)));
		m_position.y += 1;
		m_root->runAction(MoveBy::create(0.25F, Vec2(0, -64)));
		Move(0);
	}
	if (Right.containsPoint(p) && m_tiles[(int)m_position.x][(int)m_position.y]->m_connections[1] == true)
	{
		//move right
		m_playersprite->runAction(MoveBy::create(0.25F, Vec2(32,0)));
		m_position.x += 1;
		m_root->runAction(MoveBy::create(0.25F, Vec2(-64,0)));
		Move(1);
	}
	if (bottom.containsPoint(p) && m_tiles[(int)m_position.x][(int)m_position.y]->m_connections[2] == true)
	{
		m_playersprite->runAction(MoveBy::create(0.25F, Vec2(0, -32)));
		m_root->runAction(MoveBy::create(0.25F, Vec2(0, 64)));
		m_position.y -= 1;
		//move down
		Move(2);
	}
	if (Left.containsPoint(p) && m_tiles[(int)m_position.x][(int)m_position.y]->m_connections[3] == true)
	{
		//move left
		m_position.x -= 1;
		m_playersprite->runAction(MoveBy::create(0.25F, Vec2(-32, 0)));
		m_root->runAction(MoveBy::create(0.25F, Vec2(64, 0)));
		Move(3);
	}

	if (m_grabbable->isVisible() == true && m_grabbable->getBoundingBox().containsPoint(p))
	{
		m_handindex = 1;
		m_hand = m_grabbable;
	}

	if (m_handindex == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_potionsprites[i] != NULL)
			{
				if (m_potionsprites[i]->getBoundingBox().containsPoint(p))
				{
					m_hand = m_potionsprites[i];
					m_handindex = 10 + i;


				}
			}
		}
	}

}

void Mode_Explore::Move(int direction)
{
	Vector <FiniteTimeAction *> action;
	action.pushBack(DelayTime::create(0.25F));
	action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Explore::MoveComplete)));
	Sequence *seq = Sequence::create(action);
	this->runAction(seq);
	this->setTouchEnabled(false);
	m_grabbable->setVisible(false);
}

void Mode_Explore::MoveComplete()
{
	this->setTouchEnabled(true);
	m_tiles[(int)m_position.x][(int)m_position.y]->m_visited = true;
	m_tilesprites[(int)m_position.x][(int)m_position.y]->setTextureRect(Rect(0, 0, 32, 32));
	if (m_tiles[(int)m_position.x][(int)m_position.y]->m_goal == true)
	{
		Sprite *spr = Sprite::create("art/explorationsprites.png", Rect(96, 0, 32, 32));
		spr->setPosition(Vec2(16, 16));
		spr->setAnchorPoint(Vec2(0.5F, 0.5F));
		m_tilesprites[(int)m_position.x][(int)m_position.y]->addChild(spr);
	}
	//process events
	if (m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure<0)
	{
		Trap();
	}
	if (m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure>0)
	{
		//place the grabbable item
		m_grabbable->setPosition(m_background->getPosition());
		m_grabbable->setScale(2);
		m_grabbable->setVisible(true);
		switch (m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure)
		{
		case 1:
			//health
			m_grabbable->setTextureRect(Rect(32, 0, 32, 32));
			break;

		case 2:
			//xp jar
			m_grabbable->setTextureRect(Rect(32, 32, 32, 32));
			break;

		case 3:
			//res 0
			m_grabbable->setTextureRect(Rect(0, 0, 32, 32));
			break;
		case 4:
			//res 1
			m_grabbable->setTextureRect(Rect(0, 32, 32, 32));
			break;
		case 5:
			//res 2
			m_grabbable->setTextureRect(Rect(0, 64, 32, 32));
			break;
		case 6:
			//res 3;
			m_grabbable->setTextureRect(Rect(0, 96, 32, 32));
			break;
		}
	}

	if (m_tiles[(int)m_position.x][(int)m_position.y]->m_enemies[0] != NULL)
	{
		Encounter();
	}
	else if (m_tiles[(int)m_position.x][(int)m_position.y]->m_goal==true)
	{
		VictoryCheck();
	}

	for (int i = 0; i < 3; i++)
	{
		m_frames[i]->RedrawBars();
	}
}
void Mode_Explore::Trap()
{
	int damage = m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure*-1;

	int skin = m_node->m_variables[2];

	int target = -1;
	for (int i = 0; i < 3; i++)
	{
		if (m_frames[i]->m_officer->getHealth()>0)
		{
			if (m_frames[i]->m_officer->getOfficer()->getDefinition()->getName().find("ensign") != std::string::npos)
			{
				target = i;
				break;
			}
		}
	}
	if (target == -1)
	{
		target = rand() % 3;
		while (m_frames[target]->m_officer->getHealth() < 1)
		{
			target = rand() % 3;
		}
	}

	//play trap effect
	std::string str = "art/trap";
	char buffer[3];
	itoa(skin, &buffer[0], 10);
	str.append(buffer);
	str.append(".png");
	Sprite *spr = Sprite::create();
	spr->setPosition(m_background->getPosition());
	spr->setScale(4);
	spr->setZOrder(7);
	this->addChild(spr);
	Animation *animation = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			animation->addSpriteFrame(SpriteFrame::create(str, Rect(j * 64, i * 64, 64, 64)));
		}
	}
	animation->setDelayPerUnit(0.05F);
	Vector <FiniteTimeAction *> action;
	action.pushBack(Animate::create(animation));
	action.pushBack(RemoveSelf::create());
	Sequence *seq = Sequence::create(action);
	spr->runAction(seq);
	//flash target
	action.clear();
	action.pushBack(TintTo::create(0.2F, 255, 0, 0));
	action.pushBack(TintTo::create(0.2F, 255, 255, 255));
	seq = Sequence::create(action);
	m_frames[target]->m_portrait->runAction(seq);
	//fade out target if killed
	if (m_frames[target]->m_officer->Damage(damage))
	{
		action.clear();
		action.pushBack(FadeOut::create(0.8));
		action.pushBack(ToggleVisibility::create());
		seq = Sequence::create(action);
		m_frames[target]->m_portrait->runAction(seq);

		//need to check for mission fail criteria
		Failcheck();
	}

	m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
}

void Mode_Explore::Failcheck()
{
	int alive = 0;
	for (int i = 0; i < 3; i++)
	{
		if (m_frames[i]->m_officer->getHealth()>0)
		{
			alive++;
		}
		else if (m_frames[i]->m_officer->getOfficer()->getDefinition()->getName().find("captain") != std::string::npos)
		{
			//fail
			CleanUp();
			Scene *scene = GameManager::getInstance()->getHandler()->getScene(-2);
			Director::sharedDirector()->replaceScene(scene);
		}
	}

	if (alive == 0)
	{

		//fail
		Label *label = Label::createWithBMFont("art/font.fnt", "Mission failed");
		this->setTouchEnabled(false);
		label->setAnchorPoint(Vec2(0.5F, 0.5F));
		label->setPosition(m_background->getPosition());
		label->setScale(2);
		label->setColor(Color3B(255,0 , 0));
		this->addChild(label);
		Vector <FiniteTimeAction *> action;
		action.pushBack(DelayTime::create(1.0F));
		action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Explore::Defeat)));
		Sequence *seq = Sequence::create(action);
		label->runAction(seq);
	}

}


void Mode_Explore::CleanUp()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_frames[i]->m_officer->getHealth() <= 0)
		{
			for (int j = 0; j < GameManager::getInstance()->getPlayer()->getNumOfficers(); j++)
			{
				if (GameManager::getInstance()->getPlayer()->getOfficer(j) == m_frames[i]->m_officer->getOfficer())
				{
					delete m_frames[i]->m_officer->getOfficer();
					GameManager::getInstance()->getPlayer()->setOfficer(NULL, j);
				}
			}
		}
		if (m_frames[i]->m_officer->getOfficer()->getDefinition()->getName().find("ensign") != std::string::npos)
		{
			delete m_frames[i]->m_officer->getOfficer();
		}
	}

}

void Mode_Explore::Encounter()
{
	Mode_Awayteam *awayteam = Mode_Awayteam::create();
	awayteam->Setup(m_node->m_variables[2], m_officers, m_tiles[(int)m_position.x][(int)m_position.y]->m_enemies,&m_potions[0]);
	awayteam->SetHost(this);
	this->getParent()->addChild(awayteam);
	this->setTouchEnabled(false);
	this->setVisible(false);
}

void Mode_Explore::VictoryCheck()
{
	//check victory nodes
	int count = 1;
	if (m_node->m_variables[0] == 2 || m_node->m_variables[0] == -2)
	{
		count = 5;
	}
	if (m_node->m_variables[0] == 1 || m_node->m_variables[0] == 1)
	{
		count = 3;
	}
	for (int i = 0; i < m_mapwidth; i++)
	{
		for (int j = 0; j < m_mapheight; j++)
		{
			if (m_tiles[i][j] != NULL)
			{
				if (m_tiles[i][j]->m_enemies[0] == NULL && m_tiles[i][j]->m_visited == true && m_tiles[i][j]->m_goal == true)
				{
					count--;
					if (count == 0)
					{
						//victory!
						Label *label = Label::createWithBMFont("art/font.fnt", "Mission complete");
						label->setAnchorPoint(Vec2(0.5F, 0.5F));
						label->setPosition(m_background->getPosition());
						this->setTouchEnabled(false);
						label->setScale(2);
						label->setColor(Color3B(0, 255, 0));
						this->addChild(label);
						Vector <FiniteTimeAction *> action;
						action.pushBack(DelayTime::create(1.0F));
						action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Explore::victory)));
						Sequence *seq = Sequence::create(action);
						label->runAction(seq);
					}
				}
			}
		}
	}
}

void Mode_Explore::victory()
{

	for (int i = 0; i < 4; i++)
	{
		if (m_resources[i]>0)
		{
			GameManager::getInstance()->getPlayer()->getShip()->ModResource(i, m_resources[i]);
		}
	}
	GameManager::getInstance()->getPlayer()->ResetPotions(m_potions);

	CleanUp();
	Scene *scene = GameManager::getInstance()->getHandler()->getScene(0);
	Director::sharedDirector()->replaceScene(scene);

}

void Mode_Explore::Defeat()
{
	CleanUp();
	Scene *scene = GameManager::getInstance()->getHandler()->getScene(1);
	Director::sharedDirector()->replaceScene(scene);

}
void Mode_Explore::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_hand!=NULL)
	{
		m_hand->setPosition(touch->getLocation());
	}
}

void Mode_Explore::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_hand != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			Rect r = m_frames[i]->m_frame->getBoundingBox();
			r.origin = r.origin + m_frames[i]->getPosition();
			r.size = r.size*m_frames[i]->getScale();
			Vec2 p = touch->getLocation();
			if (r.containsPoint(p))
			{
				if (r.containsPoint(p) && m_frames[i]->m_officer->getHealth()>0)
				{
					if (m_handindex == 1)
					{

						switch (m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure)
						{
						case 1:
							m_frames[i]->m_officer->Heal(10);
							m_frames[i]->RedrawBars();
							m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
							m_grabbable->setVisible(false);
							m_frames[i]->RedrawBars();
							break;
						case 2:
							m_frames[i]->m_officer->getOfficer()->addEXP(m_node->m_dc);
							m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
							m_grabbable->setVisible(false);
							break;
						case 3:
							m_resources[0]++;
							m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
							m_grabbable->setVisible(false);
							break;
						case 4:
							m_resources[1]++;
							m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
							m_grabbable->setVisible(false);
							break;
						case 5:
							m_resources[2]++;
							m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
							m_grabbable->setVisible(false);
							break;
						case 6:
							m_resources[3]++;
							m_tiles[(int)m_position.x][(int)m_position.y]->m_traptreasure = 0;
							m_grabbable->setVisible(false);
							break;
						}
					}

					if (m_handindex >= 10)
					{
						if (m_hand != NULL)
						{
							if (ApplyPotion(m_handindex - 10, i))
							{
								m_handindex = 0;
								m_hand = NULL;
							}
						}
					}
				}
			}

		}
		if (m_handindex >= 10)
		{
			if (m_hand != NULL)
			{
				m_hand->setPosition(m_potionframes[m_handindex - 10]->getPosition());
				m_handindex = 0;
				m_hand = NULL;
			}
		}
		if (m_handindex == 1)
		{
			m_hand = NULL;
			m_grabbable->setPosition(m_background->getPosition());
		}
	}
	m_handindex = 0;
}


bool Mode_Explore::ApplyPotion(int index, int target)
{
	//check potion is a recover item

	if (m_potions[index] != NULL && m_officers[target]->getHealth()>0)
	{
		if (m_potions[index]->m_effect->getEffect(0)->m_type == AAT_RECOVER)
		{
			AA_Recover *recover = (AA_Recover*)m_potions[index]->m_effect->getEffect(0);
			if (recover->m_affects == AMT_ENERGY && m_officers[target]->getEnergy() < m_officers[target]->getMaxEP())
			{
				m_officers[target]->ApplyAbility(NULL, m_potions[index]->m_effect->getEffect(0), NULL);

				m_potions[index] = NULL;
				m_frames[target]->RedrawBars();
				this->removeChild(m_potionsprites[index]);
				return true;
			}
			if (recover->m_affects == AMT_HEALTH && m_officers[target]->getHealth() < m_officers[target]->getMaxHP())
			{
				m_officers[target]->ApplyAbility(NULL, m_potions[index]->m_effect->getEffect(0), NULL);
				m_potions[index] = NULL;
				m_frames[target]->RedrawBars();
				this->removeChild(m_potionsprites[index]);
				return true;
			}
		}
	}
	return false;
}

void Mode_Explore::ReturntoLayer()
{
	this->setTouchEnabled(true);
	this->setVisible(true);

	for (int i = 0; i < 3; i++)
	{
		if (m_potions[i] == NULL && m_potionsprites[i] != NULL)
		{
			this->removeChild(m_potionsprites[i]);
			m_potionsprites[i] = NULL;
		}
	}

	//clear tile
	ExploreTile *tile = m_tiles[(int)m_position.x][(int)m_position.y];
	for (int i = 0; i < 6; i++)
	{
		if (tile->m_enemies[i] != NULL)
		{
			if (tile->m_enemies[i]->getHealth() <= 0)
			{
				delete tile->m_enemies[i];
				tile->m_enemies[i] = NULL;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
	//	m_frames[i]->m_officer->setEnergy(0);
		m_frames[i]->RedrawBars();
	}
	for (int i = 0; i < 3; i++)
	{
		m_frames[i]->m_officer->getOfficer()->addEXP(m_node->m_dc);
	}
	VictoryCheck();
}

void Mode_Explore::Save(FILE *file)
{
	char v = 1;
	fwrite((void*)&v, sizeof(char), 1, file);
	//save officers
	for (int i = 0; i < 3; i++)
	{
		m_officers[i]->Save(file);

	}
	//save player position
	fwrite((void*)&m_position.x, sizeof(float), 1, file);
	fwrite((void*)&m_position.y, sizeof(float), 1, file);

	//save grid
	unsigned char x = m_mapwidth;
	unsigned char y = m_mapheight;
	fwrite((void*)&x, sizeof(char), 1, file);
	fwrite((void*)&y, sizeof(char), 1, file);
	for (int k = 0; k < m_mapwidth; k++)
	{
		for (int j = 0; j < m_mapheight; j++)
		{
			if (m_tiles[k][j]!=NULL)
			{
				char v = 42;
				fwrite((void*)&v, sizeof(char), 1, file);
				m_tiles[k][j]->Save(file);
			}
			else
			{
				char v = 17;
				fwrite((void*)&v, sizeof(char), 1, file);
			}
		}

	}

	//save potions
	for (int i = 0; i < 3; i++)
	{
		char v = -1;
		if (m_potions[i] != NULL)
		{
			v = m_potions[i]->m_uid;
		}
		fwrite((void*)&v, sizeof(char), 1, file);
	}
}

cocos2d::Scene* Mode_Explore::createLoadScene(CBReader *reader, MissionNode *node)
{
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Mode_Explore::create();

	// add layer as a child to scene
	scene->addChild(layer,0,100);
	layer->Setup(reader, node);
	return scene;
}

void Mode_Explore::Setup(CBReader *reader, MissionNode *node)
{
	m_node = node;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//load officers
	Officer_Away *officers[3];
	for (int i = 0; i < 3; i++)
	{
		officers[i] = new Officer_Away(reader);

	}
	//load player 
	float y = reader->ReadBFloat();
	m_position = Vec2(y, reader->ReadBFloat());


	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/exploration/enemies.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *enemyreader = new CBReader(buffer, length);

	//load grid
	m_mapwidth = reader->ReadChar();
	m_mapheight = reader->ReadChar();
	m_tiles = new ExploreTile **[m_mapwidth];
	for (int k = 0; k < m_mapwidth; k++)
	{
		m_tiles[k] = new ExploreTile *[m_mapheight];
		for (int j = 0; j < m_mapheight; j++)
		{
			char v = reader->ReadChar();
			if (v == 42)
			{
				m_tiles[k][j] = new ExploreTile(reader,enemyreader);
			}
			else
			{ 
				m_tiles[k][j] = NULL;
			}
		}

	}

	BuildSprites();
	for (int i = 0; i < 3; i++)
	{
		m_frames[i] = OfficerFrame::create();
		m_frames[i]->Setup(officers[i],false);
		this->addChild(m_frames[i]);
		m_frames[i]->setZOrder(5);
		m_frames[i]->setPosition(origin.x + 64 + (128 * i), origin.y + 128);
		m_officers[i] = m_frames[i]->m_officer;
	}
	//set background
	std::string str = "art/background";
	char buf[3];
	itoa(m_node->m_variables[2], &buf[0], 10);
	str.append(buf); str.append(".png");
	m_background = Sprite::create(str);
	this->addChild(m_background);
	m_background->setScale(2);
	m_background->setPosition(origin.x + 400, origin.y + 448);
	m_background->setZOrder(5);

	m_grabbable = Sprite::create("art/rewardsprites.png", Rect(0, 0, 32, 32));
	m_grabbable->setVisible(false);
	m_grabbable->setPosition(m_background->getPosition());
	m_grabbable->setZOrder(6);
	this->addChild(m_grabbable);

	this->setTouchEnabled(true);

	if (m_tiles[(int)m_position.x][(int)m_position.y] != NULL)
	{
		if (m_tiles[(int)m_position.x][(int)m_position.y]->m_enemies[0] != NULL)
		{
			Encounter();
		}
	}

	//load potions
	for (int i = 0; i < 3; i++)
	{
		char v = reader->ReadChar();
		if (v == -1)
		{
			m_potions[i] = NULL;
		}
		else
		{
			m_potions[i] = ReferenceLibrary::getInstance()->getPotion(v);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_potionframes[i] = Sprite::create("art/crewpanel.png", Rect(32, 32, 32, 32));
		m_potionframes[i]->setPosition(origin.x + 412, origin.y + 32 + (64 * i));
		this->addChild(m_potionframes[i]);
		m_potionframes[i]->setScale(2);
		if (m_potions[i] != NULL)
		{
			m_potionsprites[i] = Sprite::create("art/potions.png", Rect(m_potions[i]->m_graphicpos.x * 32, m_potions[i]->m_graphicpos.y * 32, 32, 32));
			m_potionsprites[i]->setPosition(m_potionframes[i]->getPosition());
			m_potionsprites[i]->setScale(2);
			m_potionsprites[i]->setZOrder(5);
			this->addChild(m_potionsprites[i]);
		}
		else
		{
			m_potionsprites[i] = NULL;
		}
	}

}