#include "CrewView.h"


CrewView::~CrewView()
{


}


void CrewView::Setup(Player *player, cocos2d::Layer *layer, bool hasbin)
{
	m_player = player;
	Size visibleSize = Size(800, 640);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//build officers
	for (int i = 0; i < 6; i++)
	{
		m_panels[i] = Sprite::create("art/officerframe.png");
		int x = (i % 3) * 128;
		int y = (i / 3) * 192;
		m_panels[i]->setPosition(Vec2(origin.x + x+(visibleSize.width/2-128) ,origin.y+500-y));
		m_panels[i]->setZOrder(1);
		layer->addChild(m_panels[i]);
		if (m_player->getOfficer(i) != NULL)
		{
			Sprite *spr = Sprite::create("art/officers.png", Rect(m_player->getOfficer(i)->getDefinition()->getPortrait().x * 128, m_player->getOfficer(i)->getDefinition()->getPortrait().y * 128, 128, 128));
			spr->setPosition(m_panels[i]->getPosition() + Vec2(0, 32));
			layer->addChild(spr);
			std::string str = "\n level ";
			char buffer[2];
			itoa(m_player->getOfficer(i)->getLevel(), &buffer[0], 10);
			str.append(buffer);
			m_nametags[i] = Label::createWithBMFont("art/font.fnt", m_player->getOfficer(i)->getName() + str, cocos2d::CCTextAlignment::CENTER);
			m_nametags[i]->setPosition(Vec2(64, 56));
			m_nametags[i]->setAnchorPoint(Vec2(0.5F, 1.0F));
			m_nametags[i]->setWidth(128);
			m_nametags[i]->setSystemFontSize(16);
			m_panels[i]->addChild(m_nametags[i]);

			if (m_player->getOfficer(i)->CanLevel())
			{
				Sprite *spr1 = Sprite::create("art/levelup.png");
				spr1->setPosition(64, 128);
				m_panels[i]->addChild(spr1);
			}
		}
	}
	//build lists
	
	if (hasbin == true)
	{
		m_bin = Sprite::create("art/shipui.png", Rect(128, 224, 32, 32));
		m_bin->setScale(2);
		m_bin->setPosition(origin.x + 64, origin.y + 192);
		layer->addChild(m_bin);
	}
	else
	{
		m_bin = NULL;
	}

	//build potions
	for (int i = 0; i < 6; i++)
	{
		m_frames[i] = Sprite::create("art/crewpanel.png", Rect(0, 32, 32, 32));
		m_frames[i]->setPosition(Vec2(origin.x + (visibleSize.width / 2 - 162) + (64 * i), origin.y + 168));
		m_frames[i]->setScale(2);
		layer->addChild(m_frames[i]);

		if (m_player->getPotion(i) != NULL)
		{
			m_potions[i] = Sprite::create("art/potions.png", Rect(m_player->getPotion(i)->m_potion->m_graphicpos.x * 32, m_player->getPotion(i)->m_potion->m_graphicpos.y * 32,32,32));
			m_potions[i]->setPosition(m_frames[i]->getPosition());
			m_potions[i]->setScale(2);
			layer->addChild(m_potions[i]);
			m_potionlabels[i] = Label::createWithBMFont("art/font.fnt", "");
			char buffer[2];
			itoa(m_player->getPotion(i)->m_stack, &buffer[0], 10);
			m_potionlabels[i]->setPosition(m_frames[i]->getPosition());
			m_potionlabels[i]->setString(buffer);	
			layer->addChild(m_potionlabels[i]);

		}
		else
		{
			m_potions[i] = NULL;
			m_potionlabels[i] = NULL;
		}
	}

}

void CrewView::RefreshRoster()
{
	m_player->getShip()->setCrewCount();

	for (int i = 0; i < 6; i++)
	{
		m_panels[i]->removeAllChildren();
		if(m_player->getOfficer(i) != NULL)
		{
			Sprite *spr = Sprite::create("art/officers.png", Rect(m_player->getOfficer(i)->getDefinition()->getPortrait().x * 128, m_player->getOfficer(i)->getDefinition()->getPortrait().y * 128, 128, 128));
			spr->setPosition(m_panels[i]->getPosition() + Vec2(0, 32));
			m_panels[i]->getParent()->addChild(spr);
			std::string str = "\n level ";
			char buffer[2];
			itoa(m_player->getOfficer(i)->getLevel(), &buffer[0], 10);
			str.append(buffer);
			m_nametags[i] = Label::createWithBMFont("art/font.fnt", m_player->getOfficer(i)->getName() + str, cocos2d::CCTextAlignment::CENTER);
			m_nametags[i]->setPosition(Vec2(64, 56));
			m_nametags[i]->setAnchorPoint(Vec2(0.5F, 1.0F));
			m_nametags[i]->setWidth(128);
			m_nametags[i]->setSystemFontSize(16);
			m_panels[i]->addChild(m_nametags[i]);

			if (m_player->getOfficer(i)->CanLevel())
			{
				Sprite *spr1 = Sprite::create("art/levelup.png");
				spr1->setPosition(64, 128);
				m_panels[i]->addChild(spr1);
			}
		}
	
	}
}

void CrewView::DiscardPotion(int index, Layer *layer)
{
	layer->removeChild(m_potions[index]);
	layer->removeChild(m_potionlabels[index]);
	m_potions[index] = NULL;
	m_potionlabels[index] = NULL;
	delete m_player->getPotion(index);
	m_player->setPotion(NULL, index);

}

bool CrewView::MovePotion(int destination, int source, Layer *layer)
{
	if (m_potions[destination] == NULL)
	{
		m_potions[source]->setPosition(m_frames[destination]->getPosition());
		m_potions[destination] = m_potions[source];
		m_potions[source] = NULL;


		m_potionlabels[destination] = m_potionlabels[source];
		m_potionlabels[source] = NULL;
		m_potionlabels[destination]->setPosition(m_frames[destination]->getPosition());

		m_player->setPotion(m_player->getPotion(source), destination);
		m_player->setPotion(NULL, source);
		return true;
	}
	else if (m_player->getPotion(destination)->m_stack<9)
	{
		//attempt to transfer from one stack to another
		int max = 9-m_player->getPotion(destination)->m_stack;
		int move = m_player->getPotion(source)->m_stack;
		if (max >= move)
		{
			//remove original stack
			m_player->getPotion(destination)->m_stack += move;
			char buffer[3];
			itoa(m_player->getPotion(destination)->m_stack, &buffer[0], 10);
			m_potionlabels[destination]->setString(buffer);

			delete m_player->getPotion(source);
			m_player->setPotion(NULL, source);

			layer->removeChild(m_potions[source]);
			m_potions[source]=NULL;

			layer->removeChild(m_potions[source]);
			m_potionlabels[source] = NULL;
			return true;
		}
		else
		{
			m_player->getPotion(destination)->m_stack = 9;
			m_player->getPotion(source)->m_stack -= max;

			char buffer[3];
			itoa(m_player->getPotion(destination)->m_stack, &buffer[0], 10);
			m_potionlabels[destination]->setString(buffer);
			itoa(m_player->getPotion(source)->m_stack, &buffer[0], 10);

			m_potionlabels[source]->setString(buffer);
			m_potions[source]->setPosition(m_frames[source]->getPosition());

			return true;
		}
	}
	m_potions[source]->setPosition(m_frames[source]->getPosition());
	return false;
}

Potion *CrewView::TakePotion(int slot)
{
	if (m_player->getPotion(slot)->m_stack == 0)
	{
		return NULL;
	}
	m_player->getPotion(slot)->m_stack--;

	//redraw label
	char buffer[3];
	itoa(m_player->getPotion(slot)->m_stack, &buffer[0], 10);
	m_potionlabels[slot]->setString(buffer);

	return m_player->getPotion(slot)->m_potion;
}

void CrewView::ReturnPotion(Potion *potion)
{
	for (int i = 0; i < 6; i++)
	{
		if (m_player->getPotion(i) != NULL)
		{
			if (m_player->getPotion(i)->m_potion == potion)
			{
				if (m_player->getPotion(i)->m_stack < 9)
				{
					m_player->getPotion(i)->m_stack++;

					char buffer[3];
					itoa(m_player->getPotion(i)->m_stack, &buffer[0], 10);
					m_potionlabels[i]->setString(buffer);

					break;
				}
			}
		}
	}
}