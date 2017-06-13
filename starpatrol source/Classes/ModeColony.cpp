#include "ModeColony.h"
#include "Mode_Starscape.h"
#include "SolarView.h"
#include "RenamePanel.h"
#include "ReferenceLibrary.h"
#include "ModeShop.h"
#include "ModeRecruit.h"
#include "ModeQuestBoard.h"
#include "Prompt.h"

cocos2d::Scene* ModeColony::createScene(Player *playerfile, Colony *colony)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModeColony::create();
	layer->Setup(playerfile, colony);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;


}

ModeColony::~ModeColony()
{


}

bool ModeColony::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_subframe = NULL;
	m_subframeicons = NULL;
	m_subroot = NULL;
	m_numsubframeicons = 0;

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_active = true;

	ui::Button *buttons[4];
	buttons[0] = ui::Button::create("art/colonybuttona0.png", "art/colonybuttona1.png", "art/bluebutton2.png");
	buttons[0]->addClickEventListener(CC_CALLBACK_1(ModeColony::ButtonCallback0, this));

	buttons[1] = ui::Button::create("art/colonybuttonb0.png", "art/colonybuttonb1.png", "art/bluebutton2.png");
	buttons[1]->addClickEventListener(CC_CALLBACK_1(ModeColony::ButtonCallback1, this));

	buttons[2] = ui::Button::create("art/colonybuttonc0.png", "art/colonybuttonc1.png", "art/bluebutton2.png");
	buttons[2]->addClickEventListener(CC_CALLBACK_1(ModeColony::ButtonCallback2, this));

	buttons[3] = ui::Button::create("art/colonybuttond0.png", "art/colonybuttond1.png", "art/bluebutton2.png");
	buttons[3]->addClickEventListener(CC_CALLBACK_1(ModeColony::ButtonCallback3, this));
	for (int i = 0; i < 4; i++)
	{
		buttons[i]->setPosition(Vec2(origin.x + 32 + (64 * i), origin.y + 16));
		//	buttons[i]->setScale(2);
		buttons[i]->setZOrder(11);
		this->addChild(buttons[i]);
	}

	this->setTouchEnabled(true);

	return true;
}

void ModeColony::Setup(Player *playerfile, Colony *colony)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	m_player = playerfile;
	m_colony = colony;
	m_colony->RestartCheck();
	//build colony name
	//m_name = Label::createWithBMFont("art/font.fnt", m_colony->getName());
	//m_name->setPosition(Vec2(64, 16));
	//m_nameframe->addChild(m_name);
	//m_name->setAnchorPoint(Vec2(0.5F, 0.5F));
	
	m_nameframe = ui::Button::create("art/backbox2.png", "art/backbox2a.png", "art/bluebutton2.png");
	m_nameframe->addClickEventListener(CC_CALLBACK_1(ModeColony::ButtonCallback4, this));
	m_nameframe->setScaleX(2);
	this->addChild(m_nameframe);
	m_nameframe->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height - 32)));
	m_nameframe->setTitleText(*m_colony->getName());

	char buffer[4];
	//display colony resources left

	Sprite *spr = Sprite::create("art/black.png");
	spr->setScaleY(160);
	spr->setScaleX(16);
	spr->setPosition(origin.x + 64, origin.y + (visibleSize.height/2));
	spr->setZOrder(10);
	this->addChild(spr);
	for (int i = 0; i < 8; i++)
	{
		Sprite *icon = Sprite::create("art/colonyUI.png", Rect(32, 32 * i, 32, 32));
		icon->setPosition(origin.x + 32, origin.y + 600 - (i * 64));
		icon->setScale(2);
		icon->setZOrder(11);

		m_labels[i] = Label::createWithBMFont("art/font.fnt", "");
		m_labels[i]->setPosition(icon->getPositionX() + 64, icon->getPositionY());
		m_labels[i]->setZOrder(11);
		this->addChild(m_labels[i]);
		this->addChild(icon);
	}
		Sprite *icon = Sprite::create("art/colonyUI.png", Rect(0,128 , 32, 32));
		icon->setPosition(origin.x + 32, origin.y + 600 - (8 * 64));
		icon->setScale(2);
		icon->setZOrder(11);

		m_labels[8] = Label::createWithBMFont("art/font.fnt", "");
		m_labels[8]->setPosition(icon->getPositionX() + 64, icon->getPositionY());
		m_labels[8]->setZOrder(11);
		this->addChild(m_labels[8]);
		this->addChild(icon);
	Refresh();
	//display player resources right
	for (int i = 0; i < 4; i++)
	{
		Sprite *icon = Sprite::create("art/colonyUI.png", Rect(0, 32 * i, 32, 32));
		icon->setPosition(origin.x + 700, origin.y + 268- (i * 64));
		icon->setScale(2);
		icon->setZOrder(6);
		itoa(m_player->getShip()->getResource(i), &buffer[0], 10);
		std::string str = ""; str.append(buffer);
		m_labels[i+9] = Label::createWithBMFont("art/font.fnt", str);
		m_labels[i+9]->setPosition(icon->getPositionX() + 64, icon->getPositionY());
		m_labels[i + 9]->setZOrder(6);
		this->addChild(m_labels[i+9]);
		this->addChild(icon);
	}
	//display colony view center

	//drop the pad at Z order 2 center
	m_pad = Sprite::create("art/colony.png", Rect(256 + 64+ (64 * m_player->getShip()->getS()), 0, 64, 64));
	m_pad->setAnchorPoint(Vec2(0.5F, 0.25F));
	m_pad->setScale(2);
	m_pad->setZOrder(3);
	this->addChild(m_pad);
	m_pad->setPosition(Vec2(origin.x + (visibleSize.width / 2)+64, origin.y + (visibleSize.height / 2)+64));

	Vec2 v = m_pad->getPosition();
	v.x -= 256;
	v.y -= 0;
	Vec2 vex = Vec2(64, 32);
	Vec2 vey = Vec2(64, -32);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Sprite *sprite = NULL;
			if (i == 0 || i == 4 || j == 0 || j == 4)
			{
				sprite = Sprite::create("art/colony.png", Rect(m_colony->getType() * 64, 0, 64, 32));
			}
			else
			{
				sprite = Sprite::create("art/colony.png", Rect(m_colony->getType() * 64, 32, 64, 32));
			}
			sprite->setZOrder(0);
			sprite->setScale(2);
			sprite->setPosition(v + (vex*i) + (vey*j));
			this->addChild(sprite);
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (m_colony->getFacility(i) == NULL)
		{
			m_sprites[i] = NULL;
		}
		else
		{
			m_sprites[i]=Sprite::create("art/colony.png",Rect(64*m_colony->getFacility(i)->getGraphic().x,64*m_colony->getFacility(i)->getGraphic().y,64,64));
			m_sprites[i]->setScale(2);
			m_sprites[i]->setPosition(m_pad->getPosition() + Offset(i));
			this->addChild(m_sprites[i]);
			m_sprites[i]->setAnchorPoint(Vec2(0.5F, 0.25F));
			m_sprites[i]->setZOrder(i / 2);
			if (m_colony->getRunning(i) == false)
			{
				m_sprites[i]->setColor(Color3B(100, 100, 100));
			}
		}

	}
}

Vec2 ModeColony::Offset(int i)
{
	switch (i)
	{
	case 0:
		return Vec2(0, 64);
		break;

	case 1:
		return Vec2(64, 32);
		break;

	case 2:
		return Vec2(-64, 32);
		break;

	case 3:
		return Vec2(-128, 0);
		break;

	case 4:
		return Vec2(128, 0);
		break;

	case 5:
		return Vec2(-64, -32);
		break;

	case 6:
		return Vec2(64, -32);
		break;

	case 7:
		return Vec2(0, -64);
		break;
	}
	return Vec2(0, 32);
}

void ModeColony::ButtonCallback0(Ref *pSender)
{
	//button menu
}

void ModeColony::ButtonCallback1(Ref *pSender)
{
	if (m_active == true)
	{
		//button shop
		
		Scene *scene = ModeShop::createScene(m_player, m_colony->getDef());
			Director::sharedDirector()->replaceScene(scene);
	}

}

void ModeColony::ButtonCallback2(Ref *pSender)
{
	//button recruit
	if (m_active == true)
	{
		Scene *scene = ModeRecruit::createScene(m_player, m_colony->getDef());
		Director::sharedDirector()->replaceScene(scene);
	}

}

void ModeColony::ButtonCallback3(Ref *pSender)
{
	if (m_active == true)
	{

		//button close view
		Scene *scene = Mode_Starscape::createScene(true);

		//attach solarviewer
		Player *p = GameManager::getInstance()->getPlayer();

		//is this the star the player is currently orbiting?
		//later, zoom in on star contents when this happens


		Director::sharedDirector()->replaceScene(scene);
	
		//this->getParent()->addChild(view);	

	}
}

void ModeColony::ButtonCallback4(Ref *pSender)
{
	//trigger rename prompt
	if (m_active == true)
	{
		auto *layer = RenamePanel::create();
		layer->Setup(m_colony->getName(),"Colony Name",this);
		this->getParent()->addChild(layer);
		this->m_active = false;
		this->setVisible(false);
	}
}

void ModeColony::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_active == true)
	{
		if (m_subframe == NULL)
		{


			float m = touch->getLocation().getDistance(m_pad->getPosition());
			if (m < 256 && m>32)
			{
				float d = 999;
				int index = -1;
				for (int i = 0; i < 8; i++)
				{

					float c = touch->getLocation().getDistance(Offset(i)+m_pad->getPosition());
					if (c < 48)
					{
						if (c < d)
						{
							d = c;
							index = i;
						}
					}

				}
				if (index != -1)
				{

					m_target = index;
					//open up panel for this one
					if (m_colony->getFacility(index) == NULL)
					{
						BuildConstructFrame();
					}
					else
					{
						BuildInfoFrame();
					}

				}
			}
		}
		else
		{
			if (touch->getLocation().x + Director::getInstance()->getVisibleOrigin().x > 128)
			{
				if (m_colony->getFacility(m_target) == NULL)
				{
					for (int i = 0; i < m_numsubframeicons; i++)
					{
						if (m_subframeicons[i]->getBoundingBox().containsPoint(touch->getLocation() - m_subroot->getPosition()))
						{
							m_subframeicons[m_subtarget]->setTextureRect(Rect(0, 0, 128, 256));
							m_subtarget = i;
							WriteDescription();
							m_subframeicons[m_subtarget]->setTextureRect(Rect(128, 0, 128, 256));
							break;
						}
					}
				}
				else
				{

					for (int i = 0; i < m_numsubframeicons; i++)
					{

						if (m_subframeicons[i]->getBoundingBox().containsPoint(touch->getLocation() - m_subroot->getPosition()))
						{
							m_subframeicons[m_subtarget + 1]->setTextureRect(Rect(0, 0, 128, 128));
							m_subtarget = i - 1;
							m_subframeicons[m_subtarget + 1]->setTextureRect(Rect(128, 0, 128, 128));
							if (m_subtarget >= 0)
							{
								if (m_facilities[m_subtarget] != NULL)
								{
									if (m_subtarget>0)
									{
										WriteDescription(true);
									}
									else
									{
										WriteDescription();
									}
								}
							}
						}
					}
				}
			}
		
		}
	
	}
	
}

void ModeColony::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	//check if touch within X of a tile slot
	if (m_active == true && m_subframe!=NULL)
	{
		//if target facility slot empty, then react to movement left or right within the build selection window by scrolling either direction
		if (m_colony->getFacility(m_target) == NULL)
		{
			if (touch->getDelta().x < 0)
			{
				if (m_subroot->getPositionX()>(Director::getInstance()->getVisibleOrigin().x+528)-(m_numsubframeicons*128))
				{
					int x = m_subroot->getPositionX() + touch->getDelta().x;
					m_subroot->setPositionX(x);
				}
			}
			else if (m_subroot->getPosition().x<m_pad->getPositionX()-128)
			{
				int x = m_subroot->getPositionX() + touch->getDelta().x;
				m_subroot->setPositionX(x);
			}
		}
	}
}

void ModeColony::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];

}

void ModeColony::ReturntoLayer()
{
	//redraw things
	m_nameframe->setTitleText(*m_colony->getName());
	this->setTouchEnabled(true);
	m_active = true;
}

void ModeColony::Refresh()
{
	for (int i = 0; i < 8; i++)
	{
		std::string str = "";
		char buffer[4];
		itoa(m_colony->getUsedRes(i), &buffer[0], 10);
		str.append(buffer); str.append("/");
		itoa(m_colony->getResMax(i), &buffer[0], 10);
		str.append(buffer);
		if (m_colony->getResFromTrade(i)+m_colony->getTradeResUsed(i)>0)
		{
			itoa(m_colony->getResFromTrade(i)+m_colony->getTradeResUsed(i), &buffer[0], 10);
			str.append("+"); str.append(buffer);
		}
		m_labels[i]->setString(str);
	}
	std::string str = "";
	char buffer[4];
	itoa(m_colony->getDefence(0), &buffer[0], 10);
	str.append(buffer);
	int v = m_colony->getDefenceBonus();
	if (v > 0)
	{
		itoa(v, &buffer[0], 10);
		str.append("+"); str.append(buffer);
	}
	m_labels[8]->setString(str);
}

void ModeColony::AmendServices(Facility *oldfac, Facility *newfac)
{
	//if old fac empty
	if (oldfac == NULL && newfac!=NULL)
	{
		//add all services
		m_colony->BuildFacility(newfac, m_target);
	}

	
	//if old fac something and newfac empty
	if (oldfac != NULL && newfac == NULL)
	{
		//remove all provided services

		//refund resource expenditures
		m_colony->DestroyFacility(m_target);
	}
	if (oldfac != NULL && newfac != NULL)
	{
		//if old fac something and new fac something
		m_colony->UpgradeFacility(newfac, m_target);
		//measure differences
	}

}

void ModeColony::BuildInfoFrame()
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_subtarget = 0;

	//build frame
	m_subframe = Sprite::create("art/frame.png");
	m_subframe->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2) - 128);
	m_subframe->setScaleX(4);
	m_subframe->setScaleY(2);
	m_subframe->setZOrder(7);
	this->addChild(m_subframe);

	for (int i = 0; i < 8; i++)
	{
		Sprite *spr = Sprite::create("art/colonyUI.png", Rect(32, 32 * i, 32, 32));
		spr->setPosition(8 + (32 * (i % 4)), 32 + (16 * (i / 4)));
		spr->setScaleX(0.25F); spr->setScaleY(0.5F);
		m_subframe->addChild(spr);
		m_numbers[i] = Label::createWithBMFont("art/font.fnt", "00");
		spr->addChild(m_numbers[i]);
		m_numbers[i]->setPosition(64, 16);
	}

	//build description panel
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_subframe->addChild(m_description);
	m_description->setPosition(Vec2(64, 120));
	m_description->setWidth(128);
	m_description->setScaleX(0.25F);
	m_description->setScaleY(0.5F);
	m_description->setAnchorPoint(Vec2(0.5F, 1));
	//build 3 frames above
	m_numsubframeicons = 3;
	m_subframeicons = new Sprite *[3];

	m_subroot = Node::create();
	m_subroot->setPosition(m_subframe->getPosition());
	m_subroot->setZOrder(7);
	this->addChild(m_subroot);
	
	for (int i = 0; i < 3; i++)
	{
		m_subframeicons[i] = Sprite::create("art/constructionframe.png", Rect(0, 0, 128, 128));
		m_subframeicons[i]->setPosition(-128+(128*i),192);

		m_subroot->addChild(m_subframeicons[i]);
	}

	m_facilities = new Facility *[2];
	m_facilities[0] = m_colony->getFacility(m_target);
	m_facilities[1] = ReferenceLibrary::getInstance()->getUpgrade(m_facilities[0]);

	//build destruction icon
	Label *label = Label::createWithBMFont("art/font.fnt", "Demolish");
	label->setPosition(64, 64);
	m_subframeicons[0]->addChild(label);

	//build building icon
	Sprite *spr = Sprite::create("art/colony.png", Rect(m_facilities[0]->getGraphic().x * 64, m_facilities[0]->getGraphic().y * 64, 64, 64));
	spr->setScale(2);
	spr->setPosition(64, 64);
	m_subframeicons[1]->addChild(spr);
	//build upgrade icon
	if (m_facilities[1] != NULL)
	{
		spr = Sprite::create("art/colony.png", Rect(m_facilities[1]->getGraphic().x * 64, m_facilities[1]->getGraphic().y * 64, 64, 64));
		spr->setScale(2);
		spr->setPosition(64, 64);
		m_subframeicons[2]->addChild(spr);
	}
	else
	{
		label = Label::createWithBMFont("art/font.fnt", "MAX");
		m_subframeicons[2]->addChild(label);
		label->setPosition(64, 64);
	}
	m_subframeicons[m_subtarget+1]->setTextureRect(Rect(128, 0, 128, 128));
	WriteDescription();

	//build buttons
	//build the 'demolish' button
	ui::Button *button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeColony::InfoWindowCallback0, this));
	button->setPosition(Vec2(16, 8));
	button->setTitleText("Demolish");

	button->setScale(0.25F);
	button->setTitleFontSize(32);
	m_subframe->addChild(button);

	//build the 'cancel' button
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeColony::SubwindowClose, this));
	button->setPosition(Vec2(64, 8));
	button->setTitleText("Cancel");

	button->setScale(0.25F);
	button->setTitleFontSize(32);
	m_subframe->addChild(button);
	//build the 'upgrade' button
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeColony::InfoWindowCallback1, this));
	button->setPosition(Vec2(112, 8));
	button->setTitleText("Upgrade");
	button->setTitleFontSize(32);
	button->setScale(0.25F);
	m_subframe->addChild(button);

}

void ModeColony::BuildConstructFrame()
{

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_subtarget = 0;
	//build frame
	m_subframe=Sprite::create("art/frame.png");
	m_subframe->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)-128);
	m_subframe->setScaleX(4);
	m_subframe->setScaleY(2);
	m_subframe->setZOrder(7);
	this->addChild(m_subframe);
	//build subroot
	m_subroot = Node::create();
	m_subroot->setZOrder(7);
	m_subroot->setPosition(origin+Vec2(128,0));
	this->addChild(m_subroot);
	//build structure icons
	m_facilities = ReferenceLibrary::getInstance()->getBaseFacilitiesList();
	m_numsubframeicons = ReferenceLibrary::getInstance()->getBaseFacilitiesCount();
	m_subframeicons = new Sprite *[m_numsubframeicons];
	for (int i = 0; i < m_numsubframeicons; i++)
	{
		m_subframeicons[i] = Sprite::create("art/constructionframe.png",Rect(0,0,128,256));
		m_subframeicons[i]->setPositionY((visibleSize.height / 2)+128);
		m_subframeicons[i]->setPositionX(64+(i * 128));

		m_subroot->addChild(m_subframeicons[i]);

		//add images to sub frames
		Sprite *spr = Sprite::create("art/colony.png", Rect(m_facilities[i]->getGraphic().x * 64, m_facilities[i]->getGraphic().y * 64, 64, 64));
		spr->setScale(2);
		spr->setPosition(64, 192);
		m_subframeicons[i]->addChild(spr);
		//add price sprite grid
		for (int j = 0; j < 4; j++)
		{
			spr = Sprite::create("art/colonyUI.png", Rect(0, j * 32, 32, 32));
			spr->setPosition(24+(56 * (j / 2)),32+( 64 * (j % 2)));
			m_subframeicons[i]->addChild(spr);
			char buffer[3];
			itoa(m_facilities[i]->getTokenCost(j), &buffer[0], 10);
			Label *label = Label::createWithBMFont("art/font.fnt", buffer);
			spr->addChild(label);
			label->setPosition(48, 16);
			if (m_player->getShip()->getResource(j) < m_facilities[i]->getTokenCost(j))
			{
				label->setColor(Color3B(255, 0, 0));
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		Sprite *spr = Sprite::create("art/colonyUI.png",Rect(32,32*i,32,32));
		spr->setPosition(8 + (32 * (i%4)), 32+(16*(i/4)));
		spr->setScaleX(0.25F); spr->setScaleY(0.5F);
		m_subframe->addChild(spr);
		m_numbers[i] = Label::createWithBMFont("art/font.fnt", "00");
		spr->addChild(m_numbers[i]);
		m_numbers[i]->setPosition(64, 16);
	}

	//parent prices onto structure icons

	//build description panel
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_subframe->addChild(m_description);
	m_description->setPosition(Vec2(64, 120));
	m_description->setWidth(128);
	m_description->setAnchorPoint(Vec2(0.5F, 1));
	m_description->setScaleX(0.20F);
	m_description->setScaleY(0.4F);
	
	//build the 'build' button
	ui::Button *button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeColony::BuildWindowCallback0, this));
	button->setPosition(Vec2(112, 8));
	button->setTitleText("Build");

	button->setScale(0.25F);
	button->setTitleFontSize(32);
	m_subframe->addChild(button);
	//build the 'cancel' button
	button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeColony::SubwindowClose, this));
	button->setPosition(Vec2(16, 8));
	button->setTitleText("Cancel");
	button->setTitleFontSize(32);
	button->setScale(0.25F);
	m_subframe->addChild(button);
	m_subframeicons[m_subtarget]->setTextureRect(Rect(128, 0, 128, 256));
	WriteDescription();
}

void ModeColony::BuildWindowCallback0(Ref *pSender)
{
	if (m_active == true)
	{

		//check if we can build it
		if (CanBuild())
		{
			//build facility
			m_colony->BuildFacility(m_facilities[m_subtarget], m_target);
			//subtract player resources
			for (int i = 0; i < 4; i++)
			{
				m_player->getShip()->ModResource(i, m_facilities[m_subtarget]->getTokenCost(i)*-1);
			}
			//update numbers onscreen
			for (int i = 0; i < 4; i++)
			{
				char buffer[3];
				itoa(m_player->getShip()->getResource(i), &buffer[0], 10);
				m_labels[i + 9]->setString(buffer);
			}
			Refresh();
			//create sprite for building
			m_sprites[m_target] = Sprite::create("art/colony.png", Rect(64 * m_colony->getFacility(m_target)->getGraphic().x, 64 * m_colony->getFacility(m_target)->getGraphic().y, 64, 64));
			m_sprites[m_target]->setScale(2);
			m_sprites[m_target]->setPosition(m_pad->getPosition() + Offset(m_target));
			this->addChild(m_sprites[m_target]);
			m_sprites[m_target]->setAnchorPoint(Vec2(0.5F, 0.25F));
			m_sprites[m_target]->setZOrder(m_target / 2);

			//create 'under construction' framework sprite on location

			Sprite *conyard = Sprite::create("art/colony.png", Rect(256, 0, 64, 64));
			conyard->setPosition(m_sprites[m_target]->getPosition());
			conyard->setZOrder(5);
			this->addChild(conyard);
			//cross fade with the animation system
			Vector <FiniteTimeAction *> action;
			action.pushBack(FadeOut::create(2));
			action.pushBack(RemoveSelf::create());
			Sequence *seq = Sequence::create(action);
			conyard->runAction(seq);
			conyard->setAnchorPoint(Vec2(0.5F, 0.25F));
			m_sprites[m_target]->runAction(FadeIn::create(2));
			SubwindowClose(pSender);
			RefreshIcons();
		}
		else
		{
			//prompt
			std::string str = "You cannot build that as you lack the required \n";
			for (int i = 0; i < 4; i++)
			{
				if (m_facilities[m_subtarget]->getTokenCost(i)>m_player->getShip()->getResource(i))
				{
					char buffer[3];
					itoa(m_facilities[m_subtarget]->getTokenCost(i), &buffer[0], 10);
					std::string add = buffer;
					switch (i)
					{
					case 0:
						add.append(" red");
						break;
					case 1:
						add.append(" green");
						break;
					case 2:
						add.append(" blue");
						break;
					case 3:
						add.append(" purple");
						break;
					}
					str.append(add);
					str.append(" tokens,");
				}
			}
			for (int i = 0; i < 8; i++)
			{
				if (m_facilities[m_subtarget]->getCost(i)>m_colony->getResFree(i))
				{
					char buffer[3];
					itoa(m_facilities[m_subtarget]->getCost(i), &buffer[0], 10);
					std::string add = buffer;
					switch (i)
					{
					case 0:
						add.append(" workers");
						break;
					case 1:
						add.append(" ore");
						break;
					case 2:
						add.append(" radioactives");
						break;
					case 3:
						add.append(" food");
						break;
					case 4:
						add.append(" industry");
						break;
					case 5:
						add.append(" science");
						break;
					case 6:
						add.append(" megapower");
						break;
					case 7:
						add.append(" supertech");
						break;
					}
					str.append(add);
					str.append(",");
				}
			}
			Prompt *prompt = Prompt::create();
			prompt->Setup(this, str);
			m_active = false;
			setTouchEnabled(false);
			prompt->setTouchEnabled(true);
			this->getParent()->addChild(prompt);

		}
	}
}

bool ModeColony::CanBuild()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_facilities[m_subtarget]->getTokenCost(i)>m_player->getShip()->getResource(i))
		{
			return false;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (m_facilities[m_subtarget]->getCost(i)>m_colony->getAvailableRes(i))
		{
			return false;
		}
	}

	return true;
}

bool ModeColony::CanUpgrade()
{
	if (m_facilities[1] != NULL)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_facilities[1]->getTokenCost(i)>m_player->getShip()->getResource(i))
			{
				return false;
			}
		}
		for (int i = 0; i < 8; i++)
		{
			int v = m_facilities[1]->getCost(i) - m_facilities[0]->getCost(i);
			int c = m_colony->getResFree(i);
			if (v>c)
			{
				return false;
			}
		}
		return true;
	}
	return false;

}

void ModeColony::SubwindowClose(Ref *pSender)
{
	if (m_active == true)
	{
		this->removeChild(m_subframe);
		m_subframe = NULL;
		if (m_subroot != NULL)
		{
			this->removeChild(m_subroot);
			m_subroot = NULL;
		}
		if (m_subframeicons != NULL)
		{
			delete[]m_subframeicons;
			m_subframeicons = NULL;
		}

		if (m_facilities != NULL)
		{
			delete[] m_facilities;
			m_facilities = NULL;
		}

	}

}

void ModeColony::WriteDescription(bool upgrade)
{
	for (int i = 0; i < 8; i++)
	{
		char buffer[3];

		itoa(m_facilities[m_subtarget]->getCost(i), &buffer[0], 10);
		m_numbers[i]->setVisible(true);
		m_numbers[i]->setString(buffer);
		if (upgrade == false)
		{
			if (m_facilities[m_subtarget]->getCost(i)>m_colony->getAvailableRes(i))
			{
				m_numbers[i]->setColor(Color3B(255, 0, 0));
			}
			else
			{
				m_numbers[i]->setColor(Color3B(255, 255, 255));
			}
		}
		else
		{
			if (m_facilities[m_subtarget]->getCost(i) - m_facilities[m_subtarget - 1]->getCost(i)>m_colony->getAvailableRes(i))
			{
				m_numbers[i]->setColor(Color3B(255, 0, 0));
			}
			else
			{
				m_numbers[i]->setColor(Color3B(255, 255, 255));
			}
		}

	}

	m_description->setString(m_facilities[m_subtarget]->getName()+"\n"+m_facilities[m_subtarget]->getDescription());
}

void ModeColony::InfoWindowCallback0(Ref *pSender)
{
	if (m_active == true)
	{

		//demolish
		m_colony->DestroyFacility(m_target);
	
		//create sprite for building

		//create 'under construction' framework sprite on location

		Sprite *conyard = Sprite::create("art/colony.png", Rect(256, 0, 64, 64));
		conyard->setPosition(m_sprites[m_target]->getPosition());
		conyard->setZOrder(5);
		this->addChild(conyard);
		//cross fade with the animation system
		Vector <FiniteTimeAction *> action;
		action.pushBack(FadeOut::create(2));
		action.pushBack(RemoveSelf::create());
		Sequence *seq = Sequence::create(action);
		conyard->runAction(seq);
		conyard->setAnchorPoint(Vec2(0.5F, 0.25F));
		this->removeChild(m_sprites[m_target]);
		m_sprites[m_target] = NULL;
		SubwindowClose(pSender);

		if (m_colony->SanityCheck())
		{
			//update icons
			RefreshIcons();
		}
		//update numbers onscreen
		for (int i = 0; i < 4; i++)
		{
			char buffer[3];
			itoa(m_player->getShip()->getResource(i), &buffer[0], 10);
			m_labels[i + 9]->setString(buffer);
		}
		Refresh();
	}
}

void ModeColony::RefreshIcons()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_sprites[i] != NULL)
		{
			if (m_colony->getRunning(i) == false)
			{
				m_sprites[i]->setColor(Color3B(100, 100, 100));
			}
			else
			{
				m_sprites[i]->setColor(Color3B(255, 255, 255));
			}
		}

	}
}

void ModeColony::InfoWindowCallback1(Ref *pSender)
{
	//upgrade
	if (m_active == true)
	{


		if (CanUpgrade())
		{
			m_colony->UpgradeFacility(m_facilities[1], m_target);
			//subtract player resources
			for (int i = 0; i < 4; i++)
			{
				m_player->getShip()->ModResource(i, m_facilities[m_subtarget]->getTokenCost(i)*-1);
			}
			//update numbers onscreen
			for (int i = 0; i < 4; i++)
			{
				char buffer[3];
				itoa(m_player->getShip()->getResource(i), &buffer[0], 10);
				m_labels[i + 9]->setString(buffer);
			}
			Refresh();
			//create sprite for building
			this->removeChild(m_sprites[m_target]);
			m_sprites[m_target] = Sprite::create("art/colony.png", Rect(64 * m_colony->getFacility(m_target)->getGraphic().x, 64 * m_colony->getFacility(m_target)->getGraphic().y, 64, 64));
			m_sprites[m_target]->setScale(2);
			m_sprites[m_target]->setPosition(m_pad->getPosition() + Offset(m_target));
			this->addChild(m_sprites[m_target]);
			m_sprites[m_target]->setAnchorPoint(Vec2(0.5F, 0.25F));
			m_sprites[m_target]->setZOrder(m_target / 2);

			//create 'under construction' framework sprite on location

			Sprite *conyard = Sprite::create("art/colony.png", Rect(256, 0, 64, 64));
			conyard->setPosition(m_sprites[m_target]->getPosition());
			conyard->setZOrder(5);
			this->addChild(conyard);
			//cross fade with the animation system
			Vector <FiniteTimeAction *> action;
			action.pushBack(FadeOut::create(2));
			action.pushBack(RemoveSelf::create());
			Sequence *seq = Sequence::create(action);
			conyard->runAction(seq);
			conyard->setAnchorPoint(Vec2(0.5F, 0.25F));
			m_sprites[m_target]->runAction(FadeIn::create(2));
			SubwindowClose(pSender);
		}
		else
		{
			//prompt
			//prompt
			std::string str = "You cannot build that as you lack the required \n";
			for (int i = 0; i < 4; i++)
			{
				if (m_facilities[m_subtarget]->getTokenCost(i)>m_player->getShip()->getResource(i))
				{
					char buffer[3];
					itoa(m_facilities[1]->getTokenCost(i), &buffer[0], 10);
					std::string add = buffer;
					switch (i)
					{
					case 0:
						add.append(" red");
						break;
					case 1:
						add.append(" green");
						break;
					case 2:
						add.append(" blue");
						break;
					case 3:
						add.append(" purple");
						break;
					}
					str.append(add);
					str.append(" tokens,");
				}
			}
			for (int i = 0; i < 8; i++)
			{
				if (m_facilities[m_subtarget]->getCost(i)>m_colony->getResFree(i))
				{
					char buffer[3];
					itoa(m_facilities[1]->getCost(i)-m_facilities[0]->getCost(i), &buffer[0], 10);
					std::string add = buffer;
					switch (i)
					{
					case 0:
						add.append(" workers");
						break;
					case 1:
						add.append(" ore");
						break;
					case 2:
						add.append(" radioactives");
						break;
					case 3:
						add.append(" food");
						break;
					case 4:
						add.append(" industry");
						break;
					case 5:
						add.append(" science");
						break;
					case 6:
						add.append(" megapower");
						break;
					case 7:
						add.append(" supertech");
						break;
					}
					str.append(add);
					str.append(",");
				}
			}
			Prompt *prompt = Prompt::create();
			prompt->Setup(this, str);
			m_active = false;
			setTouchEnabled(false);
			this->getParent()->addChild(prompt);
		}
	}
}

void ModeColony::InfoWindowCallback2(Ref *pSender)
{

}