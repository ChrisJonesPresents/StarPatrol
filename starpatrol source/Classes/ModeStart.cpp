#include "ModeStart.h"
#include "ReferenceLibrary.h"
#include "GameManager.h"
#include "ModeMenu.h"
#include "Mode_Starscape.h"

bool ModeStart::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_active = true;

	m_names[0] = ReferenceLibrary::getName("captain");
	m_names[1] = ReferenceLibrary::getName("ship");
	char buffer[7];
	itoa(GameManager::getInstance()->getYears(), &buffer[0], 10);
	std::string preamble = "In the year "; preamble.append(buffer); preamble.append(" the brave captain ");
	//build pre-amble
	Label *pre = Label::createWithBMFont("art/font.fnt", preamble);
	pre->setWidth(656);
	pre->setPosition(origin.x + (visibleSize.width / 2), origin.y + 500);
	this->addChild(pre);
	//build nametag
	m_nametags[0] = ui::Button::create("art/backbox2.png", "art/backbox2.png");
	m_nametags[0]->addClickEventListener(CC_CALLBACK_1(ModeStart::ButtonCallback0, this));
	m_nametags[0]->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + 400));
	m_nametags[0]->setTitleText(m_names[0]);
	m_nametags[0]->setScale(2);
	this->addChild(m_nametags[0]);
	//build mid amble
	Label *mid = Label::createWithBMFont("art/font.fnt", " along with his brave crew aboard the good ship ");
	mid->setWidth(656);
	mid->setPosition(origin.x + (visibleSize.width / 2), origin.y + 300);
	this->addChild(mid);
	//build shipnametag
	m_nametags[1] = ui::Button::create("art/backbox2.png", "art/backbox2.png");
	m_nametags[1]->addClickEventListener(CC_CALLBACK_1(ModeStart::ButtonCallback1, this));
	m_nametags[1]->setPosition(Vec2(origin.x+(visibleSize.width / 2), origin.y + 200));
	m_nametags[1]->setTitleText(m_names[1]);
	m_nametags[1]->setScale(2);
	this->addChild(m_nametags[1]);
	//build post amble
	Label *post = Label::createWithBMFont("art/font.fnt", " were assigned to safeguard and develop this frontier sector under orders from space command.");
	post->setWidth(656);
	post->setPosition(origin.x + (visibleSize.width / 2), origin.y + 100);
	this->addChild(post);
	//build boldly go button
	ui::Button *button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png");
	button->setTitleText("Begin mission");
	button->addClickEventListener(CC_CALLBACK_1(ModeStart::ButtonCallback2, this));
	button->setPosition(Vec2(origin.x + (visibleSize.width / 2), 32));
	this->addChild(button);
	//built back button
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png");
	button->setTitleText("Back");
	button->addClickEventListener(CC_CALLBACK_1(ModeStart::ButtonCallback2, this));
	button->setPosition(Vec2(origin.x + 64, 32));
	this->addChild(button);
	return true;
}

cocos2d::Scene* ModeStart::createScene()
{
	Layer *layer = ModeStart::create();

	Scene *scene = Scene::create();
	scene->addChild(layer);

	return scene;
}


void ModeStart::ButtonCallback0(cocos2d::Ref* pSender)
{
	if (m_active == true)
	{
		m_active = false;
		RenamePanel *panel = RenamePanel::create();
		panel->Setup(&m_names[0], "captain name", this);
		this->getParent()->addChild(panel);
	}

}

void ModeStart::ButtonCallback1(cocos2d::Ref* pSender)
{
	if (m_active == true)
	{
		m_active = false;
		RenamePanel *panel = RenamePanel::create();
		panel->Setup(&m_names[1], "ship name", this);
		this->getParent()->addChild(panel);
	}

}

void ModeStart::ButtonCallback2(cocos2d::Ref* pSender)
{
	if (m_active == true)
	{
		GameManager::getInstance()->NewCaptain(m_names);
		Scene *scene = Mode_Starscape::createScene(false);

		Director::sharedDirector()->replaceScene(scene);
	}

}

void ModeStart::ButtonCallback3(cocos2d::Ref* pSender)
{

	if (m_active == true)
	{
		Scene *scene = ModeMenu::createScene(0);
		Director::sharedDirector()->replaceScene(scene);
	}

}

void ModeStart::ReturntoLayer()
{
	m_active = true;
	for (int i = 0; i < 2; i++)
	{
		m_nametags[i]->setTitleText(m_names[i]);
	}
}