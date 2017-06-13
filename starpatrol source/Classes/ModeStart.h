#ifndef MODESTART
#define MODESTART

#include "cocos2d.h"
#include "ReturnableLayer.h"
#include "RenamePanel.h"
#include "ui\UIButton.h"

USING_NS_CC;

class ModeStart : public ReturnableLayer
{
	bool m_active;
	std::string m_names[2];
	ui::Button *m_nametags[2];

public:
	CREATE_FUNC(ModeStart);
	virtual bool init();

	static cocos2d::Scene* createScene();

	void ButtonCallback0(cocos2d::Ref* pSender);
	void ButtonCallback1(cocos2d::Ref* pSender);
	void ButtonCallback2(cocos2d::Ref* pSender);
	void ButtonCallback3(cocos2d::Ref* pSender);
	void ReturntoLayer();
};

#endif