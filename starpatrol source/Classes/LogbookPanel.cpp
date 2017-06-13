#include "LogbookPanel.h"
#include "GameManager.h"
#include "ShipPanel.h"
#include "CrewPanel.h"
#include "Mode_Starscape.h"

cocos2d::Scene* LogbookPanel::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LogbookPanel::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool LogbookPanel::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_active = true;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//build block behind the buttons
	Sprite *spr2 = Sprite::create("art/patternedui.png");
	//	spr2->setScale(1.60F);
	spr2->setScaleX(1.55F);
	spr2->setPosition(origin.x + 100, origin.y + 64);
	this->addChild(spr2);
	/*
	spr2 = Sprite::create("art/patternedui.png");
	//	spr2->setScale(1.60F);
	spr2->setScaleX(1.55F);
	spr2->setPosition(origin.x + 700, origin.y + 64);
	this->addChild(spr2);
	*/

	cocos2d::ui::Button *buttons[4];
	//build menu button
	buttons[0] = ui::Button::create("art/starscapebuttona0.png", "art/starscapebuttona1.png", "art/bluebutton2.png");
	buttons[0]->addClickEventListener(CC_CALLBACK_1(LogbookPanel::ButtonCallback0, this));
	buttons[0]->setPosition(Vec2(cocos2d::Vec2(origin.x + 16, origin.y + 110)));
	buttons[0]->setScale(1);
	//build ship button
	buttons[1] = ui::Button::create("art/starscapebuttonb0.png", "art/starscapebuttonb1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(LogbookPanel::ButtonCallback1, this));
	buttons[1]->setPosition(Vec2(cocos2d::Vec2(origin.x + 48, origin.y + 110)));
	buttons[1]->setScale(1);
	//build officers button
	buttons[2] = ui::Button::create("art/starscapebuttonc0.png", "art/starscapebuttonc1.png", "art/bluebutton2.png");
	buttons[2]->addClickEventListener(CC_CALLBACK_1(LogbookPanel::ButtonCallback2, this));
	buttons[2]->setPosition(Vec2(cocos2d::Vec2(origin.x + 80, origin.y + 110)));
	buttons[2]->setScale(1);

	//build log button
	buttons[3] = ui::Button::create("art/starscapebuttone0.png", "art/starscapebuttone1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(LogbookPanel::ButtonCallback3, this));
	buttons[3]->setPosition(Vec2(cocos2d::Vec2(origin.x + 112, origin.y + 110)));
	buttons[3]->setScale(1);
	for (int i = 0; i < 4; i++)
	{
		this->addChild(buttons[i]);
	}

	//build description frame
	Sprite *m_descriptionframe = Sprite::create("art/frame.png");
	m_descriptionframe->setScaleY(1);
	m_descriptionframe->setScaleX(4.65F);
	m_descriptionframe->setPosition(Vec2(origin.x + (visibleSize.width*0.62F), origin.y + 64));
	this->addChild(m_descriptionframe);
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_description->setAlignment(TextHAlignment::LEFT);
	m_description->setPosition(m_descriptionframe->getPosition());
	m_description->setSystemFontSize(24);
	m_description->setWidth(560);

	this->addChild(m_description);

	m_offset = 0;
	Logbook *logs = GameManager::getInstance()->getPlayer()->getLogbook();
	if (logs->getLength() > 16)
	{
		m_offset = logs->getLength() - 16;
	}
	m_index = -1;
	//build logs
	for (int i = 0; i < 16; i++)
	{
		m_slots[i] = Sprite::create("art/logbookui.png", Rect(0, 0, 256, 32));
		m_slots[i]->setScaleX(3);
		m_slots[i]->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 16 - (32 * i));
		this->addChild(m_slots[i]);
		//labels
		m_labels[i] = Label::createWithBMFont("art/font.fnt", "text text text text text text text text text");
		m_labels[i]->setPosition(m_slots[i]->getPosition()-Vec2(288,0));
		m_labels[i]->setScale(0.8F);
		m_labels[i]->setAnchorPoint(Vec2(0.0F, 0.5F));
		this->addChild(m_labels[i]);
		//icons
		m_subsprites[i] = Sprite::create("art/logbookui.png", Rect(32, 96, 32, 32));
		m_subsprites[i]->setPosition(Vec2(16, 16));
		m_subsprites[i]->setScaleX(0.33F);
		m_slots[i]->addChild(m_subsprites[i]);

	}
	Rebuild();
	//add 'new entry button'

	this->setTouchEnabled(true);

	return true;
}

void LogbookPanel::Rebuild()
{
	Logbook *logs = GameManager::getInstance()->getPlayer()->getLogbook();
	int index = m_offset;
	for (int i = 0; i < 16; i++)
	{
		if (index < logs->getLength())
		{
			m_subsprites[i]->setVisible(true);
			m_labels[i]->setVisible(true);
			m_slots[i]->setTextureRect(Rect(0, 32, 256, 32));
			std::string str = "";
			char buffer[6];
			itoa(logs->getEntry(i)->m_timestamp % 30, &buffer[0], 10);
			str.append(buffer); str.append("/");
			itoa((((logs->getEntry(i)->m_timestamp) / 30) % 12) +1, &buffer[0], 10);
			str.append(buffer); str.append("/");
			itoa(2115+(logs->getEntry(i)->m_timestamp / 360), &buffer[0], 10);
			str.append(buffer); str.append("  ");

			str.append(logs->getEntry(index)->m_string.substr(0, 48));
			if (logs->getEntry(index)->m_string.length()>48)
			{
				str.append("...");
			}
			m_labels[i]->setString(str);
			if (logs->getEntry(index)->m_readonly == true)
			{
				m_subsprites[i]->setTextureRect(Rect(32, 96, 32, 32));
			}
			else
			{
				m_subsprites[i]->setTextureRect(Rect(0, 96, 32, 32));
			}
		}
		else
		{
			m_slots[i]->setTextureRect(Rect(0, 0, 256, 32));
			m_subsprites[i]->setVisible(false);
			m_labels[i]->setVisible(false);
		}
		index++;
	}

}

void LogbookPanel::ReturntoLayer()
{

	//build new log entry

}

void 	LogbookPanel::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	m_shift = 0;
	Logbook *logs = GameManager::getInstance()->getPlayer()->getLogbook();
	Touch *touch = touches[0];
	for (int i = 0; i < 16; i++)
	{
		if (m_slots[i]->getBoundingBox().containsPoint(touch->getLocation()))
		{
		
			if (m_slots[i]->getTextureRect().origin.y>0)
			{
				m_description->setString(logs->getEntry(m_offset + i)->m_string);
			}
		}
	}

}

void LogbookPanel::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	m_shift += touch->getDelta().y;
	if (m_shift > 32)
	{
		ShiftUp();

		m_shift = 0;
	}
	if (m_shift < -32)
	{
		ShiftDown();

		m_shift = 0;
	}

}

void  LogbookPanel::ShiftUp()
{
	Logbook *logs = GameManager::getInstance()->getPlayer()->getLogbook();
	if (m_offset > 0)
	{
		m_offset--;
		Rebuild();
	}
}

void  LogbookPanel::ShiftDown()
{
	Logbook *logs = GameManager::getInstance()->getPlayer()->getLogbook();
	if (m_offset < logs->getLength() - 1)
	{
		m_offset++;
		Rebuild();
	}
}

void 	LogbookPanel::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{


}

void LogbookPanel::ButtonCallback0(Ref *pSender)
{

}

void LogbookPanel::ButtonCallback1(Ref *pSender)
{
	if (m_active == true)
	{


		Scene *scene = ShipPanel::createScene(GameManager::getInstance()->getPlayer());
		Director::getInstance()->replaceScene(scene);
	}
}

void LogbookPanel::ButtonCallback2(Ref *pSender)
{
	if (m_active == true)
	{
		Scene *scene = CrewPanel::createScene(GameManager::getInstance()->getPlayer());
		Director::getInstance()->replaceScene(scene); 
	}
}

void LogbookPanel::ButtonCallback3(Ref *pSender)
{
	if (m_active == true)
	{
		Scene *scene = Mode_Starscape::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void LogbookPanel::ButtonCallback4(Ref *pSender)
{

}

void LogbookPanel::ButtonCallback5(Ref *pSender)
{

}

void LogbookPanel::ButtonCallback6(Ref *pSender)
{

}

void LogbookPanel::ButtonCallback7(Ref *pSender)
{

}