#include "ModeShop.h"
#include "ModeWarpgate.h"
#include "ShopLayer.h"
#include "ReferenceLibrary.h"
#include "ModeColony.h"
#include "GameManager.h"

void ShopInventory::Setup(Sprite **sprites, int numsprites)
{
	m_y = this->getPositionY();
	m_length = numsprites;
	m_sprites = sprites;
	for (int i = 0; i < numsprites; i++)
	{
		//reposition
		m_sprites[i]->setPosition(Vec2(0, (m_length*-48) + (96 * i)));
		m_sprites[i]->setScale(2);
		//add children
		this->addChild(m_sprites[i]);
	}
	if (m_length>0)
	{
		m_index = 0;
		m_indicator = Sprite::create("art/yellowframe.png");
		m_indicator->setPosition(m_sprites[0]->getPosition());
		m_indicator->setScale(2.1F);
		this->addChild(m_indicator);
	}


}

void ShopInventory::Slide(Touch *touch)
{
	float y = touch->getDelta().y;
	if (y > 0)
	{
		if (this->getPositionY()<m_y + (m_length * 48))
		{
			this->setPosition(this->getPosition() + Vec2(0, y));
		}

	}
	else if (this->getPositionY()>m_y+(m_length*-48))
	{
		this->setPosition(this->getPosition() + Vec2(0, y));
	}
}

ShopInventory::	~ShopInventory()
{
	if (m_length > 0)
	{
		delete[]m_sprites;
	}
}

cocos2d::Scene* ModeShop::createScene(Player *playerfile,ShopDef *def,bool warpgate)
{
	auto *Scene = Scene::create();
	auto *layer = ModeShop::create();
	layer->Setup(playerfile,def,warpgate);
	Scene->addChild(layer);
	return Scene;
}

cocos2d::Scene* ModeShop::createSceneWarpgate(Player *playerfile)
{
	auto *Scene = Scene::create();
	auto *layer = ModeShop::create();
	//build shopdef
	ShopDef *def = new ShopDef();
	def->m_supplies[0][0] = 6;
	def->m_supplies[0][1] = 99;
	def->m_supplies[1][0] = 4;
	def->m_supplies[1][1] = 99;
	def->m_supplies[2][0] = 0;
	def->m_supplies[2][1] = 0;
	def->m_numitems = 1;
	def->m_numrecruits = 0;
	def->m_items[0] = new int[1];	def->m_items[1] = new int[1];
	def->m_items[0][0] = 1; def->m_items[1][0] = 0;
	def->m_recruits[0] =	NULL;	def->m_recruits[1] = NULL;
	
	layer->Setup(playerfile,def,true);
	Scene->addChild(layer);
	return Scene;
}

ModeShop::~ModeShop()
{
	if (m_warpgate == true)
	{
		delete m_def;
	}

}
bool ModeShop::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_active = true;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite *spr3 = Sprite::create("art/grid.png");
	spr3->setPosition(origin.x + (visibleSize.width / 2)-134, origin.y + (visibleSize.height / 2));
	spr3->setScale(3.125F);
	this->addChild(spr3);
	for (int i = 0; i < 1; i++)
	{
		Sprite *spr = Sprite::create("art/patternedui.png");

		spr->setPosition(origin.x + 64, origin.y + 64 +(128*i));
		this->addChild(spr);
	}


	Sprite *spr = Sprite::create("art/vbar.png");
	spr->setPosition(origin.x + visibleSize.width - 134, origin.y + (visibleSize.height / 2));
	spr->setScaleY(160);
	this->addChild(spr);
	m_descriptionframe = Sprite::create("art/frame.png");
	m_descriptionframe->setScaleY(1);
	m_descriptionframe->setScaleX(3.2F);
	m_descriptionframe->setPosition(Vec2(origin.x + (visibleSize.width / 2)-68, origin.y + 64));
	this->addChild(m_descriptionframe);
	m_description = Label::createWithBMFont("art/font.fnt", "");
	m_description->setAlignment(TextHAlignment::LEFT);
	m_description->setPosition(m_descriptionframe->getPosition());
	m_description->setSystemFontSize(24);
	m_description->setWidth(380);
	this->addChild(m_description);
	this->setTouchEnabled(true);
	m_purchasemode = 0;
	m_amountpurchase = 0;
	return true;
}

void ModeShop::Setup(Player *playerfile,ShopDef *def, bool warpgate)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_playerfile = playerfile;
	m_outline.Setup(m_playerfile->getShip(), this, m_playerfile, true);
	m_warpgate = warpgate;
	m_def = def;


	Sprite *spr = Sprite::create("art/patternedui.png");
	spr->setPosition(origin.x + visibleSize.width - 200, origin.y + 64);
	this->addChild(spr);

	//add leave button here
	ui::Button *button = ui::Button::create("art/colonybuttond0.png", "art/colonybuttond1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeShop::ButtonCallback0, this));
	button->setPosition(spr->getPosition() - Vec2(32, 48));
	button->setScale(1);
	this->addChild(button);
	//add money counter above leave button here, use spr patternedui as an anchor point
	Sprite *spr1 = Sprite::create("art/shipUI.png", Rect(192, 224, 32, 32));
	spr1->setPosition(spr->getPosition() + Vec2(-48, 16));
	this->addChild(spr1);
	char b[6];
	_itoa(m_playerfile->getMoney(), &b[0], 10);
	m_money = Label::createWithBMFont("art/font.fnt", b);

	Sprite *sprb = Sprite::create("art/backbox.png");
	sprb->setScaleX(1.5F);
	m_money->setAnchorPoint(Vec2(0, 0.5F));
	m_money->setPosition(spr1->getPosition() + Vec2(16, 0));
	sprb->setPosition(m_money->getPosition() + Vec2(-4, 0));
	sprb->setAnchorPoint(Vec2(0, 0.5F));
	this->addChild(sprb);
	this->addChild(m_money);

	m_bars[0] = Segbar::create();
	m_bars[1] = Segbar::create();
	m_bars[1]->Setup(2, 128, m_playerfile->getShip()->getMaxFuel(), m_playerfile->getShip()->getFuel());
	m_bars[0]->Setup(0, 128, m_playerfile->getShip()->getHPmax(), m_playerfile->getShip()->getHP());


	//build fix button
	m_buttons[0] = ui::Button::create("art/shopbuttona0.png", "art/shopbuttona1.png", "art/shopbuttona2.png");
	m_buttons[0]->addClickEventListener(CC_CALLBACK_1(ModeShop::ButtonCallback1, this));
	//build fuel button
	m_buttons[1] = ui::Button::create("art/shopbuttonb0.png", "art/shopbuttonb1.png", "shopbuttonb2.png");
	m_buttons[1]->addClickEventListener(CC_CALLBACK_1(ModeShop::ButtonCallback2, this));
	_itoa(m_playerfile->getShip()->getHP(), &b[0], 10);
	Sprite *spra[2];
	m_labels[0] = Label::createWithBMFont("art/font.fnt", b);
	_itoa(m_playerfile->getShip()->getFuel(), &b[0], 10);
	m_labels[1] = Label::createWithBMFont("art/font.fnt", b);
	for (int i = 0; i < 2; i++)
	{
		m_bars[i]->setPosition(origin.x + 0, origin.y + 16+(64 * i));
		spra[i] = Sprite::create("art/shipUI.png", Rect(128, 160 + (32 * i), 32, 32));
		spra[i]->setPosition(m_bars[i]->getPosition()+Vec2(16,32));
		m_labels[i]->setAnchorPoint(Vec2(0, 0.5F));
		
		m_labels[i]->setPosition(spra[i]->getPosition()+Vec2(16,0));
		Sprite *sprb = Sprite::create("art/backbox.png");
		sprb->setAnchorPoint(Vec2(0, 0.5F));
		sprb->setScaleX(0.6F);
		sprb->setPosition(m_labels[i]->getPosition()-Vec2(4,0));
		this->addChild(sprb);
		m_buttons[i]->setPosition(m_labels[i]->getPosition() + Vec2(64, 0));
		m_bars[i]->setScale(2);

		sprb = Sprite::create("art/backbox.png");
		sprb->setAnchorPoint(Vec2(0, 0.5F));
		sprb->setScaleX(1.6F);
		sprb->setPosition(m_bars[i]->getPosition()+Vec2(0,0));
		this->addChild(sprb);
		this->addChild(m_bars[i]);
		this->addChild(spra[i]);
		this->addChild(m_labels[i]);
		this->addChild(m_buttons[i]);

		m_bars[i]->setScaleX(1.6F);
	}
	//add buy ammo button
	m_buttons[2] = ui::Button::create("art/shopbuttonc0.png", "art/shopbuttonc1.png", "art/shopbuttonc2.png");
	m_buttons[2]->addClickEventListener(CC_CALLBACK_1(ModeShop::ButtonCallback3, this));
	m_buttons[2]->setPosition(Vec2(origin.x + 96, origin.y + 144));
	this->addChild(m_buttons[2]);

	setButtonState();
	
	SetupInventory();
	
	//add buy this button
	m_buttons[3] = ui::Button::create("art/shopbuttond0.png", "art/shopbuttond1.png", "art/shopbuttond2.png");
	m_buttons[3]->addClickEventListener(CC_CALLBACK_1(ModeShop::ButtonCallback4, this));
	m_buttons[3]->setPosition(Vec2(origin.x + visibleSize.width-64, origin.y + 32));
	m_buttons[3]->setScale(2);
	this->addChild(m_buttons[3]);
	if (m_def->m_numitems > 0)
	{
		CanbuySet();
	}
	else
	{
		m_buttons[3]->setBright(false);
		m_buttons[3]->setEnabled(false);
	}
	m_salebox = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
	m_salebox->setPosition(origin.x + visibleSize.width - 176, origin.y + 192);
	m_salebox->setScale(2);
	m_purchase = NULL;
	this->addChild(m_salebox);
}

void ModeShop::SetupInventory()
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//add list of items
	m_inventory = ShopInventory::create();
	m_inventory->setPosition(origin.x + visibleSize.width - 32, origin.y + (visibleSize.height / 2));
	//setup inventory
	Sprite **sprites = new Sprite *[m_def->m_numitems];

	for (int i = 0; i < m_def->m_numitems; i++)
	{
		char buffer[5];
	
		switch (m_def->m_items[0][i])
		{
		case 1:
			//is an item
			sprites[i] = Sprite::create("art/itemsprites.png", Rect((m_def->m_items[1][i] % 8) * 32, (m_def->m_items[1][i] / 8) * 32, 32, 32));
			_itoa(ReferenceLibrary::getInstance()->getItem(m_def->m_items[1][i])->m_cost, &buffer[0], 10);
			break;
		case 2:
			//is a module
			sprites[i] = Sprite::create("art/moduleicons.png", Rect(ReferenceLibrary::getInstance()->getModule(m_def->m_items[1][i])->getGraphicpos().x * 32, ReferenceLibrary::getInstance()->getModule(m_def->m_items[1][i])->getGraphicpos().y * 32, 32, 32));
			_itoa(ReferenceLibrary::getInstance()->getModule(m_def->m_items[1][i])->getCost(), &buffer[0], 10);
			break;
		}
		Label *label = Label::createWithBMFont("art/font.fnt", buffer);
		label->setPosition(Vec2(0, -8));
		label->setAnchorPoint(Vec2(0, 0.5F));
		label->setSystemFontSize(12);
		label->setScale(0.5F);
		sprites[i]->addChild(label);
	}
	m_inventory->Setup(sprites, m_def->m_numitems);
	this->addChild(m_inventory);

}

void ModeShop::setButtonState()
{

	for (int i = 0; i < 3; i++)
	{
		if (m_def->m_supplies[i][1] <= 0)
		{
			m_buttons[i]->setEnabled(false);
			m_buttons[i]->setBright(false);
		}
	}
}

void ModeShop::CanbuySet()
{
	int v = 0;
	switch (m_def->m_items[0][m_inventory->m_index])
	{
	case 1:
		//item;
		v = ReferenceLibrary::getInstance()->getItem(m_def->m_items[1][m_inventory->m_index])->m_cost;
		break;
	case 2:
		//module
		v = ReferenceLibrary::getInstance()->getModule(m_def->m_items[1][m_inventory->m_index])->getCost();
		break;
	}
	if (v <= m_playerfile->getMoney())
	{
		m_buttons[3]->setBright(true);
		m_buttons[3]->setEnabled(true);
	}
	else
	{
		m_buttons[3]->setBright(false);
		m_buttons[3]->setEnabled(false);
	}
}

void ModeShop::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	//grab whatever sprite's under the touch
	Touch *touch = touches[0];
	for (int i = 0; i < m_outline.m_player->getShip()->getNumModules(); i++)
	{
		if (m_outline.m_modulesprites[i] != NULL)
		{
			if (m_outline.m_modulesprites[i]->getBoundingBox().containsPoint(touch->getLocation()))
			{
				m_description->setString(m_outline.m_player->getShip()->getModule(i)->getDesc());
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
					m_description->setString(m_playerfile->getItem(i)->m_description);
					break;
				}
			}
		}
	}
	if (m_outline.m_heldsprite == -1)
	{
		if (touch->getLocation().x > 800 - 164 && m_inventory->m_length>0)
		{
			//figure out what the player just tapped
			for (int i = 0; i < m_inventory->m_length; i++)
			{
				Rect box = m_inventory->m_sprites[i]->getBoundingBox();
				box.origin.x += m_inventory->getPositionX();
				box.origin.y += m_inventory->getPositionY();
				if (box.containsPoint(touch->getLocation()))
				{
					switch (m_def->m_items[0][i])
					{
					case 1:
						//item;
						m_description->setString(ReferenceLibrary::getInstance()->getItem(m_def->m_items[1][i])->m_description);
						break;
					case 2:
						//module
						m_description->setString(ReferenceLibrary::getInstance()->getModule(m_def->m_items[1][i])->getDesc());
						break;
					}
					m_inventory->m_index = i;
					m_inventory->m_indicator->setPosition(m_inventory->m_sprites[i]->getPosition());
					CanbuySet();
					break;
				}
			}
		}
	}
	if (m_outline.m_heldsprite == -1)
	{
		if (m_purchase != NULL)
		{
			if (m_purchase->getBoundingBox().containsPoint(touch->getLocation()))
			{
				m_outline.m_heldsprite = 40;
			}
		}
	}
}

void ModeShop::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
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
				if (m_outline.m_heldsprite == 40)
				{
					m_purchase->setPosition(touch->getLocation());
				}
			}
		}
	}
	else
	{
		if (touch->getLocation().x > 800 - 164 && m_inventory->m_length>0)
		{
			m_inventory->Slide(touch);
		}
	}
}

void ModeShop::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
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
					if (m_outline.m_heldsprite == 40)
					{
						//put the item in the ship
						if (m_outline.AddPurchase(m_saletype[0], &m_saletype[1], touch->getLocation(), m_purchase))
						{
							m_purchase = NULL;
							m_saletype[0] = 0;
							m_saletype[1] = 0;
						}
						else
						{
							m_purchase->setPosition(m_salebox->getPosition());
						}
					}
				}
			}


		}
	}
	m_outline.m_heldsprite = -1;
}

void ModeShop::ButtonCallback0(Ref *pSender)
{
	RecycleSaleBox();
	if (m_active==true)
	{
		if (GameManager::getInstance()->getHandler()->getInMission() == true)
		{
			Scene *scene = GameManager::getInstance()->getHandler()->getScene(0);
			Director::sharedDirector()->replaceScene(scene);
		}
		else
		{
			//exit
			if (m_warpgate == true)
			{
				auto *gate = ModeWarpgate::createScene(m_playerfile);

				Director::sharedDirector()->replaceScene(gate);
			}
			else
			{
				//transition to the colony...somehow
				auto *colony = ModeColony::createScene(m_playerfile, GameManager::getInstance()->findColony(m_playerfile->getPos0().y, m_playerfile->getPos0().x));

				Director::sharedDirector()->replaceScene(colony);
			}
		}
	}
}

void ModeShop::ButtonCallback1(Ref *pSender)
{
	
	if (m_active == true && m_playerfile->getShip()->getHPmax() - m_playerfile->getShip()->getHP() >0)
	{
		//bring up shoplayer set to hull
		ShopLayer *layer = ShopLayer::create();

		int m = 20;
		if (m_def->m_supplies[0][1] < m)
		{
			m = m_def->m_supplies[0][1];
		}
		if (m>m_playerfile->getShip()->getHPmax() - m_playerfile->getShip()->getHP())
		{
			m = m_playerfile->getShip()->getHPmax() - m_playerfile->getShip()->getHP();
		}
		if (m*m_def->m_supplies[0][0] > m_playerfile->getMoney())
		{
			m = m_playerfile->getMoney() / m_def->m_supplies[0][0];
		}
		m_amountpurchase = 0;
		m_purchasemode = 0;
		layer->Setup(0, &m_amountpurchase, m, this, m_def->m_supplies[0][0]);
		this->getParent()->addChild(layer);
		this->setTouchEnabled(false);
		m_active = false;
	}
	
}

void ModeShop::ButtonCallback2(Ref *pSender)
{
	if (m_active==true)
	{
		//bring up shoplayer to set to fuel
		ShopLayer *layer = ShopLayer::create();

		int m = 20;
		if (m_def->m_supplies[1][1] < m)
		{
			m = m_def->m_supplies[1][1];
		}
		if (m*m_def->m_supplies[1][0] > m_playerfile->getMoney())
		{
			m = m_playerfile->getMoney() / m_def->m_supplies[1][0];
		}
		m_amountpurchase = 0;
		m_purchasemode = 1;
		layer->Setup(1, &m_amountpurchase, m, this, m_def->m_supplies[1][0]);
		this->getParent()->addChild(layer);
		this->setTouchEnabled(false);
		m_active = false;
	}

}

void ModeShop::ButtonCallback3(Ref *pSender)
{
	if (m_active==true)
	{
		//bring up shoplayer set to ordinance
		ShopLayer *layer = ShopLayer::create();

		int m = 20;
		if (m_def->m_supplies[2][1] < m)
		{
			m = m_def->m_supplies[2][1];
		}
		if (m*m_def->m_supplies[2][0] > m_playerfile->getMoney())
		{
			m = m_playerfile->getMoney() / m_def->m_supplies[2][0];
		}
		m_amountpurchase = 0;
		m_purchasemode = 2;
		layer->Setup(2, &m_amountpurchase, m, this,m_def->m_supplies[2][0]);
		this->getParent()->addChild(layer);
		this->setTouchEnabled(false);
		m_active = false;
	}
	
}

void ModeShop::ButtonCallback4(Ref *pSender)
{
	if (m_active == true)
	{
		//add item to sales box
		RecycleSaleBox();

		//put new thing in sales box
		m_saletype[0] = m_def->m_items[0][m_inventory->m_index];
		m_saletype[1] = m_def->m_items[1][m_inventory->m_index];
		switch (m_saletype[0])
		{
		case 1:
			m_purchase = Sprite::create("art/itemsprites.png", Rect((m_saletype[1] % 8), m_saletype[1] / 8, 32, 32));
			m_playerfile->setMoney(m_playerfile->getMoney() - ReferenceLibrary::getInstance()->getItem(m_saletype[1])->m_cost);
			break;
		case 2:
			m_purchase = Sprite::create("art/moduleicons.png",
				Rect(ReferenceLibrary::getInstance()->getModule(m_saletype[1])->getGraphicpos().x * 32, ReferenceLibrary::getInstance()->getModule(m_saletype[1])->getGraphicpos().y * 32, 32, 32));
			m_purchase->setScale(2);
			m_playerfile->setMoney(m_playerfile->getMoney() - ReferenceLibrary::getInstance()->getModule(m_saletype[1])->getCost());
			break;
		}
		m_purchase->setPosition(m_salebox->getPosition());
		this->addChild(m_purchase);

		//subtract money

		char buffer[6];
		_itoa(m_playerfile->getMoney(), &buffer[0], 10);
		m_money->setString(buffer);
	}
	
}

void ModeShop::RecycleSaleBox()
{
	if (m_purchase != NULL)
	{
		this->removeChild(m_purchase);
		m_purchase = NULL;
	}

	switch (m_saletype[0])
	{
	case 1:
		//item
		m_playerfile->setMoney(m_playerfile->getMoney() + ReferenceLibrary::getInstance()->getItem(m_saletype[1])->m_cost);
		break;

	case 2:
		//modules
		m_playerfile->setMoney(m_playerfile->getMoney() + ReferenceLibrary::getInstance()->getModule(m_saletype[1])->getCost());
		break;

	case 3:
		//fuel
		m_def->m_supplies[1][1] += m_saletype[1];
		m_playerfile->setMoney(m_playerfile->getMoney() + (m_saletype[1] * 2));
		break;

	case 4:
		//ordinance
		m_def->m_supplies[2][1] += m_saletype[1];
		m_playerfile->setMoney(m_playerfile->getMoney() + (m_saletype[1] * 4));
		break;
	}
	m_saletype[0] = 0;
	m_saletype[1] = 0;
	char buffer[7];
	_itoa(m_playerfile->getMoney(), &buffer[0], 10);
	m_money->setString(buffer);
}
void ModeShop::ReturntoLayer()
{
	m_active = true;
	this->setTouchEnabled(true);
	if (m_amountpurchase > 0)
	{
		char buffer[7];
		switch (m_purchasemode)
		{
		case 0:
			//buy repairs
			m_playerfile->setMoney(m_playerfile->getMoney() - (m_def->m_supplies[0][0] * m_amountpurchase));
			m_playerfile->getShip()->setHP(m_playerfile->getShip()->getHP() + m_amountpurchase);
			m_bars[0]->setVariable(m_playerfile->getShip()->getHP());
			_itoa(m_playerfile->getShip()->getHP(), &buffer[0], 10);
			m_labels[0]->setString(buffer);
			_itoa(m_playerfile->getMoney(), &buffer[0], 10);
			m_money->setString(buffer);
			break;

		case 1:
			//buy fuel
			m_playerfile->setMoney(m_playerfile->getMoney() - (m_def->m_supplies[1][0] * m_amountpurchase));
			BuyFuel();
			m_bars[1]->setVariable(m_playerfile->getShip()->getFuel());
			_itoa(m_playerfile->getShip()->getFuel(), &buffer[0], 10);
			m_labels[1]->setString(buffer);
			_itoa(m_playerfile->getMoney(), &buffer[0], 10);
			m_money->setString(buffer);
			break;
		case 2:
			//buy ordinance
			BuyAmmo();

			break;


		}


	}


}


void ModeShop::BuyFuel()
{
	if (m_amountpurchase > m_playerfile->getShip()->getMaxFuel() - m_playerfile->getShip()->getFuel())
	{
		m_amountpurchase -= m_playerfile->getShip()->getMaxFuel() - m_playerfile->getShip()->getFuel();
		m_playerfile->getShip()->setFuel(m_playerfile->getShip()->getMaxFuel());
		//put remainder in a stack in the slot
		RecycleSaleBox();
		m_saletype[0] = 3; m_saletype[1] = m_amountpurchase;
		m_purchase = Sprite::create("art/moduleicons.png", Rect(0, 0, 32, 32));
		m_purchase->setPosition(m_salebox->getPosition());
		char buffer[4];
		_itoa(m_saletype[1], &buffer[0], 10);
		Label *label = Label::createWithBMFont("art/font.fnt", buffer);
		label->setScale(0.5F);
		label->setPosition(16, 16);
		m_purchase->setScale(2);
		m_purchase->addChild(label,0,50);
		this->addChild(m_purchase);

	}
	else
	{
		m_playerfile->getShip()->modFuel(m_amountpurchase*-1);
	}
}

void ModeShop::BuyAmmo()
{
	RecycleSaleBox();
	m_saletype[0] = 4; m_saletype[1] = m_amountpurchase;
	m_purchase = Sprite::create("art/moduleicons.png", Rect(32, 0, 32, 32));
	m_purchase->setPosition(m_salebox->getPosition());
	char buffer[4];
	_itoa(m_saletype[1], &buffer[0], 10);
	Label *label = Label::createWithBMFont("art/font.fnt", buffer);
	label->setScale(0.5F);
	label->setPosition(16, 16);
	m_purchase->setScale(2);
	m_purchase->addChild(label);
	this->addChild(m_purchase);
}

void  ModeShop::Save(FILE *file)
{
	char v = 5;
	fwrite((void*)&v, sizeof(char), 1, file);
	m_def->Save(file);

}

cocos2d::Scene * ModeShop::createLoadScene(CBReader *reader)
{
	ShopDef *def = new ShopDef(reader);
	Player *player = GameManager::getInstance()->getPlayer();
	Scene *scene = Scene::create();
	ModeShop *shop = ModeShop::create();
	shop->Setup(player, def, true);
	scene->addChild(shop);

	return scene;
}