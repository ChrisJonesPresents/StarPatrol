#include "CrewPanel.h"
#include "Mode_Starscape.h"
#include "ShipPanel.h"
#include "OfficerView.h"
#include "ShopLayer.h"
#include "LogbookPanel.h"
#include "ModeMenu.h"

cocos2d::Scene* CrewPanel::createScene(Player *playerfile)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CrewPanel::create();
	layer->Setup(playerfile);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


bool CrewPanel::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_active = true;
	m_handindex = -1;
	m_hand = NULL;

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *spr3 = Sprite::create("art/gridblue.png");
	spr3->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	spr3->setScale(3.125F);
	this->addChild(spr3);

	Sprite *spr2 = Sprite::create("art/patternedui.png");
	//	spr2->setScale(1.60F);
	spr2->setScaleX(1.55F);
	spr2->setPosition(origin.x + 100, origin.y + 64);
	this->addChild(spr2);

	m_imageframe = Sprite::create("art/frame.png");
	m_imageframe->setScale(0.75F);
	m_imageframe->setPosition(origin.x + 152, origin.y + 48);
	this->addChild(m_imageframe);


	cocos2d::ui::Button *buttons[4];
	//build menu button
	buttons[0] = ui::Button::create("art/starscapebuttona0.png", "art/starscapebuttona1.png", "art/bluebutton2.png");
	buttons[0]->addClickEventListener(CC_CALLBACK_1(CrewPanel::ButtonCallback0, this));
	buttons[0]->setPosition(Vec2(cocos2d::Vec2(origin.x + 16, origin.y + 110)));
	buttons[0]->setScale(1);
	//build ship button
	buttons[1] = ui::Button::create("art/starscapebuttonb0.png", "art/starscapebuttonb1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(CrewPanel::ButtonCallback1, this));
	buttons[1]->setPosition(Vec2(cocos2d::Vec2(origin.x + 48, origin.y + 110)));
	buttons[1]->setScale(1);
	//build officers button
	buttons[2] = ui::Button::create("art/starscapebuttone0.png", "art/starscapebuttone1.png", "art/bluebutton2.png");
	buttons[2]->addClickEventListener(CC_CALLBACK_1(CrewPanel::ButtonCallback2, this));
	buttons[2]->setPosition(Vec2(cocos2d::Vec2(origin.x + 80, origin.y + 110)));
	buttons[2]->setScale(1);

	//build log button
	buttons[3] = ui::Button::create("art/starscapebuttond0.png", "art/starscapebuttond1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(CrewPanel::ButtonCallback3, this));
	buttons[3]->setPosition(Vec2(cocos2d::Vec2(origin.x + 112, origin.y + 110)));
	buttons[3]->setScale(1);
	for (int i = 0; i < 4; i++)
	{
		this->addChild(buttons[i]);
	}
	Sprite *m_descriptionframe;
	m_descriptionframe = Sprite::create("art/frame.png");
	m_descriptionframe->setScaleY(1);
	m_descriptionframe->setScaleX(3.2F);
	m_descriptionframe->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + 64));
	this->addChild(m_descriptionframe);
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_description->setAlignment(TextHAlignment::LEFT);
	m_description->setPosition(m_descriptionframe->getPosition());
	m_description->setSystemFontSize(18);
	m_description->setWidth(380);

	this->addChild(m_description);
	this->setTouchEnabled(true);

	//draw crew count icon
	Sprite *spr = Sprite::create("art/crewpanel.png", Rect(0, 0, 32, 32));
	spr->setPosition(origin.x + 16, origin.y + 78);
	this->addChild(spr);
	//draw background
	spr = Sprite::create("art/backbox.png");
	spr->setAnchorPoint(Vec2(0, 0.5F));
	spr->setPosition(origin.x + 32, origin.y + 78);
	spr->setScaleX(1.15F);
	this->addChild(spr);
	//draw label
	m_count = Label::createWithBMFont("art/font.fnt", "");
	m_count->setPosition(spr->getPosition()+Vec2(62,0));
	m_count->setAnchorPoint(Vec2(1, 0.5F));
	m_count->setSystemFontSize(18);
	this->addChild(m_count);

	return true;
}

void CrewPanel::Setup(Player *playerfile)
{
	playerfile->CompactPotions();
	m_view.Setup(playerfile, this,true);

	//RefreshCrewcount();
}

void CrewPanel::ButtonCallback0(Ref *pSender)
{
	Scene *scene = ModeMenu::createScene(2);
	Director::sharedDirector()->replaceScene(scene);
}

void CrewPanel::ButtonCallback1(Ref *pSender)
{
	if (m_active == true)
	{
		Scene *scene = ShipPanel::createScene(GameManager::getInstance()->getPlayer());
		Director::getInstance()->replaceScene(scene);
	}

}

void CrewPanel::ButtonCallback2(Ref *pSender)
{
	if (m_active == true)
	{
		Scene *scene = Mode_Starscape::createScene();
		Director::getInstance()->replaceScene(scene);
	}
	//back to starscape
}

void CrewPanel::ButtonCallback3(Ref *pSender)
{
	//initiate log panel
	Scene *scene = LogbookPanel::createScene();
	Director::getInstance()->replaceScene(scene);
}

void CrewPanel::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	//figure out if we've touched inside an officer panel and open the officer view if so
	for (int i = 0; i < 6; i++)
	{
		if (m_view.m_panels[i]->getBoundingBox().containsPoint(touch->getLocation()))
		{
			if (m_view.m_player->getOfficer(i) != NULL)
			{
				OfficerView *view = OfficerView::create();
				view->Setup(m_view.m_player->getOfficer(i), this, m_description);
				this->getParent()->addChild(view);
				this->setTouchEnabled(false);
				//this->setVisible(false);
				break;
			}

		}
		if (m_view.m_frames[i]->getBoundingBox().containsPoint(touch->getLocation()))
		{
			if (m_view.m_potions[i] != NULL)
			{
				m_description->setString(m_view.m_player->getPotion(i)->m_potion->m_description);
				m_handindex = i;
				m_hand = m_view.m_potions[i];
			}
		}
	}


}

void CrewPanel::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_hand != NULL)
	{
		m_hand->setPosition(touch->getLocation());
	}
}

void CrewPanel::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	//check if binning

	if (m_hand != NULL)
	{
		if (m_view.m_bin->getBoundingBox().containsPoint(touch->getLocation()) && m_handindex>=0 && m_handindex<6)
		{
			m_view.DiscardPotion(m_handindex,this);
			m_hand = NULL;
		}
	}
		//check if moving between slots

	if (m_hand != NULL && m_handindex>=0 && m_handindex<6)
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_view.m_frames[i]->getBoundingBox().containsPoint(touch->getLocation()) && m_handindex!=i && m_view.m_potions[i]==NULL)
			{
				m_view.MovePotion(i, m_handindex,this);
				m_view.m_potions[m_handindex] = NULL;
				m_hand = NULL;
			}
		}
	}


	if (m_hand != NULL)
	{

		//put back
		if (m_handindex >= 0 && m_handindex < 6)
		{
			m_hand->setPosition(m_view.m_frames[m_handindex]->getPosition());
			m_handindex = -1;
			m_hand = NULL;
		}

	}


}

void CrewPanel::ReturntoLayer()
{
	m_active = true;
	this->setTouchEnabled(true);
	RefreshRoster(); 
	if (m_discard > 0)
	{
		ShipCrew *crew = (ShipCrew*)m_view.m_player->getShip()->getCrew()->at(m_handindex);
		crew->m_count -= m_discard;
		if (crew->m_count == 0)
		{
			m_view.m_player->getShip()->getCrew()->erase(m_handindex);
			RefreshRoster();
		}
		else
		{
			char buffer[3];
			itoa(crew->m_count, &buffer[0], 10);


		}
		//RefreshCrewcount();
		m_discard = 0;
		m_handindex = 0; 
	}
}

void CrewPanel::RefreshCrewcount()
{
	int c = 0;
	char buffer[4];
	for (int i = 0; i < m_view.m_player->getShip()->getCrew()->size(); i++)
	{
		if (m_view.m_player->getShip()->getCrew()->at(i) != NULL)
		{
			ShipCrew *crew = (ShipCrew*)m_view.m_player->getShip()->getCrew()->at(i);
			c += crew->m_count;
		}
	}
	itoa(c, &buffer[0], 10);
	std::string str = buffer;
	str.append("/");
	itoa(m_view.m_player->getShip()->getMaxCrew(), &buffer[0], 10);
	str.append(buffer);
	m_count->setString(str);
}

void CrewPanel::RefreshRoster()
{
	m_view.RefreshRoster();
}