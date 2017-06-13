#include "Mode_Spacebattle.h"
#include "Module_Booster.h"
#include "Module_ability.h"
#include "Common.h"
#include "GameManager.h"
#include "ReferenceLibrary.h"
#include "ModeGameOver.h"

USING_NS_CC;

cocos2d::Scene *Mode_Spacebattle::DebugcreateScene()
{
	int v = 0;
	Playership *player = new Playership(v);
	player->InstallModule(ReferenceLibrary::getInstance()->getModule(0),0);
	player->InstallModule(ReferenceLibrary::getInstance()->getModule(6),1);
	player->InstallModule(ReferenceLibrary::getInstance()->getModule(9),2);
	int numenemies = 3;
	NPCship **enemies = new NPCship *[3];
	
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/spaceships.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);
 
	for (int i = 0; i < 3; i++)
	{
		while (true)
		{
			std::string str = cbr->ReadString();
			if (str.find("SHIP") != std::string::npos)
			{
				int v = cbr->ReadInt();
				if (v == 0)
				{
					break;
				}
			}
		}
		//find ship
		enemies[i] = new NPCship(cbr);

		cbr->setReadposition(0);
	}
	delete cbr;
	return Mode_Spacebattle::createScene(player, enemies, numenemies, NULL, 0);
}

Scene* Mode_Spacebattle::createScene(Playership *playership, NPCship **enemyship, int numenemies, NPCship **allyship, int numallies)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Mode_Spacebattle::create();

	layer->Setup(playership, enemyship, numenemies, allyship, numallies);
	// add layer as a child to scene
	scene->addChild(layer,0,100);

	// return the scene
	return scene;
}

bool Mode_Spacebattle::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	cocos2d::CCTexture2D *t0 = cocos2d::CCTextureCache::getInstance()->addImage("art/battlesprites.png");
	//m_batchnodes[0] = cocos2d::SpriteBatchNode::createWithTexture(t0);
	cocos2d::CCTexture2D *t1 = cocos2d::CCTextureCache::getInstance()->addImage("art/battleeffects.png");
	//m_batchnodes[1] = cocos2d::SpriteBatchNode::createWithTexture(t1);

	//this->addChild(m_batchnodes[0]);
	//this->addChild(m_batchnodes[1]);
	Size visibleSize = Size(800, 640);
	Size s = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	int r = rand() % 255; int g = rand() % 255; int b = rand() % 255;

	Sprite *stars[32];
	for (int i = 0; i < 32; i++)
	{
		int r = rand() % 4;
		stars[i] = Sprite::create("art/galaxysprites.png", Rect(r * 16, 4, 16, 16));
		this->addChild(stars[i]);
		stars[i]->setPosition(origin.x + (rand() % 800), origin.y + (visibleSize.height*0.5F) + (rand() % 320));
	}


	Sprite *background = Sprite::create("art/battlebackground.png");
	background->setPosition(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height*0.75F));
	this->addChild(background);
	background->setColor(Color3B(r, g, b));

	for (int i = 0; i < 8; i++)
	{
		background = Sprite::create("art/battleuibackground.png");
		background->setPosition(origin.x + (i * 128), origin.y + (visibleSize.height*0.25F));
		this->addChild(background);
	}


	//build pbar
	m_bar = new Pbar(cocos2d::Vec2(0 + origin.x, 320 + origin.y), cocos2d::Vec2(800, 4), 0.0F, 0.5F);
	//
	m_bar->Addsprites(this);
	//build labels
	m_tickover = 0;
	//hull label
	m_labels[0] = Label::createWithBMFont("art/font.fnt", "Hull:");

	m_labels[1] = Label::createWithBMFont("art/font.fnt", "Fuel:");
	//fuel label
	m_labels[2] = Label::createWithBMFont("art/font.fnt", "Ord:");
	//ordinance label
	m_labels[3] = Label::createWithBMFont("art/font.fnt", "Enemy:");
	
	cocos2d::Sprite *backgrounds[4];
	cocos2d::Sprite *backicons[4];
	for (int i = 0; i < 4; i++)
	{
		backgrounds[i] = cocos2d::Sprite::create("art/backbox2.png");
		backicons[i] = cocos2d::Sprite::create("art/battleicons.png", Rect(0 + (32 * i), 0, 32, 32));
	}
	for (int i = 0; i < 4; i++)
	{
		backgrounds[i]->setPosition(origin.x + (192 * i) + 96, origin.y + 320 - 32);
		backgrounds[i]->setScaleX(1.5F);
		backicons[i]->setPosition(origin.x + (192 * i)+16, origin.y + 320 - 32);
		m_labels[i]->setAnchorPoint(cocos2d::Vec2(1.0F, 0.5));
		m_labels[i]->setPosition(origin.x + (192 * i)+188, origin.y+320 - 32);
		this->addChild(backgrounds[i]);
		this->addChild(backicons[i]);
		if (i < 4)
		{
			this->addChild(m_labels[i]);
		}

	}


	for (int i = 0; i < 2; i++)
	{
		m_segbars[i] = Segbar::create();
		m_segbars[i]->setScale(1.5F);
		m_segbars[i]->setPosition(origin.x + (192 * i) + 32, origin.y + 320 - 32);
		this->addChild(m_segbars[i]);
	}

	m_buttontarget = -1;
	m_target = 0;
	schedule(schedule_selector(Mode_Spacebattle::update), 1.0 / 20);

	return true;
}

Mode_Spacebattle::~Mode_Spacebattle()
{

}

void Mode_Spacebattle::Setup(Playership *ship, NPCship **enemyships, int numenemies, NPCship **allyships, int numallies)
{
	m_numenemies = numenemies;
	m_numallies = numallies;
	m_ticking = true;
	//set initial ships

	m_player = new StarshipBattler(ship);
	m_enemies = new StarshipBattler*[m_numenemies];
	for (int i = 0; i < m_numenemies; i++)
	{
		m_enemies[i] = new StarshipBattler(enemyships[i]);
		m_enemies[i]->RandomizeCommand();
	}

	m_allies = new StarshipBattler*[m_numallies];
	for (int i = 0; i < m_numallies; i++)
	{
		m_allies[i] = new StarshipBattler(allyships[i]);
		m_allies[i]->RandomizeCommand();
	}

	CommonSetup();



	delete[]enemyships;
	delete[]allyships;
}

void Mode_Spacebattle::SetupButtons()
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int d = 0;
	Playership *p = (Playership *)m_player->getShip();
	for (int i = 0; i < p->getNumModules(); i++)
	{
		Module *m = p->getModule(i);
		if (m != NULL)
		{
			if (m->getType() == MT_ABILITY)
			{
				Module_Ability *ma = (Module_Ability *)m;
				Ability_Space *as = ma->getAbility();
				switch (as->getAbilityType())
				{
				case AT_beam:
					m_buttons[d] = cocos2d::ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
					break;
				case AT_missile:
					m_buttons[d] = cocos2d::ui::Button::create("art/greenbutton0.png", "art/greenbutton1.png", "art/greenbutton2.png");
					break;

				case AT_projectile:
					m_buttons[d] = cocos2d::ui::Button::create("art/redbutton0.png", "art/redbutton1.png", "art/redbutton2.png");
					break;
				case AT_other:
					m_buttons[d] = cocos2d::ui::Button::create("art/greybutton0.png", "art/greybutton1.png", "art/greybutton2.png");
					break;
				case AT_buff:
					m_buttons[d] = cocos2d::ui::Button::create("art/amberbutton0.png", "art/amberbutton1.png", "art/amberbutton2.png");
					break;
				}
				m_buttons[d]->setTitleText(getWeaponstring(as));
				if (d >= 5)
				{
					m_buttons[d]->setPosition(cocos2d::Vec2(origin.x + (160 * (d-5)) + 80, origin.y + 128));
				}
				else
				{
					m_buttons[d]->setPosition(cocos2d::Vec2(origin.x + (160 * (d)) + 80, origin.y + 192));
				}

				this->addChild(m_buttons[d]); 
				m_buttontranslation[d] = i;
				m_buttons[d]->addClickEventListener(CC_CALLBACK_1(Mode_Spacebattle::ButtonCallback,this));
				m_buttons[d]->setScaleX(1.25F);;
			
				d++;
			}
		}
		if (d>10)
		{
			break;
		}
	}
	m_buttoncount = d;

}

std::string Mode_Spacebattle::getWeaponstring(Ability_Space *ab)
{
	std::string str = ab->getName() + "\n";
	if (ab->getCost() > 0)
	{
		switch (ab->getCostType())
		{
		case AC_crew:
			str.append("crew:");
			break;
		case AC_fuel:
			str.append("fuel:");
			break;
		case AC_ordinance:
			str.append("Ammo:");
			break;
		}
		char buffer[3];
		sprintf(buffer, "%d", ab->getCost());
	
		str.append(buffer);
	}
	return str;
}

cocos2d::Vec2 Mode_Spacebattle::getPos(int i, bool side)
{

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (!side)
	{
		switch (i)
		{
		case 0:
			return cocos2d::Vec2(origin.x+100, origin.y+480);
			break;

		case 1:
			return cocos2d::Vec2(origin.x + 100, origin.y + 416);
			break;

		case 2:
			return cocos2d::Vec2(origin.x + 100, origin.y + 544);
			break;
		}
	}
	else
	{
		switch (i)
		{
		case 0:
			return cocos2d::Vec2(origin.x + 700, origin.y + 480);
			break;

		case 1:
			return cocos2d::Vec2(origin.x + 700, origin.y + 416);
			break;

		case 2:
			return cocos2d::Vec2(origin.x + 700, origin.y + 544);
			break;
		}
	}
}


void Mode_Spacebattle::ButtonCallback(Ref *pSender)
{
	if (m_ticking == true)
	{
		if (m_enemies[m_target]->getShip()->getHP() > 0)
		{
			for (int i = 0; i < 10; i++)
			{
				if (pSender == m_buttons[i])
				{
					Playership *p = (Playership*)m_player->getShip();
					Module_Ability *ma = (Module_Ability*)p->getModule(m_buttontranslation[i]);
					Ability_Space *as = ma->getAbility();
					if (m_buttontarget == i)
					{

						//subtract resources
						m_player->subtractCommand(as->getCommandcost()); 
						if (as->getCost()>0)
						{
							switch (as->getCostType())
							{
							case AC_fuel:
								p->modFuel(as->getCost());
								break;
							case AC_ordinance:
								m_ordinanceused -= as->getCost();
								m_ordinance -= as->getCost();
								break;

							case AC_crew:
						//		p->modCrew(as->getCost());
								break;
							}
						}
						//stop the clock
						m_ticking = false;
						m_buttontarget = -1;
						m_bar->setRed(0);
						m_bar->setGreen(m_player->getCommand() / 100);
						Abilitychecks();
						switch (as->getAbilityType())
						{
						case AT_beam:
							LaserAttack(m_target + m_numallies + 1, 0, as,m_player->getShip()->getOffence(0));
							break;
						case AT_missile:
							MissileAttack(m_target + m_numallies + 1, 0, as, m_player->getShip()->getOffence(1));
							break;
						case AT_projectile:
							ProjectileAttack(m_target + m_numallies + 1, 0, as, m_player->getShip()->getOffence(2));
							break;
						case AT_buff:
							Buff(0, as);
							break;
						case AT_other:
							Debuff(0,m_target + m_numallies + 1, as);
							break;
						}
					}
					else
					{
						m_buttontarget = i;
						float v = as->getCommandcost();
						m_bar->setRed(v / 100);

					}
					break;
				}
			}
		}
	
	}
}

void Mode_Spacebattle::AddIcon(Shipmod mod, int value)
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int y = 32;
	switch (mod)
	{
	case SM_deflect:
		y = 32;
			break;

		case SM_intercept:
			y = 64;
			break;

		case SM_dodge:
			y = 96;
			break;

		case SM_BEAM:
			y = 128;
			break;
		case SM_MISSILE:
			y = 160;
			break;
		case SM_PROJECTILE:
			y = 192;
			break;

	}
	int x = 0;
	if (value < 0)
	{
		x = 64;
	}
	Sprite *spr = Sprite::create("art/battlegui.png", cocos2d::CCRect(x, y, 64, 32));
	//m_buttons[d]->setPosition(cocos2d::Vec2(origin.x + (160 * (d - 5)) + 80, origin.y + 128));
	spr->setPosition(origin.x+32 +(64 * m_playerstateicons.size()), origin.y + 248);
	m_playerstateicons.pushBack(spr);
	this->addChild(spr);

}

void Mode_Spacebattle::Buff(int target, Ability_Space *as)
{
	//generate a state
	ShipStatus *s = new ShipStatus((AS_buffeffect*)as->getEffect());
	//add to state tracker for that object
	if (target == 0)
	{
		m_player->Addstatus(s, as->getEffect()->m_graphicpos[1]);
		//add icon to status list
		AddIcon(s->m_type,s->m_value);
	}
	else
	{
		if (target < m_numallies + 1)
		{
			m_allies[target - 1]->Addstatus(s, as->getEffect()->m_graphicpos[1]);

		}
		else
		{
			m_enemies[target - 1 - m_numallies]->Addstatus(s, as->getEffect()->m_graphicpos[1]);

		}
	}
	//apply the state to the target

	//play animation on ship
	Sprite *spr = Sprite::create("art/battleeffects.png", Rect(8 * as->getEffect()->m_graphicpos[0].x, 8 * as->getEffect()->m_graphicpos[0].y * 8, 16, 16));
	spr->setPosition(m_bsprites[target]->getPosition());
	spr->setScale(4);
	Vector <FiniteTimeAction *> action;
	Animation *an = cocos2d::Animation::create();
	for (int i = 0; i < 4; i++)
	{
		//add four frames
		SpriteFrame *sf = cocos2d::SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect(as->getEffect()->m_graphicpos[1].x * 8 + (16 * i), as->getEffect()->m_graphicpos[1].y * 8, 16, 16));
		an->addSpriteFrame(sf);
	}
	an->setDelayPerUnit(0.2F);
	action.pushBack(Animate::create(an));
	action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Spacebattle::AttackConcluded)));
	action.pushBack(RemoveSelf::create());
	Sequence *seq = Sequence::create(action);
	spr->runAction(seq);

	this->addChild(spr);
}

void Mode_Spacebattle::Debuff(int origin, int target, Ability_Space *as)
{
	//generate a state
	ShipStatus *s = new ShipStatus((AS_buffeffect*)as->getEffect());
	s->m_value = s->m_value*-1;
	//add to state tracker for that object
	if (target == 0)
	{
		m_player->Addstatus(s, as->getEffect()->m_graphicpos[1]);
		AddIcon(s->m_type, s->m_value);
	}
	else
	{
		if (target < m_numallies + 1)
		{

			m_allies[target - 1]->Addstatus(s, as->getEffect()->m_graphicpos[1]);
		}
		else
		{

			m_enemies[target - 1 - m_numallies]->Addstatus(s, as->getEffect()->m_graphicpos[1]);
		}
	}
	//apply the state to the target

	//play animation on ship
	Sprite *spr = Sprite::create("art/battleeffects.png", Rect(8 * as->getEffect()->m_graphicpos[0].x, 8 * as->getEffect()->m_graphicpos[0].y * 8, 16, 16));
	spr->setPosition(m_bsprites[target]->getPosition());
	spr->setScale(4);
	Vector <FiniteTimeAction *> action;
	Animation *an = cocos2d::Animation::create();
	for (int i = 0; i < 4; i++)
	{
		//add four frames
		SpriteFrame *sf = cocos2d::SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect(as->getEffect()->m_graphicpos[1].x * 8 + (16 * i), as->getEffect()->m_graphicpos[1].y * 8, 16, 16));
		an->addSpriteFrame(sf);
	}
	an->setDelayPerUnit(0.2F);
	action.pushBack(DelayTime::create(0.4F));
	action.pushBack(Animate::create(an));
	action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Spacebattle::AttackConcluded)));
	Sequence *seq = Sequence::create(action);
	spr->runAction(seq);

	this->addChild(spr);

	Sprite *spr0 = Sprite::create("art/battleeffects.png", Rect(8 * as->getEffect()->m_graphicpos[0].x, 8 * as->getEffect()->m_graphicpos[0].y * 8, 16, 16));
	spr0->setPosition(m_bsprites[target]->getPosition());
	spr0->setScale(4);
	Vector <FiniteTimeAction *> action0;
	Animation *an0 = cocos2d::Animation::create();
	for (int i = 0; i < 4; i++)
	{
		//add four frames
		SpriteFrame *sf = cocos2d::SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect(as->getEffect()->m_graphicpos[0].x * 8 + (16 * i), as->getEffect()->m_graphicpos[0].y * 8, 16, 16));
		an->addSpriteFrame(sf);
	}
	an0->setDelayPerUnit(0.2F);

	action0.pushBack(Animate::create(an0));
	Sequence *seq0 = Sequence::create(action);
	spr0->runAction(seq0);

	this->addChild(spr0);
}

void Mode_Spacebattle::update(float dt)
{
	dt = dt * 2;
	if (m_ticking == true)
	{

		int r=m_player->Update(dt);
		if (r != -1)
		{
			this->removeChild(m_playerstateicons.at(r));
			m_playerstateicons.eraseObject(m_playerstateicons.at(r));
			ReshuffleIcons();
		}
		for (int i = 0; i<m_numallies; i++)
		{
			m_allies[i]->Update(dt);
		}
		for (int i = 0; i<m_numenemies; i++)
		{
			m_enemies[i]->Update(dt);
		}
		m_tickover += dt;
		if (m_tickover>0.5F)
		{
			m_bar->setGreen(m_player->getCommand() / 100);
			m_tickover = 0;
		}
		Statehandler(dt);
		AI();
	}
	Abilitychecks();
}

void Mode_Spacebattle::AI()
{

	for (int i = 0; i<m_numenemies; i++)
	{
		if (m_enemies[i]->getCommand() >= 100 && m_enemies[i]->getShip()->getHP()>0)
		{
			Ability_Space *as = m_enemies[i]->getAI();
			m_ticking = false;
			int s = rand() % 2+m_numallies;
			if (s>1)
			{
				if (m_allies[s - 2]->getShip()->getHP() <= 0)
				{
					s = 0;
				}
			}
			if (s<2)
			{
				NPCaction(0, i + 1 + m_numallies, as, m_enemies[i]->getShip());
			}
			else
			{
				NPCaction(s-2, i + 1 + m_numallies, as, m_enemies[i]->getShip());
			}
		}
	}
	if (m_numallies>0)
	{
		for (int i = 0; i < m_numallies; i++)
		{
			if (m_allies[i]->getCommand() >= 100)
			{
				Ability_Space *as = m_enemies[i]->getAI();

				int s = rand() % m_numenemies;
				NPCaction(1+m_numallies+s, i + 1 + m_numallies, as, m_enemies[i]->getShip());
			}
		}
	}
}

void Mode_Spacebattle::NPCaction(int target, int origin, Ability_Space *as, Starship *ship)
{
	m_ticking = false;
	switch (as->getAbilityType())
	{
		case AT_beam:
			LaserAttack(target, origin, as, ship->getOffence(0));
		break; 

	case AT_missile:
		MissileAttack(target, origin, as, ship->getOffence(0));
		break;

	case AT_projectile:
		ProjectileAttack(target, origin, as, ship->getOffence(1));
		break;

	case AT_buff:
		Buff(origin, as);
		break;
	case AT_other:
		Debuff(origin, target, as);
		break;
	}
}

void Mode_Spacebattle::Statehandler(float dt)
{
	if (m_player->getNumStatus() > 0)
	{
		for (int i = 0; i < m_player->getNumStatus(); i++)
		{
			if (m_player->getStatus(i)->timeleft < 3 && m_playerstateicons.at(i)->numberOfRunningActions()==0)
			{
				m_playerstateicons.at(i)->runAction(Blink::create(6, 12));
			}
		}
	}

}

void Mode_Spacebattle::ReshuffleIcons()
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (m_playerstateicons.size() > 0)
	{
		for (int i = 0; i < m_playerstateicons.size(); i++)
		{
			Sprite *spr = m_playerstateicons.at(i);
			spr->setPosition(origin.x + 32 + (64 * i), origin.y + 248);
		}
	}
}

void Mode_Spacebattle::Abilitychecks()
{
	Playership *p = (Playership*)m_player->getShip();
	for (int i = 0; i < m_buttoncount; i++)
	{
		Module_Ability *ma = (Module_Ability *)p->getModule(m_buttontranslation[i]);
		if (Abilitycheck(ma->getAbility()))
		{
			m_buttons[i]->setBright(true);
			m_buttons[i]->setEnabled(true);
		}
		else
		{
			m_buttons[i]->setBright(false);
			m_buttons[i]->setEnabled(false);
		}
	}
}

bool Mode_Spacebattle::Abilitycheck(Ability_Space *ab)
{
	Playership *p = (Playership*)m_player->getShip();
	if (m_player->getCommand() >= ab->getCommandcost())
	{
		switch (ab->getCostType())
		{
		case AC_fuel:
			if (p->getFuel() >= ab->getCost())
			{
				return true;
			}
			break;

		case AC_ordinance:
			if (m_ordinance >= ab->getCost())
			{
				return true;
			}
			break;
		}
	}
	return false;
}

void Mode_Spacebattle::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_ticking == true)
	{
		for (int i = 1 + m_numallies; i < 1 + m_numallies + m_numenemies; i++)
		{
			Vec2 v = getTouchpos(touch->getLocationInView(), this);
			//	v = v - Director::getInstance()->getVisibleOrigin();
			Vec2 target = m_bsprites[i]->getPosition();
			//m_tsprite->setPosition(v);
			float d = v.getDistance(target);
			if (d < 32 && m_bsprites[i]->isVisible())
			{
				if (getShip(i)->getShip()->getHP()>0)
				{
					m_target = i - 1 - m_numallies;
					m_tsprite->setPosition(m_bsprites[1 + m_numallies + m_target]->getPosition());
					std::string str; char buffer[4];

					sprintf(buffer, "%d", m_enemies[m_target]->getShip()->getHP());
					str = ""; str.append(buffer);
					m_labels[3]->setString(str);
					m_tsprite->setVisible(true);
				}
	
			}
		}
	}

}

void Mode_Spacebattle::LaserAttack(int target, int origin, Ability_Space *as, float mod)
{
	int damage=LaserDamageCalc(target, as, mod);
	//generate beam
	generateBeam(as->getEffect(),target, origin,mod);
	//generate impact
	if (damage > 0)
	{
		generateImpact(as->getEffect(), target);
	}
	//generate shield effect if necessary
	if (target == 0)
	{
		generateShield(m_player->getShip(), m_bsprites[target]);
		if (m_player->getShip()->getHP() > 0)
		{
			GenerateDamageNumber(target, 0.8F, damage);
		}
		else
		{
			GenerateExplosion(target);
		}
	}
	else
	{
		if (target < m_numallies + 1)
		{
			generateShield(m_allies[target-1]->getShip(), m_bsprites[target]);
			if (m_allies[target - 1]->getShip()->getHP()>0)
			{
				GenerateDamageNumber(target, 0.8F, damage);
			}
			else
			{
				GenerateExplosion(target);
			}
		}
		else
		{
			generateShield(m_enemies[target-1-m_numallies]->getShip(), m_bsprites[target]);
			if (m_enemies[target - 1-m_numallies]->getShip()->getHP()>0)
			{
				GenerateDamageNumber(target, 0.8F, damage);
			}
			else
			{
				GenerateExplosion(target);
				m_tsprite->setVisible(false);
			}
		}
	}



}

StarshipBattler *Mode_Spacebattle::getShip(int ship)
{
	if (ship == 0)
	{
		return m_player;
	}
	else
	{
		if (ship < m_numallies + 1)
		{
			return m_allies[ship - 1];
		}
		else
		{
			return m_enemies[ship - 1 - m_numallies];
		}
	}



}

void Mode_Spacebattle::ProjectileAttack(int target, int origin, Ability_Space *as, float mod)
{
	//roll to hit
	if (ProjectileHitChance(target, as, mod))
	{
		//on hit show projectile and impact
		AS_damageeffect *ae = (AS_damageeffect *)as->getEffect();
		//if hit, show impact on target	
		//then show either explosion or number
		Sprite *spr = Sprite::create("art/battleeffects.png", cocos2d::Rect(8 * ae->m_graphicpos[0].x, 8 * ae->m_graphicpos[0].y, 16, 8));
		spr->setPosition(m_bsprites[origin]->getPosition());
		spr->setScale(2);
		//move to and animate
		Vector <FiniteTimeAction *> action;
		//add animate action

		Animation *an = cocos2d::Animation::create();
		for (int i = 0; i < 4; i++)
		{
			SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((8 * ae->m_graphicpos[0].x) + 16 * i, 8 * ae->m_graphicpos[0].y, 16, 8));
			an->addSpriteFrame(frame);
		}
		an->setDelayPerUnit(0.05F);
		an->setLoops(5);
		action.pushBack(Animate::create(an));
		//add explode action
		action.pushBack(ScaleBy::create(0.01F, 2));
		an = cocos2d::Animation::create();
		for (int i = 0; i < 4; i++)
		{
			SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((8 * ae->m_graphicpos[1].x) + (16 * i), 8 * ae->m_graphicpos[1].y, 16, 16));
			an->addSpriteFrame(frame);
		}
		an->setDelayPerUnit(0.1F);
		an->setLoops(1);
		action.pushBack(Animate::create(an));
		//add remove self
		action.pushBack(RemoveSelf::create());

		Sequence *seq = Sequence::create(action);
		spr->runAction(seq);
		spr->runAction(MoveTo::create(1.0F, m_bsprites[target]->getPosition()));
		this->addChild(spr);
		if (getShip(target)->getShip()->getHP() > 0)
		{
			GenerateDamageNumber(target, 1.2F, ae->m_damage);
		}
		else
		{
			GenerateExplosion(target, 1);
			m_tsprite->setVisible(false);
		}
	}
	else
	{

		Vec2 p = m_bsprites[target]->getPosition(); 
		if (p.x < m_bsprites[origin]->getPosition().x)
		{
			p.x = p.x - 256;
		}
		else
		{
			p.x = p.x + 256;
		}

		//on miss, animate enemy ship to dodge and show shot fly by
		AS_damageeffect *ae = (AS_damageeffect *)as->getEffect();

		Sprite *spr = Sprite::create("art/battleeffects.png", cocos2d::Rect(8 * ae->m_graphicpos[0].x, 8 * ae->m_graphicpos[0].y, 16, 8));
		spr->setPosition(m_bsprites[origin]->getPosition());
		spr->setScale(2);
		//move to and animate
		Vector <FiniteTimeAction *> action;
		//add animate action

		Animation *an = cocos2d::Animation::create();
		for (int i = 0; i < 4; i++)
		{
			SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((8 * ae->m_graphicpos[0].x) + 16 * i, 8 * ae->m_graphicpos[0].y, 16, 8));
			an->addSpriteFrame(frame);
		}
		an->setDelayPerUnit(0.05F);
		an->setLoops(5);
		action.pushBack(Animate::create(an));

		action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Spacebattle::AttackConcluded)));
		//add remove self
		action.pushBack(RemoveSelf::create());

		Sequence *seq = Sequence::create(action);
		spr->runAction(seq);
		spr->runAction(MoveTo::create(1.0F, p));
		this->addChild(spr);

		Vector <FiniteTimeAction *> action0;
		//delay
		action0.pushBack(DelayTime::create(0.4F));
		//then dodge
		action0.pushBack(MoveBy::create(0.4F, Vec2(0, 64)));
		//and return to position
		action0.pushBack(MoveBy::create(0.4F, Vec2(0, -64)));
		Sequence *seq0 = Sequence::create(action0);
		m_bsprites[target]->runAction(seq0);
	}
}

bool Mode_Spacebattle::ProjectileHitChance(int target, Ability_Space *as,float mod)
{
	if (as->getEffect()->m_type == AE_damage)
	{
		AS_damageeffect *ae = (AS_damageeffect *)as->getEffect();

		int dodge = getShip(target)->getShip()->getDefence(2);
		if (dodge == 0)
		{

			getShip(target)->getShip()->Damage(ae->m_damage);
			return true;
		}
		else
		{
			int r = rand() % (int)ae->m_penetration;
			
			if (mod != 100)
			{
				float f = mod - 1;
				int v = ae->m_penetration*f;
				r = r + v;
			}
			if (r > dodge)
			{
				getShip(target)->getShip()->Damage(ae->m_damage);
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}

void Mode_Spacebattle::MissileAttack(int target, int origin, Ability_Space *as, float mod)
{
	if (as->getEffect()->m_type == AE_damage)
	{
		AS_damageeffect *ae = (AS_damageeffect *)as->getEffect();
		//calculate whether the attack hits
		if (MissileIntercept(target, as,mod))
		{
			Vec2 p = m_bsprites[target]->getPosition() - m_bsprites[origin]->getPosition(); //find a position between target and self
			p.x = p.x*0.1F; p.y = p.y*0.1F;
			p = m_bsprites[target]->getPosition()-p;
			//if miss, show intercept
			//calculate angle
			float a = std::atan2(p.x - m_bsprites[target]->getPosition().x, p.y - m_bsprites[target]->getPosition().y);
			Sprite *spr = Sprite::create("art/battleeffects.png", cocos2d::Rect(8 * ae->m_graphicpos[0].x, 8 * ae->m_graphicpos[1].y, 16, 8));
			spr->setPosition(m_bsprites[origin]->getPosition());
			spr->setScale(2);
			Vector <FiniteTimeAction *> action;
			//animate
			Animation *an = cocos2d::Animation::create();
			for (int i = 0; i < 4; i++)
			{
				SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((8 * ae->m_graphicpos[0].x) + 16 * i, 8 * ae->m_graphicpos[0].y, 16, 8));
				an->addSpriteFrame(frame);
			}
			an->setDelayPerUnit(0.05F);
			an->setLoops(5);
			action.pushBack(Animate::create(an));
			//remove self
			action.pushBack(RemoveSelf::create());
			Sequence *seq = Sequence::create(action);
			spr->runAction(seq);
			spr->runAction(MoveTo::create(0.8F, p)); //move to a location short of the target
			this->addChild(spr);

			
			//create interceptor sprite
			Sprite *intercept = cocos2d::Sprite::create("art/battleeffects.png", cocos2d::Rect(8 * getShip(target)->getShip()->getDefenceGraphic(1).x, 8 * getShip(target)->getShip()->getDefenceGraphic(1).y, 16, 16));
			intercept->setPosition(m_bsprites[target]->getPosition());
			intercept->setVisible(false);
			intercept->setRotation((a * 57) - 90);
			Vector <FiniteTimeAction *> actionint;
			//wait
			actionint.pushBack(DelayTime::create(0.7F));
			//moveto target position
			actionint.pushBack(ToggleVisibility::create());
			actionint.pushBack(MoveTo::create(0.1F, p));
			an = Animation::create();
			actionint.pushBack(ScaleTo::create(0.0F, 3));
			//explode
			for (int i = 0; i < 4; i++)
			{
				SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((16 * i) + (getShip(target)->getShip()->getDefenceGraphic(1).x), 8 * getShip(target)->getShip()->getDefenceGraphic(1).y, 16, 16));
				an->addSpriteFrame(frame);
			}
			an->setDelayPerUnit(0.1F);
			an->setLoops(1);
			actionint.pushBack(Animate::create(an));
			//callback
			actionint.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Spacebattle::AttackConcluded)));
			//remove self
			actionint.pushBack(RemoveSelf::create());
	
			Sequence *seqint = Sequence::create(actionint);

			intercept->runAction(seqint);

			this->addChild(intercept);
		}
		else
		{
			//if hit, show impact on target	
			//then show either explosion or number
			Sprite *spr = Sprite::create("art/battleeffects.png", cocos2d::Rect(8 * ae->m_graphicpos[0].x, 8 * ae->m_graphicpos[0].y, 16, 8));
			spr->setPosition(m_bsprites[origin]->getPosition());
			spr->setScale(2);
			//move to and animate
			Vector <FiniteTimeAction *> action;
			//add animate action
			
			Animation *an = cocos2d::Animation::create();
			for (int i = 0; i < 4; i++)
			{
				SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((8 * ae->m_graphicpos[0].x) + 16 * i, 8 * ae->m_graphicpos[0].y, 16, 8));
				an->addSpriteFrame(frame);
			}
			an->setDelayPerUnit(0.05F);
			an->setLoops(5);
			action.pushBack(Animate::create(an));
			//add explode action
			action.pushBack(ScaleBy::create(0.01F,2));
			an = cocos2d::Animation::create();
			for (int i = 0; i < 4; i++)
			{
				SpriteFrame *frame = SpriteFrame::create("art/battleeffects.png", cocos2d::Rect((8 * ae->m_graphicpos[1].x) + (16 * i), 8 * ae->m_graphicpos[1].y, 16, 16));
				an->addSpriteFrame(frame);
			}
			an->setDelayPerUnit(0.1F);
			an->setLoops(1);
			action.pushBack(Animate::create(an));
			//add remove self
			action.pushBack(RemoveSelf::create());

			Sequence *seq = Sequence::create(action);
			spr->runAction(seq);
			spr->runAction(MoveTo::create(1.0F, m_bsprites[target]->getPosition()));
			this->addChild(spr);
			if (getShip(target)->getShip()->getHP() > 0)
			{
				GenerateDamageNumber(target, 1.2F, ae->m_damage);
			}
			else
			{
				GenerateExplosion(target,1);
				m_tsprite->setVisible(false);
			}
	
		}
	}

}

bool Mode_Spacebattle::MissileIntercept(int target, Ability_Space *as, float mod)
{
	//get weapon penetration score
	AS_damageeffect *ae = (AS_damageeffect *)as->getEffect();

	//get defence score
	int defence = getShip(target)->getShip()->getDefence(1);

	if (defence == 0)
	{
		getShip(target)->getShip()->Damage(ae->m_damage);
		return false;
	}
	else
	{
			int r = rand() % defence;
		if (r >= ae->m_penetration*mod)
		{

			return true;
		}
		getShip(target)->getShip()->Damage(ae->m_damage);
		return false;
	}

}

void Mode_Spacebattle::GenerateDamageNumber(int target, float delay, int damage)
{
	cocos2d::Label *label = cocos2d::Label::create();
	char buffer[3];
	this->addChild(label);
	label->setScale(2);

	sprintf(buffer, "%d", damage);
	label->setString(buffer);
	label->setColor(cocos2d::Color3B(255, 0, 0));
	label->setPosition(m_bsprites[target]->getPosition());
	label->setVisible(false);
	Vector <FiniteTimeAction *> action;
	action.pushBack(DelayTime::create(0.8F));
	action.pushBack(ToggleVisibility::create());
	action.pushBack(MoveBy::create(0.4F, cocos2d::Vec2(0, 32)));
//	action.pushBack(ani);
//	action.pushBack(DelayTime::create(0.8F));
	action.pushBack(RemoveSelf::create());
	action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Spacebattle::AttackConcluded)));
	Sequence *seq = Sequence::create(action);
	label->runAction(seq);

	if (damage > 0)
	{
		Sparks(target, damage);
	}


}

void Mode_Spacebattle::Sparks(int target, int count)
{
	Sequence *seq ;
	Vector <FiniteTimeAction *> action;
	for (int i = 0; i < count; i++)
	{
		action.clear();
		Sprite *spr = Sprite::create("art/debris.png", Rect(0,0,4,4));

		Vec2 p = m_bsprites[target]->getPosition();
		p.x -= 16;
		p.y -= 16;
		int x = (rand() % 128);
		int y = (rand() % 128);
		p.x += x / 4; x = x - 64;
		p.y += y / 4; y = y - 64;
		spr->setPosition(p);
		spr->setColor(Color3B(255, 255, 50));
		spr->setVisible(false);
		action.pushBack(DelayTime::create(0.8F));
		action.pushBack(ToggleVisibility::create());
		action.pushBack(TintTo::create(0.8F, 55, 0, 0));
		seq = Sequence::create(action);
		spr->runAction(seq);
		action.clear();

		action.pushBack(DelayTime::create(0.8F));
		action.pushBack(MoveBy::create(0.8F, Vec2(x, y)));
		action.pushBack(RemoveSelf::create());
		seq = Sequence::create(action);
		spr->runAction(seq);
		this->addChild(spr);
	}


}

void Mode_Spacebattle::Debris(int target)
{
	Sequence *seq ;
	Vector <FiniteTimeAction *> action;
	for (int i = 0; i < 16; i++)
	{
		action.clear();
		int r = 1+(rand() % 7);
		Sprite *spr = Sprite::create("art/debris.png", Rect(4*(r%4), 4*(r/4), 4, 4));
		spr->runAction(RepeatForever::create(RotateBy::create(0.2F, 90)));
		spr->setScale(2);
		Vec2 p = m_bsprites[target]->getPosition();
		p.x -= 16;
		p.y -= 16;
		int x = (rand() % 128);
		int y = (rand() % 128);
		p.x += x / 4; x = x - 64;
		p.y += y / 4; y = y - 64;
		spr->setPosition(p);
		spr->setColor(Color3B(255, 255, 255));
		spr->setVisible(false);
		action.pushBack(DelayTime::create(0.8F));
		action.pushBack(ToggleVisibility::create());
		action.pushBack(TintTo::create(0.8F, 55, 0, 0));
		seq = Sequence::create(action);
		spr->runAction(seq);
		action.clear();

		action.pushBack(DelayTime::create(0.8F));
		action.pushBack(MoveBy::create(0.8F, Vec2(x, y)));
		action.pushBack(RemoveSelf::create());
		seq = Sequence::create(action);
		spr->runAction(seq);
		this->addChild(spr);
	}
}

void Mode_Spacebattle::GenerateExplosion(int target, float t)
{
	Sprite *spr = Sprite::create("art/battleeffects.png", cocos2d::CCRect(0, 32, 64, 64));
	Animation *an = cocos2d::Animation::create();
	for (int i = 0; i < 8; i++)
	{
		SpriteFrame *sf = cocos2d::SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect((64 * i), 32, 64, 64));
		an->addSpriteFrame(sf);
	}
	an->setDelayPerUnit(0.1F);
	
	spr->setVisible(false);
	spr->setPosition(m_bsprites[target]->getPosition());
	Vector <FiniteTimeAction *> action1;

//	m_bsprites[target]->setVisible(false);
	if (getShip(target)->getShip()->getExplode())
	{
		Debris(target);
		action1.pushBack(DelayTime::create(t));
		action1.pushBack(ToggleVisibility::create());
	}
	else
	{
		action1.pushBack(DelayTime::create(t));
		action1.pushBack(TintTo::create(0.0F,50,50,50));
	}

	m_bsprites[target]->runAction(Sequence::create(action1));

	Animate *ani = Animate::create(an);
	Vector <FiniteTimeAction *> action;
	action.pushBack(DelayTime::create(t));
	action.pushBack(ToggleVisibility::create());
	action.pushBack(ani);
	action.pushBack(RemoveSelf::create());
	action.pushBack(CallFunc::create(this,SEL_CallFunc(&Mode_Spacebattle::AttackConcluded)));
	Sequence *seq = Sequence::create(action);
	spr->runAction(seq);
	this->addChild(spr);

	Sparks(target, 16);
}


void Mode_Spacebattle::AttackConcluded()
{
	Playership *playership = (Playership*)m_player->getShip();
	char buffer[4];
	m_ticking = true;

	sprintf(buffer, "%d", playership->getHP());
	std::string str = ""; str.append(buffer);
	m_labels[0]->setString(str);
	m_segbars[0]->setVariable(playership->getHP()); 
	sprintf(buffer, "%d", playership->getFuel());
	str = ""; str.append(buffer);
	m_segbars[1]->setVariable(playership->getFuel());
	m_labels[1]->setString(str);
	sprintf(buffer, "%d", m_ordinance);
	str = ""; str.append(buffer);
	m_labels[2]->setString(str);

	sprintf(buffer, "%d", m_enemies[m_target]->getShip()->getHP());
	str = ""; str.append(buffer);
	m_labels[3]->setString(str);

	ProgressCheck();
}

void Mode_Spacebattle::generateBeam(AS_effect *effect, int target, int origin, float mod)
{
	//find origin pos
	//find target pos
	Vec2 t;
	Vec2 o;
	if (origin > m_numallies)
	{
		t = m_bsprites[target]->getPosition();
		o = m_bsprites[origin]->getPosition();
		o.x -= 16;
	}
	else
	{
		t = m_bsprites[target]->getPosition();
		o = m_bsprites[origin]->getPosition();
		o.x += 16;
	}

	//calculate angle
	float a = std::atan2(t.x - o.x, t.y - o.y);
	//generate beamsprites along the path
//	sprite[index]->setRotation((a * 57) - 90);
	float d = t.getDistance(o);

	int index = 0; int max = 1+ (d / 32);
	cocos2d::Sprite **sprite = new Sprite *[max];
	Animation *an = cocos2d::Animation::create();
	for (int i = 0; i < 4; i++)
	{
		//add four frames
		SpriteFrame *sf = cocos2d::SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect(effect->m_graphicpos[0].x * 8+(16*i), effect->m_graphicpos[0].y * 8, 16, 8));
		an->addSpriteFrame(sf);
	}
	an->setDelayPerUnit(0.05F);

	while (index < max)
	{
		sprite[index] = cocos2d::Sprite::create("art/battleeffects.png", cocos2d::CCRect(effect->m_graphicpos[0].x * 8, effect->m_graphicpos[0].y * 8, 16, 8));
		Vec2 p = Vec2(0, 32 * index * 1);
		p.rotate(Vec2(0, 0), a*-1);

		sprite[index]->setAnchorPoint(cocos2d::Vec2(0, 0.5));
		if (index == max - 1)
		{
			float v = d - (index * 32); v = v / 16;
			sprite[index]->setScaleX(v);
		}
		else
		{
			sprite[index]->setScaleX(2);
		}	
		sprite[index]->setScaleY(mod);
		sprite[index]->setPosition(p+o);
		sprite[index]->setRotation((a * 57)-90);
		Animate *ani = Animate::create(an);
		Vector <FiniteTimeAction *> action;
		action.pushBack(Repeat::create(ani, 4));
		action.pushBack(RemoveSelf::create());
		Sequence *seq = Sequence::create(action);
		sprite[index]->runAction(seq);
		this->addChild(sprite[index]);
		index++;
	}
	delete [] sprite;
}

void Mode_Spacebattle::generateShield(Starship *ship, Sprite *p)
{
	if (ship->getDefence(0) > 0)
	{
		Sprite *spr = Sprite::create();
		spr->setPosition(p->getPosition());
		this->addChild(spr);
		//scale to fit around ship
		float v = p->getContentSize().width/8;
		spr->setScale(v);
		//play animation
		Animation *an = Animation::create();
		for (int i = 0; i < 4; i++)
		{
			SpriteFrame *sf = SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect(ship->getDefenceGraphic(0).x * 8 + (16 * i), ship->getDefenceGraphic(0).y * 8, 16, 16));
			an->addSpriteFrame(sf);
		}
		an->setDelayPerUnit(0.05F);
		Animate *ani = Animate::create(an);
		Vector <FiniteTimeAction *> action;
		action.pushBack(Repeat::create(ani, 4));
		action.pushBack(RemoveSelf::create());
		Sequence *seq = Sequence::create(action);
		spr->runAction(seq);
	}
}

void Mode_Spacebattle::generateImpact(AS_effect *effect, int target)
{
	Animation *an = cocos2d::Animation::create();
	for (int i = 0; i < 4; i++)
	{
		//add four frames
		SpriteFrame *sf = cocos2d::SpriteFrame::create("art/battleeffects.png", cocos2d::CCRect(effect->m_graphicpos[1].x * 8 + (8 * i), effect->m_graphicpos[1].y * 8, 8, 8));
		an->addSpriteFrame(sf);
	}
	an->setDelayPerUnit(0.2F);
	Sprite *sprite = Sprite::create();
	sprite->setScale(2);
	sprite->setPosition(m_bsprites[target]->getPosition());
	Animate *ani = Animate::create(an);
	Vector <FiniteTimeAction *> action;
	action.pushBack(Repeat::create(ani, 1));
	action.pushBack(RemoveSelf::create());
	Sequence *seq = Sequence::create(action);
	this->addChild(sprite);
	sprite->runAction(seq);
}

int Mode_Spacebattle::LaserDamageCalc(int target, Ability_Space *as, float mod)
{
	if (target == 0)
	{
		if (as->getEffect()->m_type == AE_damage)
		{
			AS_damageeffect *ae = (AS_damageeffect*)as->getEffect();
			float d = m_player->getShip()->getDefence(0);
			d = d / ae->m_penetration;
			int damage = (ae->m_damage*mod) - d;
			if (damage < 0){ damage = 0; }
			m_player->getShip()->Damage(damage);
			return (ae->m_damage*mod) - d;
		}
	}
	else
	{
		if (target <= m_numallies)
		{
			int t = target - 1;
			if (as->getEffect()->m_type == AE_damage)
			{
				AS_damageeffect *ae = (AS_damageeffect*)as->getEffect();
				float d = m_allies[t]->getShip()->getDefence(0);
				d = d / ae->m_penetration;
				int damage = (ae->m_damage*mod) - d;
				if (damage < 0){ damage = 0; }
				m_allies[t]->getShip()->Damage(damage);
				return (ae->m_damage*mod) - d;
			}
		}
		else
		{
			int t = target - 1 - m_numallies;
			if (as->getEffect()->m_type == AE_damage)
			{
				AS_damageeffect *ae = (AS_damageeffect*)as->getEffect();
				float d = m_enemies[t]->getShip()->getDefence(0);
				d = d / ae->m_penetration;
				int damage = (ae->m_damage*mod) - d;
				if (damage < 0){ damage = 0; }
				m_enemies[t]->getShip()->Damage(damage);
				return (ae->m_damage*mod) - d;
			}
		}
	}
	return 0;


}

void  Mode_Spacebattle::ProgressCheck()
{
	int h = 0;
	for (int i = 0; i < m_numenemies; i++)
	{
		if (m_enemies[i] != NULL)
		{
			h += m_enemies[i]->getShip()->getHP();
		}
	}
	if (h == 0)
	{
		m_player->ConsumeOrdinance(m_ordinanceused*-1);
		Director::sharedDirector()->getActionManager()->removeAllActions();
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(0);
		Director::sharedDirector()->replaceScene(scene);


	}
	if (m_player->getShip()->getHP() <= 0)
	{
		Director::sharedDirector()->getActionManager()->removeAllActions();
		Scene *scene = ModeGameOver::createScene(NULL);
		GameManager::getInstance()->getHandler()->MissionGameOver();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void Mode_Spacebattle::Save(FILE *file)
{
	char v = 2;
	fwrite((void*)&v, sizeof(char), 1, file);
	//save player
	m_player->SaveState(file);

	//save num enemies
	fwrite((void*)&m_numenemies, sizeof(int), 1, file);
	//save enemies
	for (int i = 0; i < m_numenemies; i++)
	{
		NPCship *ship = (NPCship*)m_enemies[i]->getShip();
		ship->Save(file);
		m_enemies[i]->SaveState(file);
	}
}

void Mode_Spacebattle::Setup(CBReader *reader)
{
	//load player
	m_ticking = true;
	m_player = new StarshipBattler(GameManager::getInstance()->getPlayer()->getShip());
	m_player->LoadState(reader);
	//load num enemies
	m_numallies = 0;
	m_numenemies = reader->ReadBInt();
	//load enemies
	m_enemies = new StarshipBattler*[m_numenemies];
	for (int i = 0; i < m_numenemies; i++)
	{
		NPCship *ship = new NPCship();
		ship->Load(reader);
		m_enemies[i] = new StarshipBattler(ship);
		m_enemies[i]->LoadState(reader);
	}

	CommonSetup();
}

cocos2d::Scene  *Mode_Spacebattle::createLoadScene(CBReader *reader)
{
	Scene *scene = Scene::create();

	Mode_Spacebattle *spacebattle = Mode_Spacebattle::create();
	spacebattle->Setup(reader);
	scene->addChild(spacebattle,0,100);

	return scene;
}

void Mode_Spacebattle::CommonSetup()
{


	//setup sprites
	int s = 1 + m_numenemies + m_numallies;
	m_bsprites = new cocos2d::Sprite *[s];
	m_bsprites[0] = cocos2d::CCSprite::create("art/battlesprites.png", cocos2d::CCRect(m_player->getShip()->getGraphicpos().x, m_player->getShip()->getGraphicpos().y, m_player->getShip()->getGraphicsize().x, m_player->getShip()->getGraphicsize().y));
	
	Vector <FiniteTimeAction *> action;
	action.pushBack(MoveBy::create(1.5F, Vec2(0, 4)));
	action.pushBack(MoveBy::create(1.5F, Vec2(0,-4)));
	Sequence *seq = Sequence::create(action);
	m_bsprites[0]->runAction(RepeatForever::create(seq));

	if (m_numallies > 0)
	{
		for (int i = 1; i < s -m_numenemies; i++)
		{
			int d = i - 1;
			m_bsprites[i] = cocos2d::CCSprite::create("art/battlesprites.png", cocos2d::CCRect(m_allies[d]->getShip()->getGraphicpos().x, m_allies[d]->getShip()->getGraphicpos().y, m_allies[d]->getShip()->getGraphicsize().x, m_allies[d]->getShip()->getGraphicsize().y));

		}


	}

	for (int i = 0; i < m_numallies + 1; i++)
	{
		int d = i - 1;
		//set position for friendly ships
		m_bsprites[i]->setPosition(getPos(i, false));
	}

	for (int i = 1 + m_numallies; i < s; i++)
	{
		int d = i - 1 - m_numallies;
		m_bsprites[i] = cocos2d::CCSprite::create("art/battlesprites.png", cocos2d::CCRect(m_enemies[d]->getShip()->getGraphicpos().x, m_enemies[d]->getShip()->getGraphicpos().y, m_enemies[d]->getShip()->getGraphicsize().x, m_enemies[d]->getShip()->getGraphicsize().y));

		//set position for enemy ships
		m_bsprites[i]->setPosition(getPos(d, true));
		m_bsprites[i]->setScaleX(-1);
		if (m_enemies[d]->getShip()->getHP() <= 0)
		{
			m_bsprites[i]->setVisible(false);
		}

		Vector <FiniteTimeAction *> action;
		action.pushBack(MoveBy::create(1.5F, Vec2(0, 4)));
		action.pushBack(MoveBy::create(1.5F, Vec2(0, -4)));
		Sequence *seq = Sequence::create(action);
		m_bsprites[i]->runAction(RepeatForever::create(seq));
	}

	for (int i = 0; i < s; i++)
	{
		this->addChild(m_bsprites[i]);
		m_bsprites[i]->setScale(2);
	}
	//connect all the labels to their values

	//hull
	Playership *playership = (Playership*)m_player->getShip();
	char buffer[4];
	sprintf(buffer, "%d", playership->getHP());
	std::string str = ""; str.append(buffer);
	m_labels[0]->setString(str);

	sprintf(buffer, "%d", playership->getFuel());
	str = ""; str.append(buffer);
	m_labels[1]->setString(str);

	m_segbars[0]->Setup(0, 192, playership->getHPmax(), playership->getHP());
	m_segbars[1]->Setup(2, 192, playership->getMaxFuel(), playership->getFuel());
	m_ordinance = playership->getOrdinance();
	m_ordinanceused = 0;
	sprintf(buffer, "%d", m_ordinance);
	str = ""; str.append(buffer);
	m_labels[2]->setString(str);

	sprintf(buffer, "%d", m_enemies[m_target]->getShip()->getHP());
	str = ""; str.append(buffer);
	m_labels[3]->setString(str);

	//generate ability buttons
	SetupButtons();

	m_bar->setGreen(m_player->getCommand() / 100);
	Abilitychecks();

	m_tsprite = cocos2d::Sprite::create("art/targeter.png");
	m_tsprite->setScale(2);
	m_tsprite->setPosition(m_bsprites[1 + m_numallies + m_target]->getPosition());
	this->addChild(m_tsprite);
	this->setTouchEnabled(true);




}