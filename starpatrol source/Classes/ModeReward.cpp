#include "ModeReward.h"
#include "GameManager.h"
#include "ReferenceLibrary.h"
#include "ui\UIButton.h"
cocos2d::Scene* ModeReward::createScene(Player *playerfile, MissionNode *stage)
{
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModeReward::create();

	layer->Setup(playerfile, stage);
	// add layer as a child to scene
	scene->addChild(layer,0,100);

	// return the scene
	return scene;
}

ModeReward::~ModeReward()
{

	delete[]m_officers;
	delete[]m_officerslots;
}

bool ModeReward::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->setTouchEnabled(true);
	m_heldsprite = -1;
	Sprite *spr3 = Sprite::create("art/grid.png");
	spr3->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));
	spr3->setScale(3.125F);
	this->addChild(spr3);

	m_descriptionframe = Sprite::create("art/frame.png");
	m_descriptionframe->setScaleY(1);
	m_descriptionframe->setScaleX(3.2F);
	m_descriptionframe->setPosition(Vec2(origin.x + visibleSize.width - 204, origin.y + 64));
	this->addChild(m_descriptionframe);
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_description->setAlignment(TextHAlignment::LEFT);
	m_description->setPosition(m_descriptionframe->getPosition());
	m_description->setSystemFontSize(24);
	m_description->setWidth(380);
	this->addChild(m_description);
	Sprite *spr2 = Sprite::create("art/patternedui.png");
	//	spr2->setScale(1.60F);
	spr2->setScaleX(2.00F);
	spr2->setPosition(origin.x + visibleSize.width - 536, origin.y + 64);
	this->addChild(spr2);

	//add mission complete message
	Label *label = Label::createWithBMFont("art/font.fnt", "MISSION COMPLETE!");
	label->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height) - 32);
	this->addChild(label);

	ui::Button *button = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeReward::ButtonCallback0, this));
	button->setTitleText("Proceed");
	button->setPosition(Vec2(0, -32) + spr2->getPosition());
	this->addChild(button);
	return true;
}

void ModeReward::Setup(Player *playerfile, MissionNode *stage)
{
	m_player = playerfile;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_stage = stage;
	m_player = playerfile;
	m_outline.Setup(playerfile->getShip(), this, playerfile);
	

	
	//add officers left
	int r = m_player->getNumOfficers();
	m_officerslots = new Sprite *[r];
	m_officers = new Sprite *[r];

	for (int i = 0; i < r; i++)
	{

		m_officerslots[i] = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
		m_officerslots[i]->setPosition(origin.x + 64, origin.y + visibleSize.height - 64 - (128 * i));
		m_officerslots[i]->setScale(3);
		
		this->addChild(m_officerslots[i]);

		if (m_player->getOfficer(i) != NULL)
		{
			m_officers[i] = Sprite::create("art/officers.png", Rect(m_player->getOfficer(i)->getDefinition()->getPortrait().x, m_player->getOfficer(i)->getDefinition()->getPortrait().y, 128, 128));
			m_officers[i]->setPosition(m_officerslots[i]->getPosition());
			m_officers[i]->setScale(0.75F);
//			m_officernames[i]->setString(m_player->getOfficer(i)->getName());
			this->addChild(m_officers[i]);
		}
		else
		{
			m_officers[i] = NULL;
		}
	}
	//add rewards right
	for (int i = 0; i < 3; i++)
	{
		m_rewardslots[i] = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
		m_rewardslots[i]->setPosition(origin.x +visibleSize.width- 64, origin.y + visibleSize.height - 128- (80 * i));
		m_rewardslots[i]->setScale(2);
		this->addChild(m_rewardslots[i]);
		Label *label = NULL;
		char buffer[4];
		std::string str = "";
		if (m_stage->m_variables[(i * 2)] != 0)
		{
			switch (m_stage->m_variables[(i * 2)])
			{
			case 1:
				//module
				m_rewards[i] = Sprite::create("art/moduleicons.png", Rect(ReferenceLibrary::getInstance()->getModule(m_stage->m_variables[1 + (i * 2)])->getGraphicpos().x*32,
					ReferenceLibrary::getInstance()->getModule(m_stage->m_variables[1 + (i * 2)])->getGraphicpos().y*32, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				m_rewards[i]->setScale(2);
				this->addChild(m_rewards[i]);
				break;
			case 2:
				//officers
				m_rewards[i] = Sprite::create("art/officers.png", Rect(ReferenceLibrary::getInstance()->getOfficer(m_stage->m_variables[1 + (i * 2)])->getPortrait().x * 128,
					ReferenceLibrary::getInstance()->getOfficer(m_stage->m_variables[1 + (i * 2)])->getPortrait().y * 128, 128, 128));
				m_rewards[i]->setScale(0.5F);
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				break;
			case 3:
				//money
				m_rewards[i] = Sprite::create("art/shipUI.png",Rect(224,192,32,32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->setMoney(m_player->getMoney() + m_stage->m_variables[1 + (i * 2)]);
				break;

			case 4:
				//item

				break;
			case 5:
				//resource 0
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 0, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(0, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 6:
				//resource 1
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 32, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(1, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 7:
				//resource 2
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 64, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(2, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 8:
				//resource 3
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 96, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(3, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 9:
				//fuel
				if (m_player->getShip()->getFuel() < m_player->getShip()->getMaxFuel())
				{
					m_stage->m_variables[1 + (i * 2)] = m_player->getShip()->Refuel(m_stage->m_variables[1 + (i * 2)]);
					Sprite *spr = Sprite::create("art/moduleicons.png", Rect(0, 0, 32, 32));
					spr->setPosition(m_rewardslots[i]->getPosition());
					Vector <FiniteTimeAction *> action;
					action.pushBack(MoveTo::create(1.0F, m_outline.m_fuelbin->getPosition()));
					action.pushBack(RemoveSelf::create());
					Sequence *seq = Sequence::create(action);
					spr->runAction(seq);
					this->addChild(spr);
				}
				if (m_stage->m_variables[1 + (i * 2)]>0)
				{
					m_rewards[i] = Sprite::create("art/moduleicons.png", Rect(0, 0, 32, 32));
					m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
					this->addChild(m_rewards[i]);
					itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
					str.append(buffer);
					label = Label::createWithBMFont("art/font.fnt", str);
					label->setSystemFontSize(16);
					label->setColor(Color3B(255, 255, 255));
					label->setAnchorPoint(Vec2(0, 0));
					m_rewards[i]->setScale(2);
					m_rewards[i]->addChild(label,0,50);
				}
				else
				{
					m_rewards[i] = NULL;
				}
				break;
			case 10:
				//ordinance
				m_rewards[i] = Sprite::create("art/moduleicons.png", Rect(32, 0, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(16);
				label->setColor(Color3B(255, 255, 255));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label,0,50);
				break;
			}
		}
		else
		{
			m_rewards[i] = NULL;
		}
	}
	//add exp
	Sprite *spr = Sprite::create("art/shipUI.png", Rect(224, 224, 32, 32));
	//spr2->setPosition(origin.x + visibleSize.width - 536, origin.y + 64);
	spr->setScale(2);
	spr->setPosition(origin.x + visibleSize.width - 632, origin.y + 96);
	this->addChild(spr);
	char buffer[4];
	itoa(m_stage->m_variables[6], &buffer[0], 10); 
	m_player->addXP(m_stage->m_variables[6]);
	Label *label = Label::createWithBMFont("art/font.fnt", "");
	label->setString("+" + std::string(buffer));
	label->setPosition(origin.x + visibleSize.width - 566, origin.y + 96);
	Sprite *sprb = Sprite::create("art/backbox.png");
	sprb->setAnchorPoint(Vec2(0.5F, 0.5F));
	sprb->setScale(1);
	sprb->setPosition(label->getPosition());
	this->addChild(sprb);
	this->addChild(label);
}

void ModeReward::ButtonCallback0(Ref *pSender)
{
	Scene *scene=GameManager::getInstance()->getHandler()->getScene(0);
	Director::getInstance()->replaceScene(scene);

}

void 	ModeReward::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
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
					break;
				}
			}
		}
	}
	if (m_outline.m_heldsprite == -1)
	{
		//check for rewards
		for (int i = 0; i < 3; i++)
		{
			if (m_rewards[i] != NULL)
			{
				if (m_rewards[i]->getBoundingBox().containsPoint(touch->getLocation())&& 
					(m_stage->m_variables[i*2]==9||m_stage->m_variables[i*2]==10||m_stage->m_variables[i*2]==1||m_stage->m_variables[i*2]==3||m_stage->m_variables[i*2]==2))
				{
					m_outline.m_heldsprite = 30 + i;
					if (m_stage->m_variables[i * 2] == 1)
					{
						m_description->setString(ReferenceLibrary::getInstance()->getModule(m_stage->m_variables[1+(i * 2)])->getDesc());
					}
					break;
				}
			}
		}
	}

}

void 	ModeReward::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
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
			if (m_outline.m_heldsprite < 30)
			{
				m_outline.m_items[m_outline.m_heldsprite - 20]->setPosition(touch->getLocation());
			}
			else
			{
				m_rewards[m_outline.m_heldsprite - 30]->setPosition(touch->getLocation());
			}
		}

	}
}

void 	ModeReward::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	//let go, figure out which slot we're over, if its empty move the sprite there
	if (m_outline.m_heldsprite != -1)
	{
		if (m_outline.m_bin->getBoundingBox().containsPoint(touch->getLocation()))
		{
			//bin the thing
			if (m_outline.m_heldsprite < 30)
			{
				m_outline.BinThing(this);
			}
			else
			{
				m_stage->m_variables[(m_outline.m_heldsprite - 30) * 2] = 0;
				this->removeChild(m_rewards[m_outline.m_heldsprite - 30]);
				m_rewards[m_outline.m_heldsprite - 30] = NULL;
			}

		}
		else
		{
			if (m_outline.m_heldsprite < 20)
			{
				m_outline.MoveModule(touch->getLocation());
			}
			else
			{
				if (m_outline.m_heldsprite < 30)
				{
					m_outline.ResetPosition();
				}
				else
				{
					MoveReward(touch->getLocation());
				}

			}


		}
	}
	m_outline.m_heldsprite = -1;
}

void ModeReward::MoveReward(Vec2 p)
{
	//get type 
	int t = m_stage->m_variables[(m_outline.m_heldsprite - 30) * 2];
	switch (t)
	{
	case 1:
		//module
		for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
		{
			if (m_outline.m_slots[i]->getBoundingBox().containsPoint(p))
			{
				if (m_player->getShip()->getModule(i) == NULL)
				{
					//install the module reward
					m_player->getShip()->InstallModule(ReferenceLibrary::getInstance()->getModule(m_stage->m_variables[1 + ((m_outline.m_heldsprite - 30) * 2)]), i);
					m_outline.m_modulesprites[i] = m_rewards[m_outline.m_heldsprite - 30];
					m_outline.m_modulesprites[i]->setPosition(m_outline.m_slots[i]->getPosition());
					m_rewards[m_outline.m_heldsprite - 30] = NULL;
					m_stage->m_variables[m_outline.m_heldsprite - 30 * 2] = 0;
					m_outline.m_heldsprite = -1;
					break;
				}
			}
		}

		break;
	case 2:
		//officer
		for (int i = 0; i < m_player->getNumOfficers(); i++)
		{

		}
		break;

	case 9:
		//fuel
		for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
		{
			if (m_outline.m_slots[i]->getBoundingBox().containsPoint(p))
			{
				m_outline.AddPurchase(3, &m_stage->m_variables[1 + ((m_outline.m_heldsprite - 30) * 2)],p, m_rewards[m_outline.m_heldsprite - 30]);
				if (m_stage->m_variables[1 + (m_outline.m_heldsprite - 30 * 2)] <= 0)
				{
					m_stage->m_variables[m_outline.m_heldsprite - 30 * 2] = 0;
					m_rewards[m_outline.m_heldsprite-30] = NULL;
					m_outline.m_heldsprite = -1;
				}
				else
				{
					Label *label=(Label*)m_rewards[m_outline.m_heldsprite - 30]->getChildByTag(50);
					if (label != NULL)
					{
						char buffer[4];
						itoa(m_stage->m_variables[1 + ((m_outline.m_heldsprite - 30) / 2)], &buffer[0], 10);
						label->setString(buffer);
					}

				}
				
			}
		}
		break;

	case 10:
		//ordinance
		for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
		{
			if (m_outline.m_slots[i]->getBoundingBox().containsPoint(p))
			{
				m_outline.AddPurchase(4, &m_stage->m_variables[1 + ((m_outline.m_heldsprite - 30) * 2)], p, m_rewards[m_outline.m_heldsprite - 30]);
				if (m_stage->m_variables[1 + ((m_outline.m_heldsprite - 30) * 2)] <= 0)
				{
					m_stage->m_variables[m_outline.m_heldsprite - 30 * 2] = 0;
					m_rewards[m_outline.m_heldsprite - 30] = NULL;
					m_outline.m_heldsprite = -1;
				}
				else
				{
					if (m_rewards[m_heldsprite - 30] != NULL && m_outline.m_modulesprites[i]!=m_rewards[m_heldsprite-30])
					{
						Label *label = (Label*)m_rewards[m_outline.m_heldsprite - 30]->getChildByTag(50);
						if (label != NULL)
						{
							char buffer[4];
							itoa(m_stage->m_variables[1 + ((m_outline.m_heldsprite - 30) / 2)], &buffer[0], 10);
							label->setString(buffer);
						}
					}
				}
			}
		}
		break;
	}
	//if module, check for module slots
	if (m_outline.m_heldsprite != -1)
	{
		m_rewards[m_outline.m_heldsprite - 30]->setPosition(m_rewardslots[m_outline.m_heldsprite - 30]->getPosition());
	}
	//if officer, check for officer slots
}

void ModeReward::Save(FILE *file)
{
	char v = 4;
	fwrite((void*)&v, sizeof(char), 1, file);

	//save whether items are still in the reward slots
	for (int i = 0; i < 3; i++)
	{
		if (m_rewards[i] == NULL)
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
		else
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
	}

}

void ModeReward::Setup(Player *playerfile, MissionNode *stage, CBReader *reader)
{
	m_player = playerfile;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_stage = stage;
	m_player = playerfile;
	m_outline.Setup(playerfile->getShip(), this, playerfile);

	char values[3];
	for (int i = 0; i < 3; i++)
	{
		values[i] = reader->ReadChar();
	}

	//add officers left
	int r = m_player->getNumOfficers();
	m_officerslots = new Sprite *[r];
	m_officers = new Sprite *[r];

	for (int i = 0; i < r; i++)
	{

		m_officerslots[i] = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
		m_officerslots[i]->setPosition(origin.x + 64, origin.y + visibleSize.height - 64 - (128 * i));
		m_officerslots[i]->setScale(3);

		this->addChild(m_officerslots[i]);

		if (m_player->getOfficer(i) != NULL)
		{
			m_officers[i] = Sprite::create("art/officers.png", Rect(m_player->getOfficer(i)->getDefinition()->getPortrait().x, m_player->getOfficer(i)->getDefinition()->getPortrait().y, 128, 128));
			m_officers[i]->setPosition(m_officerslots[i]->getPosition());
			m_officers[i]->setScale(0.75F);
			//			m_officernames[i]->setString(m_player->getOfficer(i)->getName());
			this->addChild(m_officers[i]);
		}
		else
		{
			m_officers[i] = NULL;
		}
	}
	//add rewards right

	for (int i = 0; i < 3; i++)
	{
		m_rewardslots[i] = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
		m_rewardslots[i]->setPosition(origin.x + visibleSize.width - 64, origin.y + visibleSize.height - 128 - (80 * i));
		m_rewardslots[i]->setScale(2);
		this->addChild(m_rewardslots[i]);
		Label *label = NULL;
		char buffer[4];
		std::string str = "";
		if (m_stage->m_variables[(i * 2)] != 0 && values[i] == 42)
		{
			switch (m_stage->m_variables[(i * 2)])
			{
			case 1:
				//module
				m_rewards[i] = Sprite::create("art/moduleicons.png", Rect(ReferenceLibrary::getInstance()->getModule(m_stage->m_variables[1 + (i * 2)])->getGraphicpos().x * 32,
					ReferenceLibrary::getInstance()->getModule(m_stage->m_variables[1 + (i * 2)])->getGraphicpos().y * 32, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				m_rewards[i]->setScale(2);
				this->addChild(m_rewards[i]);
				break;
			case 2:
				//officers
				m_rewards[i] = Sprite::create("art/officers.png", Rect(ReferenceLibrary::getInstance()->getOfficer(m_stage->m_variables[1 + (i * 2)])->getPortrait().x * 128,
					ReferenceLibrary::getInstance()->getOfficer(m_stage->m_variables[1 + (i * 2)])->getPortrait().y * 128, 128, 128));
				m_rewards[i]->setScale(0.5F);
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				break;
			case 3:
				//money
				m_rewards[i] = Sprite::create("art/shipUI.png", Rect(224, 192, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->setMoney(m_player->getMoney() + m_stage->m_variables[1 + (i * 2)]);
				break;

			case 4:
				//item

				break;
			case 5:
				//resource 0
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 0, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(0, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 6:
				//resource 1
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 32, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(1, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 7:
				//resource 2
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 64, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(2, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 8:
				//resource 3
				m_rewards[i] = Sprite::create("art/colonyUI.png", Rect(0, 96, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->ModResource(3, m_stage->m_variables[1 + (i * 2)]);
				break;
			case 9:
				//fuel
				m_rewards[i] = Sprite::create("art/itemsprites.png", Rect(0, 0, 32, 32));
				m_rewards[i]->setPosition(m_rewardslots[i]->getPosition());
				this->addChild(m_rewards[i]);
				itoa(m_stage->m_variables[1 + (i * 2)], &buffer[0], 10);
				str.append(buffer);
				label = Label::createWithBMFont("art/font.fnt", str);
				label->setSystemFontSize(24);
				label->setColor(Color3B(255, 0, 0));
				label->setAnchorPoint(Vec2(0, 0));
				m_rewards[i]->setScale(2);
				m_rewards[i]->addChild(label);
				m_player->getShip()->modFuel(-1 * m_stage->m_variables[1 + (i * 2)]);
			}
		}
		else
		{
			m_rewards[i] = NULL;
		}
	}
	//add exp
	Sprite *spr = Sprite::create("art/shipUI.png", Rect(224, 224, 32, 32));
	//spr2->setPosition(origin.x + visibleSize.width - 536, origin.y + 64);
	spr->setScale(2);
	spr->setPosition(origin.x + visibleSize.width - 632, origin.y + 96);
	this->addChild(spr);
	char buffer[4];
	itoa(m_stage->m_variables[6], &buffer[0], 10);
	m_player->addXP(m_stage->m_variables[6]);
	Label *label = Label::createWithBMFont("art/font.fnt", "");
	label->setString("+" + std::string(buffer));
	label->setPosition(origin.x + visibleSize.width - 566, origin.y + 96);
	Sprite *sprb = Sprite::create("art/backbox.png");
	sprb->setAnchorPoint(Vec2(0.5F, 0.5F));
	sprb->setScale(1);
	sprb->setPosition(label->getPosition());
	this->addChild(sprb);
	this->addChild(label);



}

cocos2d::Scene* ModeReward::createLoadScene(MissionNode *stage,CBReader *reader)
{
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModeReward::create();

	layer->Setup(GameManager::getInstance()->getPlayer(), stage,reader);
	// add layer as a child to scene
	scene->addChild(layer,0,100);

	// return the scene
	return scene;


}