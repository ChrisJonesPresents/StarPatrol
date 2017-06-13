#ifndef MODEMENU
#define MODEMENU
#include "cocos2d.h"
#include "ReturnableLayer.h"
#include <ui\UIButton.h>

class ModeMenu : public ReturnableLayer
{


public:
	CREATE_FUNC(ModeMenu);
	static cocos2d::Scene* createScene(int origin=0);
	virtual bool init();


	void menuResumeCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuSaveCallback(cocos2d::Ref* pSender);
	void menuSelectCallback(cocos2d::Ref* pSender);

	int m_origin;
	void ReturnToLayer(){};
};
#endif