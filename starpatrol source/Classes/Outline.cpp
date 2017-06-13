#include "Outline.h"
#include "ReferenceLibrary.h"

cocos2d::Vec2 Outline::getModulePos(int i, int type)
{
	switch (type)
	{
	case 0:
		switch (i)
		{
		case 0:
			return cocos2d::Vec2(32, -16);
		case 1:
			return cocos2d::Vec2(32, 16);
		case 2:
			return cocos2d::Vec2(0, -16);
		case 3:
			return cocos2d::Vec2(0, 16);
		case 4:
			return cocos2d::Vec2(-32, -16);
		case 5:
			return cocos2d::Vec2(-32, 16);
		}
		break;
	case 1:
		switch (i)
		{
		case 0:
			return cocos2d::Vec2(32, -16);
		case 1:
			return cocos2d::Vec2(32, 16);
		case 2:
			return cocos2d::Vec2(0, -16);
		case 3:
			return cocos2d::Vec2(0, 16);
		case 4:
			return cocos2d::Vec2(-32, -16);
		case 5:
			return cocos2d::Vec2(-32, 16);
		case 6:
			return cocos2d::Vec2(64, -16);
		case 7:
			return cocos2d::Vec2(64, 16);
		case 8:
			return cocos2d::Vec2(96, 0);
		}
		break;
	case 2:
		switch (i)
		{
		case 0:
			return cocos2d::Vec2(64, -16);
		case 1:
			return cocos2d::Vec2(64, 16);
		case 2:
			return cocos2d::Vec2(32, -16);
		case 3:
			return cocos2d::Vec2(32, 16);
		case 4:
			return cocos2d::Vec2(0, -16);
		case 5:
			return cocos2d::Vec2(0, 16);

		case 6:
			return cocos2d::Vec2(-32, -16);
		case 7:
			return cocos2d::Vec2(-32, 16);
		case 8:
			return cocos2d::Vec2(-64, -16);
		case 9:
			return cocos2d::Vec2(-64, 16);
		case 10:
			return cocos2d::Vec2(-96, -16);
		case 11:
			return cocos2d::Vec2(-96, 16);
		}
		break;
	}
}

Outline::~Outline()
{
	delete[]m_slots;
	delete[]m_modulesprites;
}

void Outline::Setup(Playership *ship, cocos2d::Layer *layer,Player *player,bool cansell)
{
	m_player = player;
	m_cansell = cansell;
	m_heldsprite = -1;
	Size visibleSize = Size(800, 640);
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//build background
	Sprite *outline = NULL;
	switch (ship->getS())
	{
	case 0:
		outline = Sprite::create("art/shipUI.png", Rect(0, 0, 128, 128));
		break;
	case 1:
		outline = Sprite::create("art/shipUI.png", Rect(128, 0, 128, 128));
		break;
	case 2:
		outline = Sprite::create("art/shipUI.png", Rect(0, 128, 128, 128));
		break;
	}
	outline->setPosition(cocos2d::Vec2(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height*0.65F)));
	outline->setScale(4);
	layer->addChild(outline);
	//build slots
	m_slots = new Sprite *[ship->getNumModules()];
	m_modulesprites = new Sprite *[ship->getNumModules()];
	for (int i = 0; i < ship->getNumModules(); i++)
	{
		m_slots[i] = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
		m_slots[i]->setPosition((getModulePos(i, ship->getS()) * 2) + outline->getPosition());
		m_slots[i]->setScale(2);
		m_slots[i]->setZOrder(0);
		layer->addChild(m_slots[i]);

		//build module symbols
		if (ship->getModule(i) != NULL)
		{
			Module *m = ship->getModule(i);
			m_modulesprites[i] = Sprite::create("art/moduleicons.png", Rect(32 * m->getGraphicpos().x, 32 * m->getGraphicpos().y, 32, 32));
			m_modulesprites[i]->setPosition(m_slots[i]->getPosition());
			m_modulesprites[i]->setScale(2);
			m_modulesprites[i]->setZOrder(1);
			layer->addChild(m_modulesprites[i]);
			if (m->getType() == MT_COMMODITY)
			{
				Module_commodity *mc = (Module_commodity*)m;
				//if module is a commodity, build label as well
				char b[3];
				itoa(mc->getStack(), &b[0], 10);
				cocos2d::Label *label = Label::createWithBMFont("art/font.fnt", b);
				label->setPosition(16, 16);
				label->setScale(0.5F);
				m_modulesprites[i]->addChild(label,0,50);
			}

		}
		else
		{
			m_modulesprites[i] = NULL;
		}

	}
	if (m_cansell == true)
	{
		m_bin = Sprite::create("art/shipui.png", Rect(224, 160, 32, 32));
	}
	else
	{
		m_bin = Sprite::create("art/shipui.png", Rect(128, 224, 32, 32));
	}

	m_bin->setScale(2);
	m_bin->setPosition(origin.x + (visibleSize.width/2) + 128, origin.y + 192);
	layer->addChild(m_bin);

	m_fuelbin = Sprite::create("art/shipui.png", Rect(224, 128, 32, 32));
	m_fuelbin->setScale(2);
	m_fuelbin->setPosition(origin.x + (visibleSize.width / 2) - 128, origin.y + 192);
	layer->addChild(m_fuelbin);


	for (int i = 0; i < 3; i++)
	{
		m_itemslots[i] = Sprite::create("art/shipUI.png", Rect(128, 128, 32, 32));
		m_itemslots[i]->setPosition(cocos2d::Vec2(origin.x + (visibleSize.width / 2) - 64 + (i * 64), origin.y + 192));

		m_itemslots[i]->setScale(2);
		layer->addChild(m_itemslots[i]);
		if (player->getItem(i) != NULL)
		{
			m_items[i] = Sprite::create("art/itemsprites.png", Rect((player->getItem(i)->m_type%8) * 32, (player->getItem(i)->m_type/8)*32, 32, 32));
			m_items[i]->setPosition(m_itemslots[i]->getPosition());
			layer->addChild(m_items[i]);
		}
		else
		{
			m_items[i] = NULL;
		}
	}

	//put icons for resources under the item row
	Sprite  *sprites[4];
	Label *labels[4];
	for (int i = 0; i < 4; i++)
	{
		sprites[i] = Sprite::create("art/colonyUI.png", Rect(0, 32 * i, 32, 32));
		sprites[i]->setPosition(origin.x + (visibleSize.width / 2) - 102 + (64 * i), origin.y + 146);
		layer->addChild(sprites[i]);
		char b[3];
		itoa(m_player->getShip()->getResource(i), &b[0], 10);
		labels[i] = Label::createWithBMFont("art/font.fnt", b);
		labels[i]->setPosition(origin.x + (visibleSize.width / 2) - 80 + (64 * i), origin.y + 146);
		layer->addChild(labels[i]);
	}

}

void Outline::BinThing(Layer *layer)
{
	if (m_heldsprite < 20)
	{
		Module *m = m_player->getShip()->getModule(m_heldsprite);
		if (m_cansell == true)
		{
			m_player->setMoney(m_player->getMoney() + (m->getCost() / 2));
		}
		m_player->getShip()->InstallModule(NULL, m_heldsprite);
		layer->removeChild(m_modulesprites[m_heldsprite]);
		m_modulesprites[m_heldsprite] = NULL;
	}
	else
	{
		Item *item = m_player->getItem(m_heldsprite - 20);
		if (m_cansell == true)
		{
			m_player->setMoney(m_player->getMoney() + (item->m_cost / 2));
		}
		delete m_player->getItem(m_heldsprite - 20);
		m_player->setItem(m_heldsprite - 20, NULL);
	}
}

void Outline::ResetPosition()
{
	if (m_heldsprite < 20)
	{
		m_modulesprites[m_heldsprite]->setPosition(m_slots[m_heldsprite]->getPosition());
	}
	else
	{
		m_items[m_heldsprite - 20]->setPosition(m_itemslots[m_heldsprite - 20]->getPosition());
	}
}

bool Outline::MoveModule(Vec2 p)
{
	//check to drop modules into fuel tank
	if (m_player->getShip()->getModule(m_heldsprite)->getType() == MT_COMMODITY)
	{
		Module_commodity *mc = (Module_commodity*)m_player->getShip()->getModule(m_heldsprite);
		if (mc->getCargoType() == CT_FUEL)
		{
			if (m_fuelbin->getBoundingBox().containsPoint(p))
			{
				//refuel ship
				int f = m_player->getShip()->Refuel(mc->getStack());
				if (f > 0)
				{
					mc->setStack(f);
				}
				else
				{
					m_player->getShip()->modFuel(mc->getStack());
					m_player->getShip()->InstallModule(NULL, m_heldsprite);
					m_modulesprites[m_heldsprite]->getParent()->removeChild(m_modulesprites[m_heldsprite]);
					m_modulesprites[m_heldsprite] = NULL;
				}
				return true;
			}
		}
		if (mc->getCargoType() == CT_ORDINANCE)
		{




		}

		for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
		{
			if (m_player->getShip()->getModule(i) != NULL && m_slots[i]->getBoundingBox().containsPoint(p))
			{
				if (m_player->getShip()->getModule(i)->getType() == MT_COMMODITY)
				{
					if (m_player->getShip()->InstallModule(m_player->getShip()->getModule(m_heldsprite), i))
					{
						//remove held stack sprite
						m_modulesprites[m_heldsprite]->getParent()->removeChild(m_modulesprites[m_heldsprite]);
						m_modulesprites[m_heldsprite] = NULL;
						m_player->getShip()->InstallModule(NULL, m_heldsprite);
						m_heldsprite = -1;
						//amend sprite for target stack
						
						if (m_modulesprites[i] != NULL)
						{
							Label *label = (Label*)m_modulesprites[i]->getChildByTag(50);
							char buffer[4];
							Module_commodity *mo = (Module_commodity*)m_player->getShip()->getModule(i);
							itoa(mo->getStack(), &buffer[0], 10);
							label->setString(buffer);
						}
	
						return true;

					}
					else
					{
						//amend both stack sprites and reset position of originator stack
						if (m_modulesprites[i] != NULL)
						{
							Label *label0 = (Label*)m_modulesprites[i]->getChildByTag(50);
							char buffer[4];
							Module_commodity *mo = (Module_commodity*)m_player->getShip()->getModule(i);
							itoa(mo->getStack(), &buffer[0], 10);
							label0->setString(buffer);
						}
						if (m_modulesprites[m_heldsprite] != NULL)
						{
							char buffer[4];
							Label *label1 = (Label*)m_modulesprites[m_heldsprite]->getChildByTag(50);
							itoa(mc->getStack(), &buffer[0], 10);
							label1->setString(buffer);

						}
	
						ResetPosition();

						return true;
					}

				}
			}
		}
	}


		for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
		{
			if (m_slots[i]->getBoundingBox().containsPoint(p))
			{
				if (m_player->getShip()->getModule(i) == NULL)
				{
					if (m_player->getShip()->InstallModule(m_player->getShip()->getModule(m_heldsprite), i))
					{
						m_player->getShip()->InstallModule(NULL, m_heldsprite, false);
						m_modulesprites[i] = m_modulesprites[m_heldsprite];
						m_modulesprites[i]->setPosition(m_slots[i]->getPosition());
						m_modulesprites[m_heldsprite] = NULL;
						m_heldsprite = -1;
						return true;
					}
				}
				break;
			}
		}
		if (m_heldsprite != -1)
		{
			ResetPosition();
		}
	
	return false;
}

void Outline::ReLabelCommodity(int i)
{
	Label *label = (Label*)m_modulesprites[i]->getChildByTag(50);
	Module_commodity *mc = NULL;
	if (m_player->getShip()->getModule(i)->getType() == MT_COMMODITY)
	{
		mc = (Module_commodity*)m_player->getShip()->getModule(i);
	}
	if (mc != NULL && label != NULL)
	{
		char buffer[4];
		itoa(mc->getStack(), &buffer[0], 10);
		label->setString(buffer);
	}
}

bool Outline::AddPurchase(int type, int *value,Vec2 p,Sprite *sprite)
{
	if (type == 1)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_itemslots[i]->getBoundingBox().containsPoint(p) && m_items[i] == NULL)
			{
				m_items[i] = sprite;
				m_items[i]->setPosition(m_itemslots[i]->getPosition());
				m_player->setItem(i, ReferenceLibrary::getInstance()->getItem(*value));
					return true;
			}
		}
	}

	if (type >= 2)
	{
		for (int i = 0; i < m_player->getShip()->getNumModules(); i++)
		{
			if (m_slots[i]->getBoundingBox().containsPoint(p))
			{
				if (type == 2)
				{	
					if (m_modulesprites[i] == NULL && m_player->getShip()->getModule(i) == NULL)
					{
						if (m_player->getShip()->InstallModule(ReferenceLibrary::getInstance()->getModule(*value), i))
						{
							*value = 0;
							m_modulesprites[i] = sprite;
							m_modulesprites[i]->setPosition(m_slots[i]->getPosition());
							return true;
						}
					}
				}
				if (type == 3)
				{
					Module_commodity *mc = new Module_commodity(CT_FUEL, *value);
					m_player->getShip()->InstallModule(mc, i);
					if (mc != m_player->getShip()->getModule(i))
					{
						if (mc->getStack() <= 0)
						{
		
							*value = 0;
						}
						else
						{
							*value = mc->getStack();
						}
						delete mc;
						sprite->getParent()->removeChild(sprite);

					}
					else
					{
						*value = 0;
						m_modulesprites[i] = sprite;
						m_modulesprites[i]->setPosition(m_slots[i]->getPosition());
					}
					ReLabelCommodity(i);
					return true;
				}
				if (type == 4)
				{
					Module_commodity *mc = new Module_commodity(CT_ORDINANCE, *value);
					m_player->getShip()->InstallModule(mc, i);
					if (mc != m_player->getShip()->getModule(i))
					{
						if (mc->getStack() <= 0)
						{

							*value = 0;
						}
						else
						{
							*value = mc->getStack();
						}
						delete mc;
						sprite->getParent()->removeChild(sprite);
					}
					else
					{
						*value = 0;
						m_modulesprites[i] = sprite;
						m_modulesprites[i]->setPosition(m_slots[i]->getPosition());
					}
					ReLabelCommodity(i);
					return true;
				}
			}
		}
	}

	return false;
}