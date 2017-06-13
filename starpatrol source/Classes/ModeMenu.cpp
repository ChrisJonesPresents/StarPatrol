#include "ModeMenu.h"
#include "GameManager.h"
#include "Mode_Starscape.h"
#include "ShipPanel.h"
#include "CrewPanel.h"
#include "Logbookpanel.h"
#include "ModeStart.h"

USING_NS_CC;

cocos2d::Scene* ModeMenu::createScene(int origin)
{
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = ModeMenu::create();
	layer->m_origin = origin;
	scene->addChild(layer);
	return scene;
}

bool ModeMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//build logo
	Sprite *sprite = Sprite::create("art/logo.png");
	sprite->setPosition(origin.x + visibleSize.width / 2, origin.y + 550);
	sprite->setScale(2);
	this->addChild(sprite);
	//build buttons
	ui::Button *buttons[4];
	//build resume button to only work if game manager is active

		buttons[0] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
		buttons[0]->addClickEventListener(CC_CALLBACK_1(ModeMenu::menuResumeCallback, this));
		if (GameManager::getInstance()->getPlayer() == NULL)
		{
			buttons[0]->setTitleText("Begin");
		}
		else
		{
			buttons[0]->setTitleText("RESUME");
		}



	//build sector select
	buttons[1] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(ModeMenu::menuSelectCallback, this));
	buttons[1]->setTitleText("SECTOR");
	//build save button to only work if game manager is active

		buttons[2] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
		buttons[2]->addClickEventListener(CC_CALLBACK_1(ModeMenu::menuSaveCallback, this));
		buttons[2]->setTitleText("SAVE");

	//build exit

	buttons[3] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(ModeMenu::menuCloseCallback, this));
	buttons[3]->setTitleText("EXIT");

	for (int i = 0; i < 4; i++)
	{
		if (buttons[i] != NULL)
		{
			this->addChild(buttons[i]);
			buttons[i]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 400-(100*i)));
			buttons[i]->setScale(2);
		}
	}

	return true;
}


void ModeMenu::menuResumeCallback(cocos2d::Ref* pSender)
{
	Scene *scene = NULL;
	if (GameManager::getInstance()->getPlayer() == NULL)
	{
		scene = ModeStart::createScene();
	
	}
	else
	{
		switch (m_origin)
		{
		case 0:
			scene = Mode_Starscape::createScene();
			break;
		case 1:
			scene = ShipPanel::createScene(GameManager::getInstance()->getPlayer());
			break;
		case 2:
			scene = CrewPanel::createScene(GameManager::getInstance()->getPlayer());
			break;
		case 3:
			scene = LogbookPanel::createScene();
			break;

		}

	}

	Director::sharedDirector()->replaceScene(scene);
}

void ModeMenu::menuCloseCallback(cocos2d::Ref* pSender)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
	#endif

		Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}
void ModeMenu::menuSaveCallback(cocos2d::Ref* pSender)
{


}
void ModeMenu::menuSelectCallback(cocos2d::Ref* pSender)
{


}