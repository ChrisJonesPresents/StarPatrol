#include "ModeRecruit.h"
#include "ShopLayer.h"
#include "OfficerView.h"
#include "ModeWarpgate.h"
#include "ReferenceLibrary.h"
#include "ModeColony.h"
#include "GameManager.h"

cocos2d::Scene* ModeRecruit::createScene(Player *playerfile, ShopDef *def)
{

	auto *Scene = Scene::create();
	auto *layer = ModeRecruit::create();
	layer->Setup(playerfile, def, false);
	Scene->addChild(layer);
	return Scene;

}

cocos2d::Scene*  ModeRecruit::createSceneWarpgate(Player *playerfile)
{

	auto *Scene = Scene::create();
	auto *layer = ModeRecruit::create();
	//build shopdef
	ShopDef *def = new ShopDef();
	def->m_supplies[0][0] = 0;
	def->m_supplies[0][1] = 0;
	def->m_supplies[1][0] = 0;
	def->m_supplies[1][1] = 0;
	def->m_supplies[2][0] = 0;
	def->m_supplies[2][1] = 0;
	def->m_numitems = 0;
	def->m_numrecruits = 0;
	def->m_recruits[0] = NULL;
	def->m_recruits[1] = NULL;
	def->m_items[0] = NULL;	def->m_items[1] = NULL;
	layer->Setup(playerfile, def, true);
	Scene->addChild(layer);
	return Scene;


}

ModeRecruit::~ModeRecruit()
{
	if (m_Warpgate == true)
	{
		delete m_def;
	}
}

void ModeRecruit::BuildInventory()
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//add list of items
	m_inventory = ShopInventory::create();
	m_inventory->setPosition(origin.x + visibleSize.width - 32, origin.y + (visibleSize.height / 2));
	//setup inventory
	Sprite **sprites = new Sprite *[m_def->m_numrecruits];

	for (int i = 0; i < m_def->m_numrecruits; i++)
	{
		char buffer[5];

		switch (m_def->m_recruits[0][i])
		{
		case 0:
			//is a recruit
			sprites[i] = Sprite::create("art/awayteamsprites.png", 
				Rect(ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_graphic.x * 16, ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_graphic.y * 16,
				ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_size.x * 16, ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_size.y * 16));
			if (m_Warpgate == true)
			{
				_itoa(ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_cost*2, &buffer[0], 10);
			}
			else
			{
				_itoa(ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_cost, &buffer[0], 10);
			}
			sprites[i]->setScale(2);
			break;
		case 2:
			//is an officer
			sprites[i] = Sprite::create("art/officers.png", 
				Rect(ReferenceLibrary::getInstance()->getOfficer(m_def->m_recruits[1][i])->getPortrait().x * 128, ReferenceLibrary::getInstance()->getOfficer(m_def->m_recruits[1][i])->getPortrait().y * 128, 128, 128));
			_itoa(ReferenceLibrary::getInstance()->getOfficer(m_def->m_recruits[1][i])->getCost(), &buffer[0], 10);
			break;
		case 1:
			//is an item
			sprites[i] = Sprite::create("art/potions.png", Rect(ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_graphicpos.x * 32, ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_graphicpos.y * 32, 32, 32));
			_itoa(ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_cost, &buffer[0], 10);
			break;
		}
		Label *label = Label::createWithBMFont("art/font.fnt", buffer);
		label->setPosition(Vec2(0, -12));
		label->setAnchorPoint(Vec2(0, 0.5F));
		label->setSystemFontSize(8);

		sprites[i]->addChild(label);
	}
	m_inventory->Setup(sprites, m_def->m_numrecruits);
	this->addChild(m_inventory);
}

bool ModeRecruit::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_active = true;
	m_handindex = 0;
	m_hand = NULL;
	m_discard = 0;

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *spr3 = Sprite::create("art/gridblue.png");
	spr3->setPosition(origin.x + (visibleSize.width / 2) - 134, origin.y + (visibleSize.height / 2));
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

	Sprite *m_descriptionframe;
	cocos2d::ui::Button *buttons[4];
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
	m_count->setPosition(spr->getPosition() + Vec2(62, 0));
	m_count->setAnchorPoint(Vec2(1, 0.5F));
	m_count->setSystemFontSize(18);
	this->addChild(m_count);

	//add leave button here
	ui::Button *button = ui::Button::create("art/colonybuttond0.png", "art/colonybuttond1.png", "art/bluebutton2.png");
	button->addClickEventListener(CC_CALLBACK_1(ModeRecruit::ButtonCallback0, this));
	button->setPosition(spr2->getPosition() + Vec2(-64, 48));
	button->setScale(1);
	this->addChild(button);

	Sprite *spr4 = Sprite::create("art/shipui.png", Rect(224, 160, 32, 32));
	spr4->setPosition(origin.x + 16, origin.y + 46);
	this->addChild(spr4);
	spr = Sprite::create("art/backbox.png");
	spr->setAnchorPoint(Vec2(0, 0.5F));
	spr->setPosition(origin.x + 32, origin.y + 46);
	spr->setScaleX(1.15F);
	this->addChild(spr);
	m_money = Label::createWithBMFont("art/font.fnt", "100");
	m_money->setPosition(spr->getPosition() + Vec2(62, 0));
	m_money->setAnchorPoint(Vec2(1, 0.5F));
	m_money->setSystemFontSize(18);

	this->addChild(m_money);

	spr = Sprite::create("art/vbar.png");
	spr->setPosition(origin.x + visibleSize.width - 134, origin.y + (visibleSize.height / 2));
	spr->setScaleY(160);
	this->addChild(spr);

	return true;
}



void ModeRecruit::RefreshRoster()
{
	m_view.RefreshRoster();
}

void ModeRecruit::RefreshMoney()
{
	char buffer[6];
	itoa(m_view.m_player->getMoney(), &buffer[0], 10);
	m_money->setString(buffer);
}

void ModeRecruit::Setup(Player *playerfile, ShopDef *def, bool warpgate)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_Warpgate = warpgate;
	m_def = def;
	playerfile->CompactPotions();
	m_view.Setup(playerfile, this);

	RefreshMoney();
	BuildInventory();
}



void ModeRecruit::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
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
	}

	if (m_hand == NULL)
	{
		
	}

	if (m_hand == NULL)
	{
		if (m_inventory->m_length > 0)
		{
			for (int i = 0; i < m_inventory->m_length; i++)
			{
				Rect box = m_inventory->m_sprites[i]->getBoundingBox();
				box.origin.x += m_inventory->getPositionX();
				box.origin.y += m_inventory->getPositionY();
				if (box.containsPoint(touch->getLocation()))
				{
					char buffer[4];
					switch (m_def->m_recruits[0][i])
					{
					case 0:
						//recruit
						if (ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_cost <= m_view.m_player->getMoney() && m_Warpgate==false)
						{
							m_hand = Sprite::create("art/awayteamsprites.png",
								Rect(ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_graphic.x * 16, ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_graphic.y * 16,
								ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_size.x * 16, ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_size.y * 16));
							m_handindex = (i + 1)*-1;
							this->addChild(m_hand);
							m_hand->setScale(2);
						}
						if (ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_cost*2 <= m_view.m_player->getMoney() && m_Warpgate == true)
						{
							m_hand = Sprite::create("art/awayteamsprites.png",
								Rect(ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_graphic.x * 16, ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_graphic.y * 16,
								ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_size.x * 16, ReferenceLibrary::getInstance()->getCrew(m_def->m_recruits[1][i])->m_size.y * 16));
							m_handindex = (i + 1)*-1;
							this->addChild(m_hand);
							m_hand->setScale(2);
						}
						break;

					case 1:
						if (ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_cost <= m_view.m_player->getMoney())
						{
							m_hand = Sprite::create("art/potions.png", Rect(ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_graphicpos.x * 32, ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_graphicpos.y * 32, 32, 32));
							m_handindex = (i*-1)-1;
							this->addChild(m_hand);
							m_hand->setScale(2);
							m_description->setString(ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][i])->m_description);
						}	
						break;

					case 2:
						//officer
						if (ReferenceLibrary::getInstance()->getOfficer(m_def->m_recruits[1][i])->getCost() <= m_view.m_player->getMoney())
						{
							Sprite::create("art/officers.png",
								Rect(ReferenceLibrary::getInstance()->getOfficer(m_def->m_recruits[1][i])->getPortrait().x * 128, ReferenceLibrary::getInstance()->getOfficer(m_def->m_recruits[1][i])->getPortrait().y * 128, 128, 128));
							m_handindex = (i + 1)*-1;
							this->addChild(m_hand);
						}
						break;
					}

				}
			}
		}
	}
}
void ModeRecruit::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_hand != NULL)
	{
		m_hand->setPosition(touch->getLocation());
	}
}
void ModeRecruit::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];

	if (m_hand != NULL)
	{
		if (m_handindex > 0)
		{
			if (m_view.m_bin->getBoundingBox().containsPoint(touch->getLocation()))
			{
				m_view.DiscardPotion(m_handindex - 1,this);
				m_hand = NULL;
				m_handindex = 0;
			}
			if (m_hand != NULL)
			{
				for (int i = 0; i < 6; i++)
				{
					if (m_view.m_frames[i]->getBoundingBox().containsPoint(touch->getLocation()))
					{
						m_view.MovePotion(i, m_handindex - 1, this);
					}
				}

			}
			if (m_hand != NULL)
			{
				m_hand->setPosition(m_view.m_frames[m_handindex - 1]->getPosition());
				m_hand = NULL;
				m_handindex = 0;
			}
		}
		if (m_handindex < 0 && m_hand!=NULL)
		{
			if (m_def->m_recruits[0][(m_handindex + 1)*-1] == 0)
			{
			
			}
			if (m_def->m_recruits[0][(m_handindex + 1)*-1] == 1)
			{
				//items
				for (int i = 0; i < 6; i++)
				{
					if (m_view.m_frames[i]->getBoundingBox().containsPoint(touch->getLocation()))
					{
						if (m_view.m_player->getPotion(i) != NULL)
						{
							if (m_view.m_player->getPotion(i)->m_potion == ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][(m_handindex*-1)-1]) && m_view.m_player->getPotion(i)->m_stack<9)
							{
								//subtract money
								m_view.m_player->setMoney(m_view.m_player->getMoney() - ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][(m_handindex*-1)-1])->m_cost);
								char buffer[5];
								itoa(m_view.m_player->getMoney(), &buffer[0], 10);
								m_money->setString(buffer);
								//add one to stack
								m_view.m_player->getPotion(i)->m_stack++;
								//amend label
								itoa(m_view.m_player->getPotion(i)->m_stack, &buffer[0], 10);
								m_view.m_potionlabels[i]->setString(buffer);
								//destroy hand sprite
								this->removeChild(m_hand);
								m_hand = NULL;
								m_handindex = 0;
							}
						}
						else
						{
							//subtract money
							m_view.m_player->setMoney(m_view.m_player->getMoney() - ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][(m_handindex*-1)-1])->m_cost);
							char buffer[5];
							itoa(m_view.m_player->getMoney(), &buffer[0], 10);
							m_money->setString(buffer);
							//add sprite to slot
							m_view.m_potions[i] = m_hand;
							m_hand->setPosition(m_view.m_frames[i]->getPosition());
							
							m_view.m_player->setPotion(new PotionStack(1, ReferenceLibrary::getInstance()->getPotion(m_def->m_recruits[1][(m_handindex*-1) - 1])),i);

							//add label
							itoa(m_view.m_player->getPotion(i)->m_stack, &buffer[0], 10);
							m_view.m_potionlabels[i] = Label::createWithBMFont("art/font.fnt", buffer);
							this->addChild(m_view.m_potionlabels[i]);
							m_view.m_potionlabels[i]->setPosition(m_view.m_frames[i]->getPosition());
							//clear hand sprite
							m_hand = NULL;
						}
					}
				}
			}
			if (m_def->m_recruits[0][(m_handindex + 1)*-1] == 2)
			{
				//officers
			}

		}
		this->removeChild(m_hand);
		m_hand = NULL;
	}

}

void  ModeRecruit::ButtonCallback0(Ref *pSender)
{
	if (m_active == true)
	{
		//exit
		if (m_Warpgate == true)
		{
			auto *gate = ModeWarpgate::createScene(m_view.m_player);

			Director::sharedDirector()->replaceScene(gate);
		}
		else
		{
			//transition to the colony...somehow
			auto *colony = ModeColony::createScene(m_view.m_player,GameManager::getInstance()->findColony(m_view.m_player->getPos0().y,m_view.m_player->getPos0().x));

			Director::sharedDirector()->replaceScene(colony);
		}
	}
}

void ModeRecruit::ReturntoLayer()
{
	m_active = true;
	this->setTouchEnabled(true);
	if (m_discard > 0 && m_handindex>=0)
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
	
		}

		m_discard = 0;
		m_handindex = 0;
	}
	if (m_discard > 0 && m_handindex < 0)
	{
		int v = m_handindex + 1; v = v*-1;
		Crew *crew = ReferenceLibrary::getInstance()->getCrew(v);
		if (m_view.m_player->getShip()->HireCrew(crew, m_discard))
		{
			//subtract cost from money
			int cost = m_discard*crew->m_cost;
			if (m_Warpgate == true)
			{
				cost = cost * 2;
			}
			m_view.m_player->setMoney(m_view.m_player->getMoney() - cost);
			m_view.RefreshRoster();
			RefreshMoney();


		}

	}
}