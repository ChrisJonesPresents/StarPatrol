#include "Mode_Starscape.h"
#include "SolarView.h"
#include "ShipPanel.h"
#include "CrewPanel.h"
#include "LogbookPanel.h"
#include "ModeMenu.h"
#include "ReferenceLibrary.h"

USING_NS_CC;

Scene* Mode_Starscape::createScene(bool solarview)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Mode_Starscape::create();

	// add layer as a child to scene
	scene->addChild(layer);

	if (solarview == true)
	{
		SolarView *view = SolarView::create();
		Player *p = GameManager::getInstance()->getPlayer();
		view->Setup(layer, GameManager::getInstance()->getStars()[(int)p->getPos0().y][(int)p->getPos0().x]);
		scene->addChild(view);
		layer->setTouchEnabled(false);
		layer->setVisible(false);
	}
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Mode_Starscape::init()
{
	m_player = NULL;
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_Background = Starback::create();
	m_Midground = Starback::create();
	this->addChild(m_Background);
	m_Background->setPosition(0, 0);
	m_Midground->setPosition(0, 0);
	this->addChild(m_Midground);
	m_root = Node::create();
	this->addChild(m_root);
	//display stars
	//Star ***stars = GameManager::getInstance()->getStars();
	//int w = GameManager::getInstance()->getWidth();
	//int h = GameManager::getInstance()->getHeight();


	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			m_stars[i][j] = NULL;
		}
	}
	/*
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (stars[i][j] != NULL)
			{
				Sprite *spr = Sprite::create("art/galaxysprites.png", Rect(0, 4, 16, 16));
				spr->setPosition(stars[i][j]->getPosition());
				spr->setScale(2);
				//spr->setZOrder(1);
				m_root->addChild(spr);
			}
		}
	}
	*/
	Playerpos();
	Starspawn();
	this->setTouchEnabled(true);
	m_slide = false;


	//build buttons
	//build travel button
	m_buttons[0] = ui::Button::create("art/starscapebuttonf0.png", "art/starscapebuttonf1.png", "art/bluebutton2.png");
	m_buttons[0]->addClickEventListener(CC_CALLBACK_1(Mode_Starscape::ButtonCallback0, this));

	m_buttons[0]->setPosition(Vec2(cocos2d::Vec2(origin.x + (visibleSize.width/2), origin.y + 48)));
	m_buttons[0]->setVisible(false); m_buttons[0]->setEnabled(false);
	//build wait button
	m_buttons[1] = ui::Button::create("art/starscapebuttong0.png", "art/starscapebuttong1.png", "art/bluebutton2.png");
	m_buttons[1]->addClickEventListener(CC_CALLBACK_1(Mode_Starscape::ButtonCallback1, this));

	m_buttons[1]->setPosition(m_buttons[0]->getPosition());
	
	//build menu button
	m_buttons[2] = ui::Button::create("art/starscapebuttona0.png", "art/starscapebuttona1.png", "art/bluebutton2.png");
	m_buttons[2]->addClickEventListener(CC_CALLBACK_1(Mode_Starscape::ButtonCallback2, this));
	m_buttons[2]->setPosition(Vec2(cocos2d::Vec2(origin.x + 782, origin.y + 16)));
	m_buttons[2]->setScale(1);
	//build ship button
	m_buttons[3] = ui::Button::create("art/starscapebuttonb0.png", "art/starscapebuttonb1.png", "art/bluebutton2.png");
	m_buttons[3]->addClickEventListener(CC_CALLBACK_1(Mode_Starscape::ButtonCallback3, this));
	m_buttons[3]->setPosition(Vec2(cocos2d::Vec2(origin.x + 750, origin.y + 48)));
	m_buttons[3]->setScale(1);
	//build officers button
	m_buttons[4] = ui::Button::create("art/starscapebuttonc0.png", "art/starscapebuttonc1.png", "art/bluebutton2.png");
	m_buttons[4]->addClickEventListener(CC_CALLBACK_1(Mode_Starscape::ButtonCallback4, this));
	m_buttons[4]->setPosition(Vec2(cocos2d::Vec2(origin.x + 782, origin.y + 48)));
	m_buttons[4]->setScale(1);

	//build log button
	m_buttons[5] = ui::Button::create("art/starscapebuttond0.png", "art/starscapebuttond1.png", "art/bluebutton2.png");
	m_buttons[5]->addClickEventListener(CC_CALLBACK_1(Mode_Starscape::ButtonCallback5, this));
	m_buttons[5]->setPosition(Vec2(cocos2d::Vec2(origin.x + 750, origin.y + 16)));
	m_buttons[5]->setScale(1);

	Sprite *spr;
	spr = Sprite::create("art/greybutton2.png");
	spr->setScaleX(3.2F);
	spr->setScaleY(0.5F);
	spr->setPosition(Vec2(origin.x + 400, origin.y + 16));


	this->addChild(spr);
	//build label background in the middle

	m_labels[2] = Label::createWithBMFont("art/font.fnt", "cost:");
	m_labels[2]->setPosition(spr->getPosition());
	m_labels[2]->setSystemFontSize(16);
	SetupPbar();
	this->addChild(m_labels[2]);

	//build date block
	spr = Sprite::create("art/backbox.png");
	spr->setPosition(origin.x + 668, origin.y + 32);
	spr->setScale(2);
	this->addChild(spr);
	m_labels[3] = Label::createWithBMFont("art/font.fnt", "");
	m_labels[3]->setPosition(spr->getPosition());
	m_labels[3]->setSystemFontSize(16);
	this->addChild(m_labels[3]);

	for (int i = 0; i < 6; i++)
	{
		this->addChild(m_buttons[i]);
	}
	SetLabels();
	WriteDate();


	return true;
}

void Mode_Starscape::WriteDate()
{
	std::string str = "";
	char buffer[5];
	itoa(GameManager::getInstance()->getDays(), &buffer[0], 10);
	str.append(buffer);
	str.append("/");
	itoa(GameManager::getInstance()->getMonths()+1, &buffer[0], 10);
	str.append(buffer);
	str.append("/");
	str.append("\n");
	itoa(GameManager::getInstance()->getYears(), &buffer[0], 10);
	str.append(buffer);
	m_labels[3]->setString(str);
}

void Mode_Starscape::SetupPbar()
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Player *player = GameManager::getInstance()->getPlayer();
	//build powerbars
	Sprite *spr[2];
	m_bars[0] = Segbar::create();
	m_bars[0]->Setup(0, 128,player->getShip()->getHPmax(), player->getShip()->getHP());

	m_bars[1] = Segbar::create();
	m_bars[1]->Setup(2, 128, player->getShip()->getMaxFuel(), player->getShip()->getFuel());

	
	char b[6];
	itoa(player->getShip()->getHP(), &b[0], 10);

	m_labels[0] = Label::createWithBMFont("art/font.fnt", b);
	itoa(player->getShip()->getHPmax(), &b[0], 10);
	m_labels[1] = Label::createWithBMFont("art/font.fnt", b);
	for (int i = 0; i < 2; i++)
	{
		spr[i] = Sprite::create("art/shipUI.png", Rect(128, 160 + (32 * i), 32, 32));
		spr[i]->setPosition(origin.x + 16, origin.y + 16 + (32 * i));
		m_labels[i]->setAnchorPoint(Vec2(0, 0.5F));

		m_labels[i]->setPosition(origin.x + 162, origin.y + 16 + (32 * i));
		Sprite *sprb = Sprite::create("art/backbox.png");
		sprb->setAnchorPoint(Vec2(-0.1F, 0.5F));
		sprb->setScaleX(0.5F);
		sprb->setPosition(m_labels[i]->getPosition());
		this->addChild(sprb);
		m_bars[i]->setPosition(origin.x + 32, origin.y + 16 + (32 * i));
		m_bars[i]->setScale(2);
		sprb = Sprite::create("art/backbox.png");
		sprb->setAnchorPoint(Vec2(-0.1F, 0.5F));
		sprb->setScaleX(2.0F);
		sprb->setPosition(m_bars[i]->getPosition());
		this->addChild(sprb);
		this->addChild(m_bars[i]);
		this->addChild(spr[i]);
		this->addChild(m_labels[i]);
		m_bars[i]->setScaleX(1.6F);
	}
}

void Mode_Starscape::Playerpos()
{
	Star ***stars = GameManager::getInstance()->getStars();
	Player *p = GameManager::getInstance()->getPlayer();
	if (m_player == NULL)
	{
		m_player = Sprite::create("art/galaxysprites.png",Rect(0,20,16,16));
		m_root->addChild(m_player);
		m_player->setScale(2);
		m_player->setZOrder(2);
	}
	if (p->getTransit() == 0)
	{
		//place at star and use circle
		m_player->setTextureRect(Rect(0, 20, 16, 16));
		m_player->setPosition(stars[(int)p->getPos0().y][(int)p->getPos0().x]->getPosition());
	}
	else
	{
		//place midway along a path to the destination
		Vec2 p0 = stars[(int)p->getPos0().y][(int)p->getPos0().x]->getPosition();
		Vec2 p1 = stars[(int)p->getPos1().y][(int)p->getPos1().x]->getPosition();
		Vec2 p2 = p0 - p1;
		p2 = p2*p->getTransit();
		m_player->setTextureRect(Rect(16, 20, 16, 16));
		m_player->setPosition(p0+p2);
	}
}

Mode_Starscape::~Mode_Starscape()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				delete m_stars[i][j];
			}
		}
	}

}

void Mode_Starscape::Starspawn()
{
	Player *p = GameManager::getInstance()->getPlayer();
	Star ***stars = GameManager::getInstance()->getStars();
	int w = GameManager::getInstance()->getWidth();
	int h = GameManager::getInstance()->getHeight();
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int x = p->getPos0().x+j-3; int y = p->getPos0().y+i-3;
			if (x >= 0 && y >= 0 && x < w && y < h)
			{
				if (stars[y][x] != NULL)
				{
					Sprite *spr = Sprite::create("art/galaxysprites.png", Rect(128*stars[y][x]->getGraphic().x, 64+(32*stars[y][x]->getGraphic().y), 32, 32));
					spr->setPosition(stars[y][x]->getPosition());
					spr->setScale(1);
					//spr->setZOrder(1);
					m_root->addChild(spr);
					Animation *an = Animation::create();
					for (int k = 0; k < 4; k++)
					{
						SpriteFrame *Frame = SpriteFrame::create("art/galaxysprites.png", Rect((128 * stars[y][x]->getGraphic().x)+(32*k), 64 + (32 * stars[y][x]->getGraphic().y), 32, 32));
						an->addSpriteFrame(Frame);
					}
					an->setDelayPerUnit(0.25F);
					spr->runAction(RepeatForever::create(Animate::create(an)));

					m_stars[i][j] = new StarWrap();
					m_stars[i][j]->sprite = spr;
					m_stars[i][j]->m_x = x;
					m_stars[i][j]->m_y = y;
					Label *label = Label::createWithBMFont("art/font.fnt", stars[y][x]->getName());
					m_stars[i][j]->m_label = label;
					m_root->addChild(label);
					label->setPosition(stars[y][x]->getPosition() - Vec2(0, 32));
					if (GameManager::getInstance()->findColony(y, x) != NULL)
					{
						label->setColor(Color3B(0, 0, 255));
					}
					m_stars[i][j]->sprite->setZOrder(1);
					m_stars[i][j]->m_label->setZOrder(1);
				}
			}
		}
	}

}

void Mode_Starscape::Starcull()
{

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				m_root->removeChild(m_stars[i][j]->sprite);
				delete m_stars[i][j];

				m_stars[i][j] = NULL;
			}
		}
	}

}

bool Mode_Starscape::updatePlayer()
{


	return false;
}

void 	Mode_Starscape::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	m_slide = true;
	bool notouch = true;
	Vec2 p = getTouchpos(touch->getLocationInView(), this)-m_root->getPosition();
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (m_stars[i][j] != NULL)
			{
				if (p.getDistance(m_stars[i][j]->sprite->getPosition())<16)
				{
					notouch = false;
					m_slide = false;
					Startouched(m_stars[i][j]->m_x, m_stars[i][j]->m_y);
				}
			}
		}
	}
	if (notouch == true)
	{
		m_path.Clear();
		m_buttons[0]->setVisible(false); m_buttons[0]->setEnabled(false);
		m_buttons[1]->setVisible(true); m_buttons[1]->setEnabled(true);
		m_labels[2]->setString("");
	}
}

void 	Mode_Starscape::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_slide == true)
	{
		m_root->setPosition(m_root->getPosition() + touch->getDelta());
		m_Background->setPosition(m_Background->getPosition() + (touch->getDelta()*0.2F));
		m_Midground->setPosition(m_Midground->getPosition() + (touch->getDelta()*0.5F));
		//prevent it going too far away
		Vec2 p = (m_root->getPosition()*-1) + Vec2(400, 320);
		Vec2 player = m_player->getPosition();
		float d =p.getDistance(player);
		if (d > 300)
		{
			Vec2 n = player- p; float l = n.length(); n = n / l; n = n*touch->getDelta().length();
			m_root->setPosition(m_root->getPosition() - (n));
			m_Background->setPosition(m_Background->getPosition() - (n*0.2F));
			m_Midground->setPosition(m_Midground->getPosition() - (n*0.5F));
		}
	}
}

void 	Mode_Starscape::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	m_slide = false;
}


void Mode_Starscape::Startouched(int x, int y)
{
	Player *p=GameManager::getInstance()->getPlayer();
	int xat = p->getPos0().x; int yat = p->getPos0().y;
	//is this the star the player is currently orbiting?
	if (xat == x && yat == y)
	{
		//later, zoom in on star contents when this happens
		SolarView *view = SolarView::create();
		view->Setup(this,GameManager::getInstance()->getStars()[(int)p->getPos0().y][(int)p->getPos0().x]);

		this->getParent()->addChild(view);
		this->setTouchEnabled(false);
		this->setVisible(false);
	}
	//if not, attempt to show a path
	else
	{
		if (m_path.Generate(m_root, GameManager::getInstance()->getStars(), GameManager::getInstance()->getWidth(), GameManager::getInstance()->getHeight(), p->getPos0(), Vec2(x, y)))
		{
			m_buttons[0]->setVisible(true); m_buttons[0]->setEnabled(true);
			m_buttons[1]->setVisible(false); m_buttons[1]->setEnabled(false);
			CalcCost(x, y);
		}
		else
		{
			m_buttons[0]->setVisible(false); m_buttons[0]->setEnabled(false);
			m_buttons[1]->setVisible(true); m_buttons[1]->setEnabled(true);
		}
	}
}

void Mode_Starscape::CalcCost(int x, int y)
{
	GameManager *manager = GameManager::getInstance();
	Playership *pp = manager->getPlayer()->getShip();
	Vec2 p = manager->getStars()[(int)manager->getPlayer()->getPos0().y][(int)manager->getPlayer()->getPos0().x]->getPosition();
	Vec2 d = manager->getStars()[y][x]->getPosition();
	float distance = p.getDistance(d);
	m_costfuel = distance / 16;
	m_costtime = distance / manager->getPlayer()->getShip()->getSpeed();
	char buffer[4];
	sprintf(buffer, "%d", m_costfuel);
	std::string str; str.append(buffer); str.append(" fuel ");
	sprintf(buffer, "%d", m_costtime);
	str.append(buffer); str.append(" days");
	m_labels[2]->setString(str);
}

void Mode_Starscape::ButtonCallback0(Ref *pSender)
{
	if (m_costfuel < GameManager::getInstance()->getPlayer()->getShip()->getFuel() && m_player->getNumberOfRunningActions()==0)
	{
		this->setTouchEnabled(false);
		m_player->setTextureRect(Rect(16, 20, 16, 16));
		Vector <FiniteTimeAction*> actions;
		actions.pushBack(MoveTo::create(1.0F, GameManager::getInstance()->getStars()[(int)m_path.getStep().y][(int)m_path.getStep().x]->getPosition()));
		actions.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Starscape::Concluded)));
		m_player->runAction(Sequence::create(actions));
		TimeIncrement(m_costtime);
	}
}

void Mode_Starscape::ButtonCallback1(Ref *pSender)
{
	TimeIncrement(10);
	WriteDate();
	//GenLogDisplay();
	Vec2 p = GameManager::getInstance()->getPlayer()->getPos0();
	if (GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission() != NULL)
	{
		GameManager::getInstance()->getHandler()->setMission(GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission());
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(2);
		GameManager::getInstance()->getHandler()->setLayer((SaveLayer*)scene->getChildByTag(100));
		Director::sharedDirector()->replaceScene(scene);
	}
	else
	{
		if (GameManager::getInstance()->getPlayer()->getShip()->getFuel() < 10)
		{
			//trigger fuel encounter
			if (GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission() == NULL)
			{
				GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->setMission(ReferenceLibrary::getInstance()->getQuestEngine()->AddWait(2));
			}
		}
	}
	
}

void Mode_Starscape::Concluded()
{
	m_buttons[0]->setVisible(false); m_buttons[0]->setEnabled(false);
	m_buttons[1]->setVisible(true); m_buttons[1]->setEnabled(true);
	this->setTouchEnabled(true);
	m_player->setTextureRect(Rect(0, 20, 16, 16));
	GameManager::getInstance()->getPlayer()->setPos(m_path.getStep());
	GameManager::getInstance()->getPlayer()->getShip()->modFuel(m_costfuel);


	m_labels[1]->setString("");
	if (GameManager::getInstance()->getStars()[(int)m_path.getStep().y][(int)m_path.getStep().x]->getExplored() == false)
	{
		GameManager::getInstance()->Explore(m_path.getStep().y, m_path.getStep().x);
		Starcull();
		Starspawn();
	}

	m_path.Clear();
	SetLabels();
	m_labels[2]->setString("");
	WriteDate();
	Vec2 p = GameManager::getInstance()->getPlayer()->getPos0();
	if (GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission() != NULL)
	{
		if (GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission()->getCurrentNode() == NULL)
		{
			ReferenceLibrary::getInstance()->getQuestEngine()->Convertstub(GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission(),NULL);
		}
		GameManager::getInstance()->getHandler()->setMission(GameManager::getInstance()->getStars()[(int)p.y][(int)p.x]->getMission());
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(2);
		Director::sharedDirector()->replaceScene(scene);
	}
}

void Mode_Starscape::SetLabels()
{
	char buffer[4];

	sprintf(buffer, "%d", GameManager::getInstance()->getPlayer()->getShip()->getHP());
	std::string str = ""; str.append(buffer);
	m_labels[0]->setString(str);
	sprintf(buffer, "%d", GameManager::getInstance()->getPlayer()->getShip()->getFuel());
	str = ""; str.append(buffer);
	m_labels[1]->setString(str);
	m_bars[0]->setVariable(GameManager::getInstance()->getPlayer()->getShip()->getHP());
	m_bars[1]->setVariable(GameManager::getInstance()->getPlayer()->getShip()->getFuel());

}

void Mode_Starscape::ButtonCallback2(Ref *pSender)
{
	//initiate menu
	Scene *scene= ModeMenu::createScene(0);
	Director::sharedDirector()->replaceScene(scene);
}

void Mode_Starscape::ButtonCallback3(Ref *pSender)
{
	//initiate ship panel
	
	Scene *scene = ShipPanel::createScene(GameManager::getInstance()->getPlayer());
	Director::getInstance()->replaceScene(scene);
}

void Mode_Starscape::ButtonCallback4(Ref *pSender)
{
	//initiate officers panel
	Scene *scene = CrewPanel::createScene(GameManager::getInstance()->getPlayer());
	Director::getInstance()->replaceScene(scene);
}

void Mode_Starscape::ButtonCallback5(Ref *pSender)
{
	//initiate log panel
	Scene *scene = LogbookPanel::createScene();
	Director::getInstance()->replaceScene(scene);
}

void Mode_Starscape::GenLogDisplay()
{
	GameManager *gamemanager = GameManager::getInstance();
	Logbook *logs = gamemanager->getPlayer()->getLogbook();
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//generate frame
	Sprite *frame = Sprite::create("art/logbookui.png", Rect(0, 32, 256, 32));
	frame->setScaleX(3);
	frame->setPosition(origin.x + visibleSize.width / 2, origin.y + 88);
	this->addChild(frame);
	//labels
	Label *label = Label::createWithBMFont("art/font.fnt", "text text text text text text text text text");
	label->setPosition(Vec2(24,16));
	label->setScaleY(0.75F);
	label->setScaleX(0.30F);
	label->setAnchorPoint(Vec2(0.0F, 0.5F));
	frame->addChild(label);
	//icons
	Sprite *sprite = Sprite::create("art/logbookui.png", Rect(32, 96, 32, 32));
	sprite->setPosition(Vec2(16, 16));
	sprite->setScaleX(0.33F);
	frame->addChild(sprite);
	//generate icon
	std::string str = "";
	char buffer[6];
	itoa(logs->getEntry(logs->getLength()-1)->m_timestamp % 30, &buffer[0], 10);
	str.append(buffer); str.append("/");
	itoa(((logs->getEntry(logs->getLength() - 1)->m_timestamp/ 30) % 12)+1, &buffer[0], 10);
	str.append(buffer); str.append("/");
	itoa(2115 + (logs->getEntry(logs->getLength() - 1)->m_timestamp / 360), &buffer[0], 10);
	str.append(buffer); str.append(" ");
	str.append(logs->getEntry(logs->getLength() - 1)->m_string.substr(0, 48));
	if (logs->getEntry(logs->getLength() - 1)->m_string.length()>48)
	{
		str.append("...");
	}
	label->setString(str);

	/*		Vector <FiniteTimeAction*> actions;
		actions.pushBack(MoveTo::create(1.0F, GameManager::getInstance()->getStars()[(int)m_path.getStep().y][(int)m_path.getStep().x]->getPosition()));
		actions.pushBack(CallFunc::create(this, SEL_CallFunc(&Mode_Starscape::Concluded)));
		m_player->runAction(Sequence::create(actions));
		*/
	frame->setVisible(false);
	Vector <FiniteTimeAction*> actions;
	actions.pushBack(DelayTime::create(1.0F));
	actions.pushBack(ToggleVisibility::create());
	actions.pushBack(DelayTime::create(5.0F));
	actions.pushBack(RemoveSelf::create());
	frame->runAction(Sequence::create(actions));
}

void  Mode_Starscape::TimeIncrement(int time)
{
	if (GameManager::getInstance()->IncrementTime(time))
	{
		GenLogDisplay();
	}

}