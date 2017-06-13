#include "Mode_Awayteam.h"
#include "ReferenceLibrary.h"
#include "GameManager.h"
USING_NS_CC;

void OfficerFrame::Setup(Officer *officer,bool battleuse)
{
	if (officer == NULL)
	{
		officer = new Officer(ReferenceLibrary::getInstance()->getOfficer(0));
		officer->setName(ReferenceLibrary::getName("ensign"));
	}
	//setup portrait
	m_portrait = Sprite::create("art/officers.png",Rect(officer->getDefinition()->getPortrait().x*128,officer->getDefinition()->getPortrait().y*128,128,128));
	m_portrait->setAnchorPoint(Vec2(0.5F, 0.5F));
	m_portrait->setPosition(Vec2(0, 56));

	m_officer = new Officer_Away(officer, m_portrait);
	this->addChild(m_portrait);
	//m_portrait->setScale(2);
	//setup frame
	m_frame = Sprite::create("art/awayframes.png", Rect(0, 0, 64, 128));
	this->addChild(m_frame);
	m_frame->setPosition(0, 0);
	m_frame->setScale(2);

	//setup label
	m_label = Label::createWithBMFont("art/font.fnt", m_officer->getOfficer()->getName());
	m_label->setPosition(4, -8);
	m_label->setSystemFontSize(18);
	m_label->setWidth(120);
	this->addChild(m_label);
	//setup segbars
	m_bars[0] = Segbar::create();
	m_bars[1] = Segbar::create();
	m_bars[0]->Setup(0, 96, m_officer->getHealth(), m_officer->getHealth());
	m_bars[1]->Setup(2, 96., 5, m_officer->getEnergy());
	char buffer[4];
	itoa(m_officer->getHealth(), &buffer[0], 10);
	std::string str = ""; str.append(buffer);
	m_numbers[0] = Label::createWithBMFont("art/font.fnt", str);
	itoa(m_officer->getEnergy(), &buffer[0], 10);
	str = ""; str.append(buffer);
	m_numbers[1] = Label::createWithBMFont("art/font.fnt", str);
	for (int i = 0; i < 2; i++)
	{
		m_bars[i]->setScaleY(2);
		m_bars[i]->setPosition(-56, -64+(i * -32));
		m_numbers[i]->setPosition(40, -64 + (i * -32));
		m_numbers[i]->setSystemFontSize(20);
		this->addChild(m_bars[i]);
		this->addChild(m_numbers[i]);
	}
	if (battleuse == true)
	{
		for (int i = 0; i < 4; i++)
		{
			m_portrait->addChild(m_officer->getStatusIcon(i));
			///place status icons properly
			m_officer->getStatusIcon(i)->setPosition(16, 16 + 32 * i);
			m_officer->getStatusIcon(i)->setZOrder(0);
			//then later make them invisible
			m_officer->getStatusIcon(i)->setVisible(false);
		}
	}



}

void OfficerFrame::Setup(Officer_Away *officer, bool battleuse)
{

	//setup portrait
	m_portrait = Sprite::create("art/officers.png", Rect(officer->getOfficer()->getDefinition()->getPortrait().x * 128, officer->getOfficer()->getDefinition()->getPortrait().y * 128, 128, 128));
	m_portrait->setAnchorPoint(Vec2(0.5F, 0.5F));
	m_portrait->setPosition(Vec2(0, 56));
	
	m_officer = officer;
	m_officer->setSprite(m_portrait);
	this->addChild(m_portrait);
	//m_portrait->setScale(2);
	//setup frame
	m_frame = Sprite::create("art/awayframes.png", Rect(0, 0, 64, 128));
	this->addChild(m_frame);
	m_frame->setPosition(0, 0);
	m_frame->setScale(2);

	//setup label
	m_label = Label::createWithBMFont("art/font.fnt", m_officer->getOfficer()->getName());
	m_label->setPosition(4, -8);
	m_label->setSystemFontSize(18);
	m_label->setWidth(120);
	this->addChild(m_label);
	//setup segbars
	m_bars[0] = Segbar::create();
	m_bars[1] = Segbar::create();
	m_bars[0]->Setup(0, 96, m_officer->getMaxHP(), m_officer->getHealth());
	m_bars[1]->Setup(2, 96.,m_officer->getMaxEP(), m_officer->getEnergy());
	char buffer[4];
	itoa(m_officer->getHealth(), &buffer[0], 10);
	std::string str = ""; str.append(buffer);
	m_numbers[0] = Label::createWithBMFont("art/font.fnt", str);
	itoa(m_officer->getEnergy(), &buffer[0], 10);
	str = ""; str.append(buffer);
	m_numbers[1] = Label::createWithBMFont("art/font.fnt", str);
	for (int i = 0; i < 2; i++)
	{
		m_bars[i]->setScaleY(2);
		m_bars[i]->setPosition(-56, -64 + (i * -32));
		m_numbers[i]->setPosition(40, -64 + (i * -32));
		m_numbers[i]->setSystemFontSize(20);
		this->addChild(m_bars[i]);
		this->addChild(m_numbers[i]);
	}
	if (battleuse == true)
	{
		for (int i = 0; i < 4; i++)
		{
			m_portrait->addChild(m_officer->getStatusIcon(i));
			///place status icons properly
			m_officer->getStatusIcon(i)->setPosition(16, 16 + 32 * i);
			m_officer->getStatusIcon(i)->setZOrder(0);
			//then later make them invisible
			m_officer->getStatusIcon(i)->setVisible(false);
		}
	}
}

void OfficerFrame::RedrawBars()
{
	m_bars[0]->setVariable(m_officer->getHealth());
	m_bars[1]->setVariable(m_officer->getEnergy());
	char buffer[4];
	itoa(m_officer->getHealth(), &buffer[0], 10);
	m_numbers[0]->setString(buffer);
	itoa(m_officer->getEnergy(), &buffer[0], 10);
	m_numbers[1]->setString(buffer);
	if (m_officer->getHealth() < 0)
	{
		m_portrait->setVisible(false);
	}

}

Mode_Awayteam::~Mode_Awayteam()
{

	
}

cocos2d::Scene *Mode_Awayteam::DebugcreateScene()
{
	Mode_Awayteam *mode = Mode_Awayteam::create();

	//setup simple battle
	Officer *officers[3]; 
	officers[0] = new Officer(ReferenceLibrary::getInstance()->getOfficer(1));
	officers[1] = NULL; officers[2] = NULL;
	//captain as officer
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/enemies.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer =cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);

	Enemy_Away **enemies;
	enemies = new Enemy_Away *[6];
	for (int i = 0; i < 6; i++)
	{
		enemies[i] = new Enemy_Away(0, cbr);
	}
	Scene *scene = Scene::create();
	scene->addChild(mode);
	mode->Setup(1, officers, enemies,NULL);

	delete cbr;

	return scene;
}

Scene* Mode_Awayteam::createScene(int background, Officer* officers[3],Enemy_Away *enemies[6])
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Mode_Awayteam::create();

//	layer->Setup(background, officers, rows, numinrows);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

Scene* Mode_Awayteam::createScene(Officer *officers[3], MissionNode *stage, Potion **potions)
{
	Mode_Awayteam *mode = Mode_Awayteam::create();
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/missions/enemies.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);

	while (s == 0)
	{
		buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	}
	length = s;

	CBReader *cbr = new CBReader(buffer, length);
	Enemy_Away **enemies;
	enemies = new Enemy_Away *[6];
	for (int i = 0; i < 6; i++)
	{
		if (stage->m_variables[i] == -1)
		{
			enemies[i] = NULL;
		}
		else
		{
			enemies[i] = new Enemy_Away(stage->m_variables[i], cbr);
		}

	}
	Scene *scene = Scene::create();
	scene->addChild(mode);
	mode->Setup(stage->m_variables[6], officers, enemies,potions);

	delete cbr;

	return scene;

}

// on "init" you need to initialize your instance
bool Mode_Awayteam::init()
{
	m_host = NULL;
	m_active = true;
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	m_turnindex = -1;
	m_mode = 0;
	return true;
}

void Mode_Awayteam::Setup(int background, Officer_Away *officers[3], Enemy_Away *enemies[6], Potion **potions)
{
	Size visibleSize = Size(800, 640);
	Size s = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_potions = potions;
	//set background
	std::string str = "art/background";
	char buffer[3];
	itoa(background, &buffer[0], 10);
	str.append(buffer); str.append(".png");
	Sprite *sprite = Sprite::create(str);
	this->addChild(sprite);
	sprite->setScale(2);
	sprite->setPosition(origin.x + 400, origin.y + 448);

	for (int i = 0; i < 3; i++)
	{
		officers[i]->SetupIcons();
		m_frames[i] = OfficerFrame::create();
		m_frames[i]->Setup(officers[i],true);
		this->addChild(m_frames[i]);

		m_frames[i]->setPosition(origin.x + 64 + (128 * i), origin.y + 128);
	}


	for (int i = 0; i < 6; i++)
	{
		m_enemies[i] = enemies[i];
		if (m_enemies[i] != NULL)
		{
			m_enemies[i]->BuildSprite();
			int x = i % 3;
			int y = i / 3;
			x = x * 200;
			y = y * 120;
			x = x + origin.x + 198;
			y = y + origin.y + 328;
			y = y + (m_enemies[i]->getSprite()->getContentSize().height/2);
			if (i >= 3)
			{
				x = x*0.8F; x = x + 100;
			}
			m_enemies[i]->getSprite()->setAnchorPoint(Vec2(0.5F, 0.5F));
			m_enemies[i]->getSprite()->setPosition(x, y);

			this->addChild(m_enemies[i]->getSprite());
			for (int j = 0; j < 4; j++)
			{
				m_enemies[i]->getStatusIcon(j)->setScale(0.4F);
				m_enemies[i]->getStatusIcon(j)->setPosition(Vec2(0, 8 + 13 * j));
				m_enemies[i]->getSprite()->addChild(m_enemies[i]->getStatusIcon(j));
				m_enemies[i]->getStatusIcon(j)->setVisible(false);
			}
		}
	}

	//build buttons
	for (int i = 0; i < 5; i++)
	{
		m_buttons[i] = ui::Button::create("art/longbutton0.png", "art/longbutton1.png", "art/longbutton2.png");
		m_buttons[i]->setPosition(Vec2(origin.x + 592, origin.y + 230 - (51 * i)));
		m_buttons[i]->setScaleY(0.95F);
		//		m_buttons[i]->setScaleX(2.0F);
		m_buttons[i]->setTitleFontSize(32);
		m_buttons[i]->addClickEventListener(CC_CALLBACK_1(Mode_Awayteam::ButtonCallback, this));
		this->addChild(m_buttons[i]);
	}
	m_buttons[0]->setTitleText("Attack");
	m_buttons[1]->setTitleText("Ability");
	m_buttons[2]->setTitleText("Defend");
	m_buttons[3]->setTitleText("Item");
	m_buttons[4]->setVisible(false);
	m_buttons[4]->setTitleText("Cancel");

	//build prompt
	m_promptframe = Sprite::create("art/backbox2.png");
	m_promptframe->setScale(4);
	m_promptframe->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height - 64)));
	this->addChild(m_promptframe);

	m_prompt = Label::createWithBMFont("art/font.fnt", "words words words words words words words");
	m_prompt->setWidth(256);
	m_prompt->setAnchorPoint(Vec2(0.0F, 0.5F));
	m_prompt->setSystemFontSize(16);
		m_prompt->setPosition(Vec2(4, 16));
		m_prompt->setScale(0.25F);
	m_promptframe->addChild(m_prompt);
	m_promptframe->setVisible(false);
	ProgressTurn();
	this->setTouchEnabled(true);
}

void  Mode_Awayteam::Setup(int background, Officer* officers[3], Enemy_Away *enemies[6], Potion **potions)
{
	m_potions = potions;
	Size visibleSize = Size(800, 640);
	Size s = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//set background
	std::string str = "art/background";
	char buffer[3];
	itoa(background, &buffer[0], 10);
	str.append(buffer); str.append(".png");
	Sprite *sprite = Sprite::create(str);
	this->addChild(sprite);
	sprite->setScale(2);
	sprite->setPosition(origin.x + 400, origin.y + 448);

	for (int i = 0; i < 3; i++)
	{
		m_frames[i] = OfficerFrame::create();
		m_frames[i]->Setup(officers[i]);
		this->addChild(m_frames[i]);

		m_frames[i]->setPosition(origin.x + 64 + (128 * i), origin.y + 128);
	}


	for (int i = 0; i < 6; i++)
	{
		m_enemies[i] = enemies[i];
		if (m_enemies[i] != NULL)
		{
			int x = i % 3;
			int y = i/ 3;
			x = x * 200;
			y = y * 120;
			x = x + origin.x + 198;
			y = y + origin.y + 328;
			if (i >= 3)
			{
				x = x*0.8F; x = x + 100;
			}
			m_enemies[i]->getSprite()->setAnchorPoint(Vec2(0.5F, 0.5F));
			m_enemies[i]->getSprite()->setPosition(x, y);

			this->addChild(m_enemies[i]->getSprite());
			for (int j = 0; j < 4; j++)
			{
				m_enemies[i]->getStatusIcon(j)->setScale(0.4F);
				m_enemies[i]->getStatusIcon(j)->setPosition(Vec2(0, 8+13 * j));
				m_enemies[i]->getSprite()->addChild(m_enemies[i]->getStatusIcon(j));
				m_enemies[i]->getStatusIcon(j)->setVisible(false);
			}
			if (m_enemies[i]->getHealth() <= 0)
			{
				m_enemies[i]->getSprite()->setVisible(false);
			}
		}
	}

	//build buttons
	for (int i = 0; i < 5; i++)
	{
		m_buttons[i] = ui::Button::create("art/longbutton0.png", "art/longbutton1.png", "art/longbutton2.png");
		m_buttons[i]->setPosition(Vec2(origin.x + 592, origin.y + 230 - (51 * i)));
		m_buttons[i]->setScaleY(0.95F);
//		m_buttons[i]->setScaleX(2.0F);
		m_buttons[i]->setTitleFontSize(32);
		m_buttons[i]->addClickEventListener(CC_CALLBACK_1(Mode_Awayteam::ButtonCallback, this));
		this->addChild(m_buttons[i]);
	}
	m_buttons[0]->setTitleText("Attack");
	m_buttons[1]->setTitleText("Ability");
	m_buttons[2]->setTitleText("Defend");
	m_buttons[3]->setTitleText("Item");
	m_buttons[4]->setVisible(false);
	m_buttons[4]->setTitleText("Cancel");

	//build prompt
	m_promptframe = Sprite::create("art/backbox2.png");
	m_promptframe->setScale(4);
	m_promptframe->setPosition(Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height - 32)));
	this->addChild(m_promptframe);

	m_prompt = Label::createWithBMFont("art/font.fnt", "words words words words words words words");
	m_prompt->setWidth(256);
	m_prompt->setAnchorPoint(Vec2(0.0F, 0.5F));
	m_prompt->setSystemFontSize(32);
//	m_prompt->setPosition(Vec2(4, m_promptframe->getBoundingBox().size.height / 4));
//	m_prompt->setScale(0.25F);
	m_promptframe->addChild(m_prompt);
	m_promptframe->setVisible(false);
	ProgressTurn();
	this->setTouchEnabled(true);
}

void Mode_Awayteam::ProcessCommand(int index)
{
	int m = m_mode;
	if (m == 0)
	{
		//select from main menu
		switch (index)
		{
		case 0:
			//attack
			m_promptframe->setVisible(true);
			m_prompt->setString("Select enemy target");
			m_buttons[4]->setVisible(true);
			m_mode = 1;
			break;
		case 1:
			//ability
			AbilityPanel();
			break;
		case 2:
			//defend
			m_frames[m_turnindex]->m_officer->Defend();
			ProgressTurn();
			break;
		case 3:
			//items
			PotionPanel();
			break;


		}
	}
	if (m== 1)
	{
		switch (index)
		{
			case 4:
			m_buttons[4]->setVisible(false);
			m_promptframe->setVisible(false);
			m_mode = 0;
			break;
		}

	}
	if (m== 10)
	{
		//select ability menu
		switch (index)
		{
		case 0:
			ProcessAbility(index);
			break;
		case 1:
			ProcessAbility(index);
			break;
		case 2:
			ProcessAbility(index);
			break;

		case 3:
			ProcessAbility(index);
			break;

		case 4:
			MainPanel();
			break;
		}
	}
	if (m== 20)
	{
		switch (index)
		{
		case 0:
			if (m_potions[index] != NULL)
			{
				ProcessPotion(index);
			}
			break;

		case 1:
			if (m_potions[index] != NULL)
			{
				ProcessPotion(index);
			}
			break;


		case 2:
			if (m_potions[index] != NULL)
			{
				ProcessPotion(index);
			}
			break;

		case 3:
			MainPanel();
			break;
		}
	}
	if (m > 20)
	{
		if (m_mode - 21 == index)
		{
			UsePotion(index);
		}
	}
	if (index == 4)
	{
		MainPanel();
	}
}

void Mode_Awayteam::UsePotion(int index)
{
	m_frames[m_turnindex]->m_officer->ApplyAbility(NULL, m_potions[index]->m_effect->getEffect(0), this);
	m_potions[index] = NULL;
	m_frames[m_turnindex]->RedrawBars();
	MainPanel();
	m_mode = 0;
	ProgressTurn();
}

void Mode_Awayteam::ProcessPotion(int index)
{
	//check for recovery item
	if (m_potions[index] != NULL)
	{
		if (m_potions[index]->m_effect->getEffect(0)->m_type == AAT_RECOVER)
		{
			AA_Recover *recover = (AA_Recover*)m_potions[index]->m_effect->getEffect(0);
			if (recover->m_affects == AMT_HEALTH && m_frames[m_turnindex]->m_officer->getHealth()<m_frames[m_turnindex]->m_officer->getMaxHP())
			{
				ActivatePotion(index);
			}
			if (recover->m_affects == AMT_ENERGY && m_frames[m_turnindex]->m_officer->getEnergy() < m_frames[m_turnindex]->m_officer->getMaxEP())
			{
				ActivatePotion(index);
			}
		}
		if (m_potions[index]->m_effect->getEffect(0)->m_type == AAT_BUFF)
		{
			ActivatePotion(index);
		}
	}
}

void Mode_Awayteam::ActivatePotion(int index)
{
	//create prompt
	m_prompt->setString("Tap again to use the " + m_potions[index]->m_name);
	m_promptframe->setVisible(true);
	m_mode = 20 + index + 1;

}

void Mode_Awayteam::ProcessAbility(int index)
{
	Ability_Away *ability = m_frames[m_turnindex]->m_officer->getOfficer()->getAbility(index);
	//decipher aiming rules
	if (ability != NULL)
	{
		int energy = m_frames[m_turnindex]->m_officer->getEnergy();
		if (energy >= ability->getCost())
		{
			m_mode = 10 + index+1;
			AA_Effect *ae = ability->getEffect(0);
			m_promptframe->setVisible(true);
			switch (ae->m_targetrule)
			{
			case AATR_ENEMY:
				m_prompt->setString("Select enemy target");
				break;

			case AATR_ALLY:
				m_prompt->setString("Select an ally");
				break;

			case AATR_SELF:
				m_prompt->setString("touch ability user to activate ability");
				break;

			case AATR_ENEMIES:
				m_prompt->setString("select any enemy target to activate group ability");
				break;

			case AATR_ALLIES:
				m_prompt->setString("select any ally to activate group ability");
				break;
			}
			m_targetrule = ae->m_targetrule;
		}

	}
}



void Mode_Awayteam::AbilityPanel()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_frames[m_turnindex]->m_officer->getOfficer()->getAbility(i) != NULL)
		{
			m_buttons[i]->setVisible(true);
			Ability_Away *ability = m_frames[m_turnindex]->m_officer->getOfficer()->getAbility(i);
			std::string str = ability->getName()+" ";
			char buffer[2];
			itoa(ability->getCost(), &buffer[0], 10);
			str.append(buffer);
			m_buttons[i]->setTitleText(str);
			if (m_frames[m_turnindex]->m_officer->getEnergy() < ability->getCost() || m_frames[m_turnindex]->m_officer->getCooldown(i)>0)
			{
				m_buttons[i]->setEnabled(false);
				m_buttons[i]->setBright(false);
			}
			else
			{
				m_buttons[i]->setEnabled(true);
				m_buttons[i]->setBright(true);

			}

		}
		else
		{
			m_buttons[i]->setVisible(false);
		}

	}
	m_buttons[4]->setVisible(true);
	m_mode = 10;
}

void Mode_Awayteam::PotionPanel()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_potions[i] != NULL)
		{
			m_buttons[i]->setTitleText(m_potions[i]->m_name);
		}
		else
		{
			m_buttons[i]->setVisible(false);
		}
	}
	m_buttons[3]->setVisible(true);
	m_buttons[3]->setBright(true);
	m_buttons[3]->setTitleText("Exit");
	m_mode = 20;
}

void Mode_Awayteam::MainPanel()
{
	m_promptframe->setVisible(false);
	for (int i = 0; i < 4; i++)
	{
		m_buttons[i]->setVisible(true);
		m_buttons[i]->setEnabled(true);
		m_buttons[i]->setBright(true);

	}
	m_buttons[4]->setVisible(false);
	m_buttons[0]->setTitleText("Attack");
	m_buttons[1]->setTitleText("Ability");
	m_buttons[2]->setTitleText("Defend");
	m_buttons[3]->setTitleText("Item");
	m_mode = 0;
}

void Mode_Awayteam::ButtonCallback(Ref *pSender)
{
	if (m_active == true)
	{
		//run ability
		for (int i = 0; i < 5; i++)
		{
			if (pSender == m_buttons[i])
			{
				ProcessCommand(i);
				break;
			}
		}
	}
}


void Mode_Awayteam::ProgressTurn()
{
	m_active = true;
	for (int i = 0; i < 3; i++)
	{
		m_frames[i]->m_frame->setTextureRect(Rect(0, 0, 64, 128));
	}
	while (true)
	{
		m_turnindex++;
		if (m_turnindex>8)
		{
			m_turnindex = 0;
		}
		if (m_turnindex < 3)
		{
			if (m_frames[m_turnindex]->m_officer->getHealth()>0)
			{
				if (m_frames[m_turnindex]->m_officer->Update(m_promptframe,m_prompt))
				{
					RedrawBars();
				}
				if (m_frames[m_turnindex]->m_officer->CanAct())
				{
					m_mode = 0;
					m_frames[m_turnindex]->m_frame->setTextureRect(Rect(64, 0, 64, 128));
					for (int i = 0; i < 4; i++)
					{
						m_buttons[i]->setVisible(true);
						
					}
					m_buttons[0]->setTitleText("Attack");
					m_buttons[1]->setTitleText("Ability");
					m_buttons[2]->setTitleText("Defend");
					m_buttons[3]->setTitleText("Item");
					break;
				}
			}
		}
		else
		{
			if (m_enemies[m_turnindex - 3] != NULL)
			{
				if (m_enemies[m_turnindex - 3]->getHealth() > 0)
				{
					if (m_enemies[m_turnindex - 3]->CanAct())
					{
						m_enemies[m_turnindex - 3]->Update(m_promptframe, m_prompt);
						ProcessAI();
						break;
					}
				}
			}
		}

	}
	VictoryCheck();
}

void Mode_Awayteam::ProcessAI()
{
	int t = 0;
	int x[3][2];
	for (int i = 0; i < 3; i++)
	{
		x[i][0] = t;
		if (m_frames[i]->m_officer->getHealth()>0)
		{
			t += m_frames[i]->m_officer->getChance();
			x[i][1] = m_frames[i]->m_officer->getChance();
		}
		else
		{
			x[i][1] = 0;
		}
	}
	int r = rand() % t;
	int target = 0;
	for (int i = 0; i <3; i++)
	{
		if (r > x[i][0] && r<x[i][0]+x[i][1])
		{
			target = i;
			break;
		}
	}

	int ability = rand() % 2;
	if (ability == 0)
	{
		if (AI_Ability(target)==false)
		{
			Attack(m_enemies[m_turnindex - 3], m_frames[target]->m_officer);
		}
	}
	else
	{
		Attack(m_enemies[m_turnindex - 3], m_frames[target]->m_officer);
	}
	for (int i = 0; i < 3; i++)
	{
		m_frames[target]->RedrawBars();
	}

}

bool Mode_Awayteam::AI_Ability(int target)
{
	//pick an ability
	int c = 0;
	for (int i = 0; i < 3; i++)
	{
		if (m_enemies[m_turnindex - 3]->getData()->m_abilities[i] != NULL)
		{
			c++;
		}
		else
		{
			break;
		}
	}
	if (c == 0)
	{
		return false;
	}
	int r = 0;
	if (c > 1)
	{
		r = rand() % c;
	}
	
	//can we use this ability?
	if (m_enemies[m_turnindex - 3]->getEnergy() >= m_enemies[m_turnindex - 3]->getData()->m_abilities[r]->getCost() &&
		m_enemies[m_turnindex-3]->getCooldown(r)==0)
	{
		//is this ability a targeted ability?
		Ability_Away *ability = m_enemies[m_turnindex - 3]->getData()->m_abilities[r];
		//if so use on target
		switch (ability->getEffect(0)->m_targetrule)
		{
			case AATR_SELF:
			//is this ability a self buff
				m_enemies[m_turnindex - 3]->ApplyAbility(m_enemies[m_turnindex - 3], ability->getEffect(0), this);
				m_enemies[m_turnindex - 3]->ModEnergy(ability->getCost());
				m_enemies[m_turnindex - 3]->setCooldown(r, ability->getCooldown());
				//use on self, yes
			break;

			case AATR_ENEMY:
				//use on target
				m_frames[target]->m_officer->ApplyAbility(m_enemies[m_turnindex - 3], ability->getEffect(0), this);
				m_enemies[m_turnindex - 3]->ModEnergy(ability->getCost());
				m_enemies[m_turnindex - 3]->setCooldown(r, ability->getCooldown());
				break;

			case AATR_ENEMIES:
				//use on all targets

				break;

			case AATR_ALLIES:
				//use on all allies
				break;
			case AATR_ALLY:
				//use on a random ally
				break;

		}
		return true;

	}

	return false;
}

void Mode_Awayteam::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	if (m_active == true)
	{
		Touch *touch = touches[0];
		for (int i = 0; i < 6; i++)
		{
			if (m_enemies[i] != NULL)
			{
				if (m_enemies[i]->getSprite()->getBoundingBox().containsPoint(touch->getLocation()) && m_enemies[i]->getSprite()->isVisible())
				{
					ProcessTouch(m_enemies[i], 3 + i);
					m_frames[m_turnindex]->RedrawBars();
				}
			}

		}

		for (int i = 0; i < 3; i++)
		{
			if (m_frames[i]->m_frame->getBoundingBox().containsPoint(touch->getLocation() - m_frames[i]->getPosition()) && m_frames[i]->m_officer->getHealth()>0)
			{
				ProcessTouch(m_frames[i]->m_officer, i);
				m_frames[m_turnindex]->RedrawBars();
			}
		}
	}
}

void Mode_Awayteam::ProcessTouch(Entity_Away *target, int index)
{
	if (m_mode == 1 && index>2)
	{
		Attack(m_frames[m_turnindex]->m_officer, m_enemies[index - 3]);
		for (int i = 0; i < 5; i++)
		{
			m_buttons[i]->setVisible(false);
		}
	}
	if (m_mode>10)
	{
		if (index < 3)
		{
			if (m_targetrule == AATR_ALLY && index != m_turnindex)
			{
				if (UseAbility(target,index))
				{
					RunAltAbility(target, index);
					
				}
			}
			if (m_targetrule == AATR_SELF && index == m_turnindex)
			{
				if (UseAbility(target, index))
				{
					RunAltAbility(target, index);
				}
			}
			if (m_targetrule == AATR_ALLIES)
			{
				if (UseAbility(target, index))
				{
					RunAltAbility(target, index);
				}
			}
		}
		else
		{
			if (m_targetrule == AATR_ENEMY || m_targetrule == AATR_ENEMIES)
			{
				if (UseAbility(target, index))
				{
					RunAltAbility(target, index);
		
				}
			}
		}

	}
}

void Mode_Awayteam::RunAltAbility(Entity_Away *target, int index)
{



}

void Mode_Awayteam::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
}

void Mode_Awayteam::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
}

void Mode_Awayteam::RedrawBars()
{
	for (int i = 0; i < 3; i++)
	{
		m_frames[i]->RedrawBars();

	}
}

bool Mode_Awayteam::UseAbility(Entity_Away *target,int index)
{
	Entity_Away *origin = NULL;
	if (m_turnindex < 3)
	{
		origin = m_frames[m_turnindex]->m_officer;

	}
	else
	{
		origin = m_enemies[m_turnindex - 3];
	}

	Sprite *sprite = NULL;
	if (index < 3)
	{
		sprite = m_frames[index]->m_portrait;
	}
	else
	{
		sprite = m_enemies[index - 3]->getSprite();
	}
	//check if the target is the right type
	Ability_Away *ability = m_frames[m_turnindex]->m_officer->getOfficer()->getAbility(m_mode - 11);
	AA_Effect *ae = ability->getEffect(0);
	if (ae->m_targettype != FT_ALL)
	{
		if (target->getType() == FT_ROBOT && ae->m_targettype == FT_ORGANIC)
		{
			m_prompt->setString("invalid target");
			m_mode = 10;
			return false;
		}
		if (target->getType() != ae->m_targettype && ae->m_targettype != FT_ORGANIC)
		{
			m_prompt->setString("invalid target");
			m_mode = 10;
			return false;
		}
	}

	if (ae->m_targetrule == AATR_ALLIES || ae->m_targetrule==AATR_ENEMIES)
	{
			if (index < 3)
			{
				//affect all officers
				for (int i = 0; i < 3; i++)
				{
					if (m_frames[i]->m_officer->getHealth()>0)
					{
						m_frames[i]->m_officer->ApplyAbility(origin,ae,this);
					}
				}
			}
			else
			{
				//affect all enemies
				for (int i = 0; i < 6; i++)
				{
					if (m_enemies[i]!=NULL)
					{
						if (m_enemies[i]->getHealth()>0)
						{
							m_enemies[i]->ApplyAbility(origin,ae,this);
						}

					}
				}
			}
	}
	else
	{
		target->ApplyAbility(origin,ae,this);


	}
	origin->setCooldown(index, ability->getCooldown());
	origin->ModEnergy(ability->getCost());
	return true;
}

void Mode_Awayteam::Attack(Entity_Away *attacker, Entity_Away *defender)
{
	m_active = false;
	m_promptframe->setVisible(false);
	Sprite *defendspr = defender->getSprite();
	Sprite *attackspr = attacker->getSprite();
	FieldAttack type = attacker->getAttackType();
	int attack = attacker->getAttack();
	int defence = defender->getDefence(type);

	attack = attack * 2;
	int damage = attack - defence;
	if (damage < 0)
	{
		damage = 0;
	}
	char buffer[4];
	itoa(damage, &buffer[0], 10);
	Label *number = Label::createWithBMFont("art/font.fnt", buffer);
	number->setColor(Color3B(255, 0, 0));
	number->setPosition(defender->getCenter());
	defendspr->addChild(number);
	Vector <FiniteTimeAction *> action;
	action.pushBack(MoveBy::create(0.8F, cocos2d::Vec2(0, 64)));
	//	action.pushBack(ani);
	//	action.pushBack(DelayTime::create(0.8F));
	action.pushBack(RemoveSelf::create());
	action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Awayteam::ProgressTurn)));
	Sequence *seq = Sequence::create(action);
	number->runAction(seq);

	action.clear();
	action.pushBack(TintTo::create(0.2F, 255, 0, 0));
	action.pushBack(TintTo::create(0.2F, 255, 255, 255));
	seq = Sequence::create(action);
	defendspr->runAction(seq);
	action.clear();
	attacker->Flash();
	if (defender->Damage(damage))
	{
		action.clear();
		action.pushBack(FadeOut::create(0.8));
		action.pushBack(ToggleVisibility::create());
		seq = Sequence::create(action);
		defendspr->runAction(seq);

		std::string str = attacker->getString();
		if (str.length() > 0)
		{
			//show death message on prompt
			m_promptframe->setVisible(true);
			long s = str.find("NAME");
			str = str.replace(s, 4, defender->getName());
			m_prompt->setString(str);
			action.clear();
			action.pushBack(DelayTime::create(2.0F));
			action.pushBack(ToggleVisibility::create());
			seq = Sequence::create(action);
			m_promptframe->runAction(seq);
		}

	}
	action.clear();
	
	int x = type * 128;
	Sprite *spr = Sprite::create("art/awayteamfx.png", Rect(x, 0, 32, 32));
	spr->setScale(4/defendspr->getScale());
	defendspr->addChild(spr);
	spr->setPosition(Vec2(0,0));
	spr->setAnchorPoint(Vec2(0.5F, 0.5F));
	spr->setPosition(defender->getCenter());
	Animation *animation = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		animation->addSpriteFrame(SpriteFrame::create("art/awayteamfx.png", Rect(x + (32 * i), 0, 32, 32)));
	}
	animation->setDelayPerUnit(0.1F);
	action.pushBack(Animate::create(animation));
	action.pushBack(RemoveSelf::create());
	seq = Sequence::create(action);
	spr->runAction(seq);

	RedrawBars();
}

void Mode_Awayteam::VictoryCheck()
{
	//check if a captain has 0 hp
	int deadjim = 0;
	for (int i = 0; i < 3; i++)
	{
		if (m_frames[i]->m_officer->getHealth() <= 0)
		{
			if (m_frames[i]->m_officer->getOfficer()->getDefinition()->getName().find("captain") != std::string::npos)
			{
				//gameover

				Scene *scene = GameManager::getInstance()->getHandler()->getScene(-2);
				GameManager::getInstance()->getHandler()->MissionGameOver();
				Director::sharedDirector()->replaceScene(scene);
			}
			deadjim++;
		}
	}
	if (deadjim == 3)
	{
		//failed
		Label *label = Label::createWithBMFont("art/font.fnt", "defeated.");
		label->setAnchorPoint(Vec2(0.5F, 0.5F));
		label->setPosition(Director::getInstance()->getVisibleOrigin()+Vec2(400,500));
		this->setTouchEnabled(false);
		label->setScale(2);
		label->setColor(Color3B(255,0 , 0));
		this->addChild(label);
		Vector <FiniteTimeAction *> action;
		action.pushBack(DelayTime::create(1.0F));
		action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Awayteam::Defeat)));
		Sequence *seq = Sequence::create(action);
		label->runAction(seq);
	}
	int alive = 0;
	for (int i = 0; i < 6; i++)
	{
		if (m_enemies[i] != NULL)
		{
			if (m_enemies[i]->getHealth()>0)
			{
				alive++;
			}
		}
	}
	if (alive == 0)
	{
		//victory
//		CleanUp();
		/*
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(0);
		Director::sharedDirector()->replaceScene(scene); */
		Label *label = Label::createWithBMFont("art/font.fnt", "victory!");
		label->setAnchorPoint(Vec2(0.5F, 0.5F));
		label->setPosition(Director::getInstance()->getVisibleOrigin() + Vec2(400, 500));
		this->setTouchEnabled(false);
		label->setScale(2);
		label->setColor(Color3B(0, 255, 0));
		this->addChild(label);
		Vector <FiniteTimeAction *> action;
		action.pushBack(DelayTime::create(1.0F));
		action.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Awayteam::Victory)));
		Sequence *seq = Sequence::create(action);
		label->runAction(seq);
	}

}

void Mode_Awayteam::Victory()
{
	if (m_host != NULL)
	{
		m_host->ReturntoLayer();
		this->getParent()->removeChild(this);
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			delete m_enemies[i];
		}
		for (int i = 0; i < 3; i++)
		{
		//	m_frames[i]->m_officer->ModEnergy(m_frames[i]->m_officer->getEnergy());
		}
		CleanUp();
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(0);
		Director::sharedDirector()->replaceScene(scene);
	}
}

void Mode_Awayteam::Defeat()
{
	CleanUp();
	Scene *scene = GameManager::getInstance()->getHandler()->getScene(1);
	Director::sharedDirector()->replaceScene(scene);
}

void Mode_Awayteam::SetHost(ReturnableLayer *host)
{
	m_host = host;
}

void Mode_Awayteam::CleanUp()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_frames[i]->m_officer->getHealth() <= 0 && m_frames[i]->m_officer->getOfficer()->getDefinition()->getName().find("captain")==std::string::npos)
		{
			for (int j = 0; j < GameManager::getInstance()->getPlayer()->getNumOfficers(); j++)
			{
				if (GameManager::getInstance()->getPlayer()->getOfficer(j) == m_frames[i]->m_officer->getOfficer())
				{
					delete m_frames[i]->m_officer->getOfficer();
					GameManager::getInstance()->getPlayer()->setOfficer(NULL, j);
				}
			}
		}
		if (m_frames[i]->m_officer->getOfficer()->getDefinition()->getName().find("ensign") != std::string::npos)
		{
			delete m_frames[i]->m_officer->getOfficer();
		}
	}

}

void Mode_Awayteam::Save(FILE *file)
{

}