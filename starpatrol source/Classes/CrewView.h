#ifndef CREWVIEW
#define CREWVIEW

#include "cocos2d.h"
#include "Player.h"
#include "Playership.h"

USING_NS_CC;
class CrewView
{
public:
	Player *m_player;

	Sprite *m_panels[6];
	Label *m_nametags[6];

	Sprite *m_potions[6];
	Sprite *m_frames[6];
	Label *m_potionlabels[6];

	Sprite *m_bin;

	~CrewView();
	void Setup(Player *player, cocos2d::Layer *layer,bool hasbin=false);

	void RefreshRoster();

	void DiscardPotion(int index,Layer *layer);
	bool MovePotion(int destination, int source, Layer *layer);
	Potion *TakePotion(int slot);
	void ReturnPotion(Potion *potion);
};

#endif