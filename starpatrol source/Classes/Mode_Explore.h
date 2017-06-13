#ifndef MODEEXPLORE
#define MODEEXPLORE

#include "Mode_Awayteam.h"
#include "ReturnableLayer.h"
#include "Common.h"
#include "MissionNode.h"
#include "ExploreTile.h"
#include "SaveLayer.h"
#include "Potion.h"

enum EXPLOREGOAL{eg_exterminate,eg_assassinate,eg_secure};

class Mode_Explore : protected SaveLayer
{
	Officer_Away *m_officers[3];
	MissionNode *m_node;
	int m_mapwidth, m_mapheight;
	Vec2 m_position;
	Node *m_root;
	ExploreTile ***m_tiles;
	Sprite ***m_tilesprites;
	OfficerFrame *m_frames[3];
	Sprite *m_background;
	Sprite *m_playersprite;
	Sprite *m_grabbable;
	int m_handindex;
	Sprite *m_hand;
	int m_resources[4];
	Sprite *m_potionframes[3];
	Sprite *m_potionsprites[3];

	bool EnemyPlacementCheck(int x, int y,bool cangoal);
	void Enemyspawn(int x, int y, int variables[6], CBReader *cbr);
	void EnemyPlacement(MissionNode *node);
	void RewardGen(MissionNode *node);
	bool Mapgen(MissionNode *node);
	void GoalGen(MissionNode *node);
	void PlaceTraps(int count, int strength);
	void Goaldrop(int count);
	bool Goaltest(int x, int y);
	void ReadForward(CBReader *reader, int value);
	int GenMove(int *x, int *y,int r);
	bool GenStartPoint(int x, int y);
	bool IslandCheck();
	void RecursiveIslander(int x, int y, bool **grid);
	bool GenConnection(int x, int y, bool **grid);
	void Move(int direction);
	void Trap();
	void Encounter();
	void VictoryCheck();
	void Failcheck();
	void CleanUp();
	void BuildSprites();

	Potion *m_potions[3];

	bool ApplyPotion(int index, int target);

public:
	CREATE_FUNC(Mode_Explore);
	~Mode_Explore();
	static cocos2d::Scene* createScene(Officer *officers[3], MissionNode *stage,Potion *potions[3]);
	static cocos2d::Scene* createDebugScene();
	virtual bool init();

	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void Setup(Officer* officers[3], MissionNode *stage, Potion *potions[3]);
	void ReturntoLayer();
	void MoveComplete();
	void victory();
	void Defeat();
	void Save(FILE *file);
	static cocos2d::Scene* createLoadScene(CBReader *reader, MissionNode *node);
	void Setup(CBReader *reader, MissionNode *node);
};

#endif