#ifndef MODEWARPGATE
#define MODEWARPGATE

#include "cocos2d.h"
#include "Player.h"
#include "ui\UIButton.h"
USING_NS_CC;
class ModeWarpgate : Layer
{
	Player *m_player;

public:
	CREATE_FUNC(ModeWarpgate);

	static cocos2d::Scene* createScene(Player *playerfile);

	~ModeWarpgate();
	virtual bool init();

	void Setup(Player *playerfile);

	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);
	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
};

#endif