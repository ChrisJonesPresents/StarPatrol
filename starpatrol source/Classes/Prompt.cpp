#include "Prompt.h"

bool Prompt::init()
{
	if (!Layer::init())
	{
		return false;
	}
	this->setTouchEnabled(true);
	return true;
}

void Prompt::Setup(ReturnableLayer *host, std::string text)
{
	m_host = host;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//build frame
	Sprite *spr = Sprite::create("art/frame.png");
	spr->setScale(3.3F);
	spr->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	this->addChild(spr);
	//build label

	Label *label = Label::createWithBMFont("art/font.fnt", text); 
	label->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	this->addChild(label);
	label->setWidth(384);
}

void Prompt::onTouchesBegan(const std::vector< cocos2d::Touch * > &touches, cocos2d::Event *unused_event)
{
	m_host->ReturntoLayer();
	this->getParent()->removeChild(this);
}