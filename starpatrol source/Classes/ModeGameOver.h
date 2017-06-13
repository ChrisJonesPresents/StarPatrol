#ifndef MODEGAMEOVER
#define MODEGAMEOVER

#include "cocos2d.h"
#include "MissionNode.h"
#include "Common.h"
#include "ui\UIButton.h"

class ModeGameOver : public cocos2d::Layer
{

public:
	virtual bool init();
	CREATE_FUNC(ModeGameOver);
	static cocos2d::Scene* createScene(MissionNode *stage);

	void Setup(int index);
	void ButtonCallback(cocos2d::Ref* pSender);
};

#endif