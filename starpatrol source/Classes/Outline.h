#ifndef SHIPOUTLINE
#define SHIPOUTLINE

#include "Playership.h"
#include "Player.h"
#include "cocos2d.h"
#include "Module_commodity.h"

USING_NS_CC;
class Outline
{
	
	cocos2d::Vec2 getModulePos(int i, int type);
	bool m_cansell;
public:
	int m_heldsprite;
	cocos2d::Sprite **m_slots;
	cocos2d::Sprite **m_modulesprites;
	cocos2d::Sprite *m_bin;
	cocos2d::Sprite *m_fuelbin;
	cocos2d::Sprite *m_itemslots[3];
	cocos2d::Sprite *m_items[3];

	void Setup(Playership *ship, cocos2d::Layer *layer, Player *player,bool cansell=false);
	~Outline();
	void BinThing(Layer *layer);
	void ResetPosition();
	bool MoveModule(Vec2 p);
	void ReLabelCommodity(int i);
	Player *m_player;

	bool AddPurchase(int type, int *value,Vec2 p,Sprite *sprite);
};


#endif