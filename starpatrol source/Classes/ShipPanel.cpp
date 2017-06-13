#include "ShipPanel.h"
#include "Module_commodity.h"
#include "Mode_Starscape.h"
#include "CrewPanel.h"
#include "LogbookPanel.h"
#include "ModeMenu.h"

ShipPanel::~ShipPanel()
{

}

cocos2d::Scene* ShipPanel::createScene(Player *playerfile)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ShipPanel::create();
	layer->Setup(playerfile);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;


}


bool ShipPanel::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Sprite *m_descriptionframe;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->setTouchEnabled(true);
	Sprite *spr3 = Sprite::create("art/grid.png");
	spr3->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	spr3->setScale(3.125F);
	this->addChild(spr3);
	Sprite *spr2 = Sprite::create("art/patternedui.png");
//	spr2->setScale(1.60F);
	spr2->setScaleX(1.55F);
	spr2->setPosition(origin.x + 100, origin.y + 64);
	this->addChild(spr2);
	spr2 = Sprite::create("art/patternedui.png");
//	spr2->setScale(1.60F);
	spr2->setScaleX(1.55F);
	spr2->setPosition(origin.x + 700, origin.y + 64);
	this->addChild(spr2);

	cocos2d::ui::Button *buttons[4];
	//build menu button
	buttons[0] = ui::Button::create("art/starscapebuttona0.png", "art/starscapebuttona1.png", "art/bluebutton2.png");
	buttons[0]->addClickEventListener(CC_CALLBACK_1(ShipPanel::ButtonCallback0, this));
	buttons[0]->setPosition(Vec2(cocos2d::Vec2(origin.x + 16, origin.y + 110)));
	buttons[0]->setScale(1);
	//build ship button
	buttons[1] = ui::Button::create("art/starscapebuttone0.png", "art/starscapebuttone1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(ShipPanel::ButtonCallback1, this));
	buttons[1]->setPosition(Vec2(cocos2d::Vec2(origin.x + 48, origin.y + 110)));
	buttons[1]->setScale(1);
	//build officers button
	buttons[2] = ui::Button::create("art/starscapebuttonc0.png", "art/starscapebuttonc1.png", "art/bluebutton2.png");
	buttons[2]->addClickEventListener(CC_CALLBACK_1(ShipPanel::ButtonCallback2, this));
	buttons[2]->setPosition(Vec2(cocos2d::Vec2(origin.x + 80, origin.y + 110)));
	buttons[2]->setScale(1);

	//build log button
	buttons[3] = ui::Button::create("art/starscapebuttond0.png", "art/starscapebuttond1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(ShipPanel::ButtonCallback3, this));
	buttons[3]->setPosition(Vec2(cocos2d::Vec2(origin.x + 112, origin.y + 110)));
	buttons[3]->setScale(1);
	for (int i = 0; i < 4; i++)
	{
		this->addChild(buttons[i]);
	}

	m_descriptionframe = Sprite::create("art/frame.png");
	m_descriptionframe->setScaleY(1);
	m_descriptionframe->setScaleX(3.2F);
	m_descriptionframe->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + 64));
	this->addChild(m_descriptionframe);
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_description->setAlignment(TextHAlignment::LEFT);
	m_description->setPosition(m_descriptionframe->getPosition());
	m_description->setSystemFontSize(24);
	m_description->setWidth(380);

	this->addChild(m_description);
	return true;
}

void ShipPanel::Setup(Player *playerfile)
{
	m_player = playerfile;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_outline.Setup(playerfile->getShip(), this, playerfile);

	//build name
	
	Label *label = Label::createWithBMFont("art/font.fnt", m_player->getShip()->getName());
	label->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height*0.95F)));
	this->addChild(label);
	//build powerbars
	Sprite *spr[2];
	m_bars[0] = Segbar::create();
	m_bars[0]->Setup(0, 128, m_player->getShip()->getHPmax(), m_player->getShip()->getHP());
	
	m_bars[1] = Segbar::create();
	m_bars[1]->Setup(2, 128, m_player->getShip()->getMaxFuel(), m_player->getShip()->getFuel());

	Label *labels[2];
	char b[6];
	itoa(m_player->getShip()->getHP(), &b[0], 10);

	labels[0] = Label::createWithBMFont("art/font.fnt", b);
	itoa(m_player->getShip()->getFuel(), &b[0], 10);
	labels[1] = Label::createWithBMFont("art/font.fnt", b);
	for (int i = 0; i < 2; i++)
	{
		spr[i] = Sprite::create("art/shipUI.png", Rect(128, 160+(32*i), 32, 32));
		spr[i]->setPosition(origin.x + 16, origin.y + 30 + (40 * i));
		labels[i]->setAnchorPoint(Vec2(0, 0.5F));
	
		labels[i]->setPosition(origin.x + 162, origin.y + 30 + (40 * i));
		Sprite *sprb = Sprite::create("art/backbox.png");
		sprb->setAnchorPoint(Vec2(-0.1F, 0.5F));
		sprb->setScaleX(0.5F);
		sprb->setPosition(labels[i]->getPosition());
		this->addChild(sprb);
		m_bars[i]->setPosition(origin.x + 32, origin.y + 30+(40*i));
		m_bars[i]->setScale(2);
		sprb = Sprite::create("art/backbox.png");
		sprb->setAnchorPoint(Vec2(-0.1F, 0.5F));
		sprb->setScaleX(2.0F);
		sprb->setPosition(m_bars[i]->getPosition());
		this->addChild(sprb);
		this->addChild(m_bars[i]);
		this->addChild(spr[i]);
		this->addChild(labels[i]);
		m_bars[i]->setScaleX(1.6F);
	}
	Sprite *spr0[4];
	Sprite *spr1[4];

	for (int i = 0; i < 4; i++)
	{
		spr0[i] = Sprite::create("art/shipUI.png", Rect(160, 128 + (32 * i), 32, 32));
		spr1[i] = Sprite::create("art/shipUI.png", Rect(192, 128 + (32 * i), 32, 32));

		spr0[i]->setPosition(origin.x + 616, origin.y + 16 + (32 * i));
		spr1[i]->setPosition(origin.x + 716, origin.y + 16 + (32 * i));
		char b[6];
		if (i < 3)
		{	
			_itoa(m_player->getShip()->getOffence(i)*100, &b[0], 10);
			Label *label = Label::createWithBMFont("art/font.fnt", b);
			label->setAnchorPoint(Vec2(0, 0.5F));
			label->setPosition(origin.x + 636, origin.y + 16 + (32 * i));
			Sprite *sprb = Sprite::create("art/backbox.png");
			sprb->setAnchorPoint(Vec2(0, 0.5F));
			sprb->setPosition(label->getPosition() + Vec2(-4, 0));
			this->addChild(sprb);
			this->addChild(label);
			itoa(m_player->getShip()->getDefence(i), &b[0], 10);
			label = Label::createWithBMFont("art/font.fnt", b);
			label->setAnchorPoint(Vec2(0, 0.5F));
			label->setPosition(origin.x + 736, origin.y + 16 + (32 * i));
			sprb = Sprite::create("art/backbox.png");
			sprb->setAnchorPoint(Vec2(0, 0.5F));
			sprb->setPosition(label->getPosition()+Vec2(-4,0));
			this->addChild(sprb);
			this->addChild(label);
		}
		else
		{
			itoa(m_player->getShip()->getSpeed() , &b[0], 10);
			Label *label = Label::createWithBMFont("art/font.fnt", b);
			label->setPosition(origin.x + 636, origin.y + 16 + (32 * i));
			label->setAnchorPoint(Vec2(0, 0.5F));
			Sprite *sprb = Sprite::create("art/backbox.png");
			sprb->setAnchorPoint(Vec2(0, 0.5F));
			sprb->setPosition(label->getPosition() + Vec2(-4, 0));
			this->addChild(sprb);
			this->addChild(label);
			itoa(m_player->getMoney(), &b[0], 10);
			label = Label::createWithBMFont("art/font.fnt", b);
			label->setSystemFontSize(16);
			label->setAnchorPoint(Vec2(0, 0.5F));
			label->setPosition(origin.x + 736, origin.y + 16 + (32 * i));
			sprb = Sprite::create("art/backbox.png");
			sprb->setAnchorPoint(Vec2(0, 0.5F));
			sprb->setPosition(label->getPosition() + Vec2(-4, 0));
			this->addChild(sprb);
			this->addChild(label);
		}
		this->addChild(spr0[i]);
		this->addChild(spr1[i]);
	}


}

void ShipPanel::ButtonCallback0(Ref *pSender)
{
	//menu
	Scene *scene = ModeMenu::createScene(1);
	Director::sharedDirector()->replaceScene(scene);
}

void ShipPanel::ButtonCallback1(Ref *pSender)
{
	Scene *scene = Mode_Starscape::createScene();
	Director::getInstance()->replaceScene(scene);
	//back to starscape
}

void ShipPanel::ButtonCallback2(Ref *pSender)
{
	//officers
	Scene *scene = CrewPanel::createScene(m_player);
	Director::getInstance()->replaceScene(scene);
}

void ShipPanel::ButtonCallback3(Ref *pSender)
{
	//logbook
	//initiate log panel
	Scene *scene = LogbookPanel::createScene();
	Director::getInstance()->replaceScene(scene);
}

void ShipPanel::ButtonCallback4(Ref *pSender)
{

}

void 	ShipPanel::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	//grab whatever sprite's under the touch
	Touch *touch = touches[0];
	for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
	{
		if (m_outline.m_modulesprites[i] != NULL)
		{
			if (m_outline.m_modulesprites[i]->getBoundingBox().containsPoint(touch->getLocation()))
			{
				m_description->setString(m_player->getShip()->getModule(i)->getDesc());
				m_outline.m_heldsprite = i;
				break;
			}
		}
	}
	if (m_outline.m_heldsprite == -1)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_outline.m_items[i] != NULL)
			{
				if (m_outline.m_items[i]->getBoundingBox().containsPoint(touch->getLocation()))
				{
					m_outline.m_heldsprite = 20 + i;
					m_description->setString(m_player->getItem(i)->m_description);
				}
			}
		}
	}

}

void 	ShipPanel::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	//move sprite if sprite is held
	Touch *touch = touches[0];
	if (m_outline.m_heldsprite != -1)
	{

		if (m_outline.m_heldsprite < 20)
		{
			m_outline.m_modulesprites[m_outline.m_heldsprite]->setPosition(touch->getLocation());
		}
		else
		{
			m_outline.m_items[m_outline.m_heldsprite - 20]->setPosition(touch->getLocation());
		}

	}
}

void 	ShipPanel::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	//let go, figure out which slot we're over, if its empty move the sprite there
	if (m_outline.m_heldsprite != -1)
	{
		if (m_outline.m_bin->getBoundingBox().containsPoint(touch->getLocation()))
		{
			//bin the thing
			m_outline.BinThing(this);
		}
		else
		{
			if (m_outline.m_heldsprite < 20)
			{
				if (m_outline.MoveModule(touch->getLocation()))
				{
					m_bars[1]->setVariable(m_player->getShip()->getFuel());
				}
			}
			else
			{
				m_outline.ResetPosition();
			}


		}
	}
	m_outline.m_heldsprite = -1;
}

