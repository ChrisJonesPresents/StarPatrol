#include "SolarView.h"
#include "GameManager.h"
#include "ModeWarpgate.h"
#include "ModeColony.h"
#include "ReferenceLibrary.h"

USING_NS_CC;
bool SolarView::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_frame = Sprite::create("art/solarback.png");
	m_frame->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	m_frame->setScale(4);
	this->addChild(m_frame);

	this->setTouchEnabled(true);

	//add exit button
	ui::Button *button = NULL;
	button = ui::Button::create("art/starscapebuttone0.png", "art/starscapebuttone1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(SolarView::ButtonCallback0, this));
	button->setPosition(Vec2(origin.x + (visibleSize.width / 2)+240, origin.y + (visibleSize.height / 2)+240));
	this->addChild(button);
	m_colonize = false;
	return true;
}

SolarView::~SolarView()
{
	delete []m_locations;
	delete[]m_questionmarks;
}

int getY(LocationType lt)
{
	switch (lt)
	{
	case LT_GAS:
		return 0;
	case LT_BARREN:
		return 1;
	case LT_ICE:
		return 2;
	case LT_HELL:
		return 3;
	case LT_GARDEN:
		return 4;
	case LT_ASTEROID:
		return 5;
	case LT_STATION:
		return 6;
	case LT_SHIP:
		return 7;

	}

}

bool SolarView::CanColonize()
{
	if (m_star->getType() == ST_BLACK)
	{
		return false;
	}
	bool colony = false;
	bool missions = false;
	bool license = false;
	for (int i = 0; i < m_star->getNumLoc(); i++)
	{
		if (m_star->getLocations()[i] != NULL)
		{
			if (m_star->getLocations()[i]->getMission() != NULL)
			{
				missions = true;
			}
			if (m_star->getLocations()[i]->getColony() != NULL)
			{
				colony = true;
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (GameManager::getInstance()->getPlayer()->getItem(i) != NULL)
		{
			if (GameManager::getInstance()->getPlayer()->getItem(i)->m_type == ITYPE_LICENSE)
			{
				license = true;
			}
		}
	}
	if (colony == false && missions == false && license == true)
	{
		return true;
	}
	return false;
}

void SolarView::Setup(Layer *main,Star *star)
{
	m_star = star;
	m_main = main;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//build star name top
	cocos2d::Label *label = Label::createWithBMFont("art/font.fnt", m_star->getName());

	label->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2) + 304);
	this->addChild(label);
	//build star
	Sprite *spr = Sprite::create("art/galaxysprites.png", Rect(128 * m_star->getGraphic().x, 64 + (32 * m_star->getGraphic().y), 32, 32));
	spr->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	spr->setScale(2);
	this->addChild(spr);
	//spr->setZOrder(1);
	Animation *an = Animation::create();
	for (int k = 0; k < 4; k++)
	{
		SpriteFrame *Frame = SpriteFrame::create("art/galaxysprites.png", Rect((128 * m_star->getGraphic().x) + (32 * k), 64 + (32 * m_star->getGraphic().y), 32, 32));
		an->addSpriteFrame(Frame);
	}
	an->setDelayPerUnit(0.25F);
	spr->runAction(RepeatForever::create(Animate::create(an)));
	//build locations
	m_locations = new Sprite *[m_star->getNumLoc()];
	m_questionmarks = new Sprite *[m_star->getNumLoc()];
	for (int i = 0; i < m_star->getNumLoc(); i++)
	{
		if (m_star->getLocations()[i] != NULL)
		{
			if (m_star->getLocations()[i]->getType() == LT_WARPGATE)
			{
				m_locations[i] = Sprite::create("art/warpgate.png", cocos2d::Rect(0, 0, 32, 32));
				Animation *an = Animation::create();
				for (int k = 0; k < 4; k++)
				{
					SpriteFrame *Frame = SpriteFrame::create("art/warpgate.png", cocos2d::Rect(32*k, 0, 32, 32));
					an->addSpriteFrame(Frame);
				}
				an->setDelayPerUnit(0.5F);
				m_locations[i]->runAction(RepeatForever::create(Animate::create(an)));
			}
			else
			{
				m_locations[i] = Sprite::create("art/solarsprites.png", cocos2d::Rect(128 * m_star->getLocations()[i]->getWhatsprite(), 32 * getY(m_star->getLocations()[i]->getType()), 32, 32));
				Animation *an = Animation::create();
				for (int k = 0; k < 4; k++)
				{
					SpriteFrame *Frame = SpriteFrame::create("art/solarsprites.png", 
					cocos2d::Rect((128 * m_star->getLocations()[i]->getWhatsprite())+(32*k), 32 * getY(m_star->getLocations()[i]->getType()), 32, 32));
					an->addSpriteFrame(Frame);
				}
				an->setDelayPerUnit(0.5F);
				m_locations[i]->runAction(RepeatForever::create(Animate::create(an)));
			}
			m_locations[i]->setPosition(origin.x + (visibleSize.width / 2) + (32 * (m_star->getLocations()[i]->getPos().m_x-4)), origin.y + (visibleSize.height / 2)+(32 * (m_star->getLocations()[i]->getPos().m_y-4)));
			this->addChild(m_locations[i]);

			//check location for a mission
			if (m_star->getLocations()[i]->getMission() != NULL)
			{
				m_questionmarks[i] = Sprite::create("art/solarsprites.png", cocos2d::Rect(480,480,32,32));
				m_questionmarks[i]->setPosition(m_locations[i]->getPosition());
				this->addChild(m_questionmarks[i]);
			}
			else
			{
				m_questionmarks[i] = NULL;
			}
			//build questionmark
			if (m_star->getLocations()[i]->getColony() != NULL)
			{
				Sprite *sprite = Sprite::create("art/solarsprites.png", cocos2d::Rect(448, 480, 32, 32));
				sprite->setPosition(m_locations[i]->getPosition());
				this->addChild(sprite);
				Label *label = Label::createWithBMFont("art/font.fnt", *m_star->getLocations()[i]->getColony()->getName());
				label->setPosition(Vec2(0 + m_locations[i]->getPosition().x, -16 + m_locations[i]->getPosition().y));
				this->addChild(label);
			}
		}
		else
		{
			m_locations[i] = NULL;
			m_questionmarks[i] = NULL;
		}
	}

	if (CanColonize())
	{
		//build colony button
		m_button = ui::Button::create("art/starscapebuttonh0.png", "art/starscapebuttonh1.png", "art/bluebutton2.png");
		m_button->addClickEventListener(CC_CALLBACK_1(SolarView::ButtonCallback1, this));
		m_button->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2) - 240));
		this->addChild(m_button);
	}
}

void 	SolarView::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	//check if touch is outside window
	Touch *t = touches[0];
	if (m_frame->getBoundingBox().containsPoint(t->getLocation()))
	{
		for (int i = 0; i < m_star->getNumLoc(); i++)
		{
			if (m_locations[i] != NULL)
			{

				if (t->getLocation().getDistance(m_locations[i]->getPosition()) < 16)
				{
					if (m_star->getLocations()[i]->getType() == LT_WARPGATE)
					{
						//go to warpgate
						Scene *scene = ModeWarpgate::createScene(GameManager::getInstance()->getPlayer());
						Director::sharedDirector()->replaceScene(scene);
					}
					else
					{
						if (m_star->getLocations()[i]->getMission() != NULL)
						{
							if (m_star->getLocations()[i]->getMission()->getCurrentNode() == NULL)
							{
								ReferenceLibrary::getInstance()->getQuestEngine()->Convertstub(m_star->getLocations()[i]->getMission(), m_star->getLocations()[i]);
							}
							GameManager::getInstance()->getHandler()->setMission(m_star->getLocations()[i]->getMission());
							Scene *scene = GameManager::getInstance()->getHandler()->getScene(2);
							Director::sharedDirector()->replaceScene(scene);
						}
						else if (m_star->getLocations()[i]->getColony() != NULL)
						{
							if (m_star->getLocations()[i]->getMission() == NULL)
							{
								Scene *scene = ModeColony::createScene(GameManager::getInstance()->getPlayer(), m_star->getLocations()[i]->getColony());
								Director::sharedDirector()->replaceScene(scene);
							}
							else
							{
								GameManager::getInstance()->getHandler()->setMission(m_star->getLocations()[i]->getMission());
								Scene *scene = GameManager::getInstance()->getHandler()->getScene(2);
								Director::sharedDirector()->replaceScene(scene);
							}

						}

						if (m_colonize == true && (m_star->getLocations()[i]->getType() == LT_GARDEN || m_star->getLocations()[i]->getType() == LT_BARREN || m_star->getLocations()[i]->getType() == LT_ICE || m_star->getLocations()[i]->getType() == LT_HELL))
						{
							//build colony
							Colonize(m_star->getLocations()[i]);
						}
					}

					char b = 0;
				}
			}
		}
	}
	else
	{

		m_main->setVisible(true);
		m_main->setTouchEnabled(true);
		this->getParent()->removeChild(this);
	}
}

void SolarView::Colonize(Location *lt)
{
	lt->CreateColony(m_star->getName());
	GameManager::getInstance()->AddColony(lt);
	//expend colony license
	for (int i = 0; i < 3; i ++)
	{
		GameManager *manager = GameManager::getInstance();
		if (manager->getPlayer()->getItem(i) != NULL)
		{
			if (manager->getPlayer()->getItem(i)->m_type == ITYPE_LICENSE)
			{
				GameManager::getInstance()->getPlayer()->setItem(i, NULL);

				break;
			}
		}
	}
	GameManager::getInstance()->ColonyConnector(GameManager::getInstance()->getPlayer()->getPos0().y, GameManager::getInstance()->getPlayer()->getPos0().x, lt->getColony());
	Scene *scene = ModeColony::createScene(GameManager::getInstance()->getPlayer(), lt->getColony());
	Director::sharedDirector()->replaceScene(scene);
}

void 	SolarView::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{

}

void 	SolarView::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{

}

void SolarView::ButtonCallback0(Ref *pSender)
{

	m_main->setVisible(true);
	m_main->setTouchEnabled(true);
	this->getParent()->removeChild(this);
}

void SolarView::ButtonCallback1(Ref *pSender)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_colonize = true;
	//write text saying to select colony location
	Label *label = Label::createWithBMFont("art/font.fnt", "Select world to establish a colony on captain");
	label->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2) + 272);

	this->addChild(label);
}