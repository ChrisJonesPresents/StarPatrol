#ifndef MODEPUNISH
#define MODEPUNISH

#include "MissionNode.h"
#include "Player.h"
#include <cocos2d.h>
#include "SaveLayer.h"
#include "ui\UIButton.h"

class ModePunish : public SaveLayer
{
	int m_numbers[2];
	Player *m_player;

public:
	CREATE_FUNC(ModePunish);

	static cocos2d::Scene* createScene(Player *playerfile, MissionNode *stage);

	virtual bool init();
	void Save(FILE *file);
	void ReturntoLayer(){};
	void Setup(Player *playerfile, MissionNode *stage, bool apply=true);
	void ButtonCallback0(Ref *pSender);
	void Load(CBReader *reader);
	static cocos2d::Scene* createSceneLoad(Player *playerfile, MissionNode *stage,CBReader *reader);

};


#endif