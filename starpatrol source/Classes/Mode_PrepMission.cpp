#include "Mode_PrepMission.h"
#include "GameManager.h"
#include "Mode_Awayteam.h"
#include "Mode_Explore.h"
#include "FieldUnit.h"
#include "CrewFieldUnit.h"
#include "ShopLayer.h"

cocos2d::Scene* Mode_PrepMission::createScene(MissionNode *stage, Player *player)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Mode_PrepMission *layer = Mode_PrepMission::create();

	layer->Setup(stage, player);

	// add layer as a child to scene
	scene->addChild(layer,0,100);

	// return the scene
	return scene;



}

bool Mode_PrepMission::init()
{
	
	if (!Layer::init())
	{
		return false;
	}
	m_active = true;
	m_hand = NULL;
	m_handindex = 0;
	m_target = 0;
	m_amount = 0;

	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *spr3 = Sprite::create("art/gridblue.png");
	spr3->setPosition(origin.x + (visibleSize.width / 2) - 134, origin.y + (visibleSize.height / 2));
	spr3->setScale(3.125F);
	this->addChild(spr3);

	spr3 = Sprite::create("art/vbar.png");
	spr3->setPosition(origin.x + visibleSize.width - 134, origin.y + (visibleSize.height / 2));
	spr3->setScaleY(160);
	this->addChild(spr3);

	ui::Button *button[2];

	button[0] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button[0]->addClickEventListener(CC_CALLBACK_1(Mode_PrepMission::ButtonCallback0, this));
	button[0]->setPosition(Vec2(origin.x+64,origin.y+32));
	button[0]->setTitleColor(Color3B(0, 0, 0));
	button[0]->setTitleText("Abort");
	button[0]->setTitleFontSize(32);
	button[0]->setScale(1);

	button[1] = ui::Button::create("art/bluebutton0.png", "art/bluebutton1.png", "art/bluebutton2.png");
	button[1]->addClickEventListener(CC_CALLBACK_1(Mode_PrepMission::ButtonCallback1, this));
	button[1]->setPosition(Vec2(origin.x+visibleSize.width - 64, origin.y + 32));
	button[1]->setTitleColor(Color3B(0, 0, 0));
	button[1]->setTitleText("Commit");
	button[1]->setTitleFontSize(32);
	button[1]->setScale(1);
	for (int i = 0; i < 2; i++)
	{
		this->addChild(button[i]);
	}

	for (int i = 0; i < 3; i++)
	{

		m_officer[i] = NULL;



		m_officerslots[i] = Sprite::create("art/crewpanel.png",Rect(32,0,32,32));

		m_officerslots[i]->setScale(2);

		m_officerslots[i]->setPosition(origin.x + 736, origin.y + 568 - (88 * i));
	
		this->addChild(m_officerslots[i]);
	
		m_officerslots[i]->setZOrder(1);
		m_officersprites[i] = NULL;

		m_potionslots[i] = Sprite::create("art/crewpanel.png", Rect(32, 32, 32, 32));
		m_potionslots[i]->setPosition(origin.x + 736, origin.y + 300 - (88 * i));
		this->addChild(m_potionslots[i]);
		m_potionslots[i]->setScale(2);
		m_potionsprites[i] = NULL;
		m_potions[i] = NULL;
	}

	this->setTouchEnabled(true);

	Sprite *m_descriptionframe;
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

	return true;

}

void Mode_PrepMission::Setup(MissionNode *stage, Player *player)
{
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_stage = stage;
	m_view.Setup(player, this,false);




}

void Mode_PrepMission::Cleanup()
{

}

void Mode_PrepMission::ButtonCallback0(Ref *pSender)
{
	if (m_active == true)
	{
		m_view.m_player->ResetPotions(m_potions);
		Cleanup();
		Scene *scene = GameManager::getInstance()->getHandler()->getScene(1);
		Director::sharedDirector()->replaceScene(scene);

	}

}

void Mode_PrepMission::ButtonCallback1(Ref *pSender)
{
	if (m_active == true)
	{
		bool t = false;
		for (int i = 0; i < 3; i++)
		{
			if (m_officer[i] != NULL)
			{
				t = true;
			}
		}
		if (t == true)
		{
			Cleanup();
			StartMission();
		}
		else
		{
			Prompt *prompt = Prompt::create();
			prompt->Setup(this, "You can't launch an away mission that consists entirely of redshirts");
			m_active = false;
			setTouchEnabled(false);
			prompt->setTouchEnabled(true);
			this->getParent()->addChild(prompt);
		}

	}
}

void Mode_PrepMission::StartMission()
{
	m_view.m_player->PotionCheck();
	if (m_stage->m_type == MST_BATTLE)
	{
		Scene *scene = Mode_Awayteam::createScene(m_officer, m_stage,m_potions);
		Director::sharedDirector()->replaceScene(scene);
	}
	else if (m_stage->m_type==MST_EXPLORE)
	{
		Scene *scene = Mode_Explore::createScene(m_officer, m_stage,m_potions);
		Director::sharedDirector()->replaceScene(scene);
	}
	
}

void Mode_PrepMission::onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	//check if we're over an officer panel
	for (int i = 0; i < 6; i++)
	{
		if (m_view.m_panels[i]->getBoundingBox().containsPoint(touch->getLocation()) && m_view.m_player->getOfficer(i)!=NULL)
		{
			m_hand = Sprite::create("art/officers.png", Rect(m_view.m_player->getOfficer(i)->getDefinition()->getPortrait().x * 128, m_view.m_player->getOfficer(i)->getDefinition()->getPortrait().y * 128, 128, 128));
			this->addChild(m_hand);
			m_hand->setZOrder(3);
			m_hand->setScale(0.5F);
			m_handindex = i;
			break;
		}
	}

	//check for potion slots
	if (m_hand == NULL)
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_view.m_potions[i] != NULL)
			{
				if (m_view.m_potions[i]->getBoundingBox().containsPoint(touch->getLocation()) && m_view.m_player->getPotion(i)->m_stack>0)
				{
					m_hand = Sprite::create("art/potions.png", Rect(m_view.m_player->getPotion(i)->m_potion->m_graphicpos.x * 32, m_view.m_player->getPotion(i)->m_potion->m_graphicpos.y * 32, 32, 32));
					m_hand->setZOrder(3);
					m_description->setString(m_view.m_player->getPotion(i)->m_potion->m_description);
					this->addChild(m_hand);
					m_handindex = i + 20;
				}
			}
		}
	}

	//check if over potion slots
	if (m_hand == NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_potionsprites[i] != NULL)
			{
				if (m_potionsprites[i]->getBoundingBox().containsPoint(touch->getLocation()))
				{
					m_hand = m_potionsprites[i];
					m_handindex = i + 40;
					m_description->setString(m_potions[i]->m_description);
				}
			}
		}
	}

	//check if we're over an officer slot
	if (m_hand == NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_officerslots[i]->getBoundingBox().containsPoint(touch->getLocation()) && m_officer[i]!=NULL)
			{
				m_hand = Sprite::create("art/officers.png", Rect(m_officer[i]->getDefinition()->getPortrait().x * 128, m_officer[i]->getDefinition()->getPortrait().y * 128, 128, 128));
				this->addChild(m_hand);
				m_hand->setZOrder(3);
				m_hand->setScale(0.5F);
				m_handindex = i + 6;
				break;
			}
		}
	}


}

void Mode_PrepMission::onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event)
{
	Touch *touch = touches[0];
	if (m_hand != NULL)
	{
		m_hand->setPosition(touch->getLocation());
	}
}

void Mode_PrepMission::onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event)
{
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Touch *touch = touches[0];
	if (m_hand != NULL)
	{
		if (m_handindex < 6)
		{
			//from officer panel
			for (int i = 0; i < 3; i++)
			{
				if (m_officerslots[i]->getBoundingBox().containsPoint(touch->getLocation()))
				{
					if (m_officer[i] == NULL)
					{
						PlaceOfficer(m_view.m_player->getOfficer(m_handindex), i);
					}
				}
			}
		}
		else
		{
			if (m_handindex < 12)
			{
				//from officer slot
				for (int i = 0; i < 3; i++)
				{
					if (m_officerslots[i]->getBoundingBox().containsPoint(touch->getLocation()) && m_officer[i]==NULL)
					{
						PlaceOfficer(m_officer[m_handindex - 6], i);
		
					}
				}
				if (touch->getLocation().x < origin.x + 672)
				{
					//remove officer from officer slot
				
					m_officer[m_handindex - 6] = NULL;
					this->removeChild(m_officersprites[m_handindex - 6]);
					m_officersprites[m_handindex - 6] = NULL;
		
				}
			}
			else
			{
				if (m_handindex < 30)
				{
					//from potion list
					for (int i = 0; i < 3; i++)
					{
						if (m_potions[i]==NULL && m_potionslots[i]->getBoundingBox().containsPoint(touch->getLocation()))
						{
							//put potion in list
							m_potions[i] = m_view.TakePotion(m_handindex - 20);
							m_potionsprites[i] = m_hand;
							m_potionsprites[i]->setPosition(m_potionslots[i]->getPosition());
							m_potionsprites[i]->setScale(2);
							m_hand = NULL;
						}
					}
				}
				else
				{
					//from potion slot
					if (touch->getLocation().x < origin.x + 672)
					{
						//return to list
						m_view.ReturnPotion(m_potions[m_handindex - 40]);
						m_potions[m_handindex - 40] = NULL;
						this->removeChild(m_potionsprites[m_handindex - 40]);
						m_hand = NULL;
					}
				}
			}
		}
		if (m_hand != NULL)
		{
			this->removeChild(m_hand);
			m_hand = NULL;
		}

	}

}

bool Mode_PrepMission::PlaceOfficer(Officer *officer, int slot)
{
	//check for identical officer
	for (int i = 0; i < 3; i++)
	{
		if (m_officer[i] == officer)
		{
		
			m_officer[i] = NULL;
			this->removeChild(m_officersprites[i]);
			m_officersprites[i] = NULL;
		}
	}
	m_officer[slot] = officer;
	m_officersprites[slot] = Sprite::create("art/officers.png", Rect(m_officer[slot]->getDefinition()->getPortrait().x * 128, m_officer[slot]->getDefinition()->getPortrait().y * 128, 128, 128));
	m_officersprites[slot]->setPosition(m_officerslots[slot]->getPosition());
	m_officersprites[slot]->setScale(0.5F);
	
	this->addChild(m_officersprites[slot]);
	return true;
}
void Mode_PrepMission::ReturntoLayer()
{
	m_active = true;
	this->setTouchEnabled(true);
	if (m_amount > 0)
	{
	
		m_view.m_player->getShip()->getCrew()->at(m_handindex - 20)->m_count -= m_amount;
		m_view.RefreshRoster();
	}
}

void Mode_PrepMission::Save(FILE *file)
{
	char v = 7;
	fwrite((void*)&v, sizeof(char), 1, file);
}