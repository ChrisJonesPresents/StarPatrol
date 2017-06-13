#include "FieldUnit.h"

FieldUnit::FieldUnit()
{
	m_sprites = NULL;
}

void FieldUnit::Setup(Vec2 graphic, Vec2 size, bool individual, bool hard, bool heavy, int hp, int attack, int defence, FieldType type)
{
	m_attack = attack; m_defence = defence; m_hp = hp;
	m_graphic = graphic;
	m_graphicsize = size;
	m_individual = individual;
	m_type = type;
	for (int i = 0; i < 4; i++)
	{
		m_status[i].m_sprite = NULL;
	}
	m_heavy = heavy;
	m_hard = hard;
}

void FieldUnit::Setup(CBReader *reader, int index, int hp)
{
	reader->setReadposition(0);
	while (true)
	{
		std::string str = reader->ReadString();
		if (str.find("ALIEN") != std::string::npos)
		{
			if (reader->ReadInt() == index)
			{
				break;
			}
		}
	}
	m_name = "";
	while (true)
	{
		std::string str = reader->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_name.append(str + " ");
		}
	}
	std::string str = reader->ReadString();
	m_type=StrtoFieldType(str);
	m_attack = reader->ReadInt();
	m_defence = reader->ReadInt();
	m_individual = reader->ReadInt();
	m_hard = reader->ReadInt();
	m_heavy = reader->ReadInt();
	m_graphic = Vec2(reader->ReadInt(), reader->ReadInt());
	m_graphicsize = Vec2(reader->ReadInt(), reader->ReadInt());
	for (int i = 0; i < 4; i++)
	{
		m_status[i].m_sprite = NULL;
	}
	m_hp = hp;
}

void FieldUnit::Gensprites()
{
	if (m_individual == false)
	{
		m_sprites = new Sprite *[m_hp];
		m_numsprites = m_hp;
		for (int i = 0; i < m_hp; i++)
		{
			m_sprites[i] = Sprite::create("art/awayteamsprites.png", Rect(16 * m_graphic.x, 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
			m_sprites[i]->setPosition(0, (m_hp) - (i *2));
			m_sprites[i]->setPositionX(((i%4)*16)-8+rand()%16);
			m_sprites[i]->setZOrder(i/4);
			this->addChild(m_sprites[i]);
		}

	}
	else
	{
		m_sprites = new Sprite *[1];
		m_numsprites = 1;
		m_sprites[0] = Sprite::create("art/awayteamsprites.png", Rect(16 * m_graphic.x, 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
		this->addChild(m_sprites[0]);
	}
}

FieldType FieldUnit::getFieldType()
{
	return m_type;
}

FieldUnit::~FieldUnit()
{
	if (m_sprites != NULL)
	{
		delete[] m_sprites;
	}

}

void FieldUnit::Add(int hp)
{

	m_hp = m_hp+hp;
}

Vec2 FieldUnit::getGraphic()
{
	return m_graphic;
}

void FieldUnit::ModAttack(int attack)
{
	m_attack = m_attack + attack;
}
void FieldUnit::ModDefence(int defence)
{
	m_defence = m_defence + defence;
}

int FieldUnit::getAttack()
{
	return m_attack;
}
int FieldUnit::getDefence()
{
	return m_defence;
}
int FieldUnit::getHP()
{
	return m_hp;
}
bool FieldUnit::getIndividual()
{
	return m_individual;
}
int FieldUnit::Attack(int defence, int count)
{
	count = count / 2; 
	if (count <= 0)
	{
		count = 1;
	}
	if (m_individual==true )
	{
		//roll once, subtract defence
		int r = rand() % 8;
		int damage = r + m_attack - defence-3;
		//play animation
		Animation *an = Animation::create();
		for (int i = 0; i < 4; i++)
		{
			SpriteFrame *frame = SpriteFrame::create("art/awayteamsprites.png", Rect(16 * m_graphic.x+((i)*m_graphicsize.x*16), 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
			an->addSpriteFrame(frame);
		}
		an->setDelayPerUnit(0.1F);
		m_sprites[0]->runAction(Animate::create(an));

		//return roll - defence
		return damage;
	}
	else
	{
		//for each guy who is visible and not performing an animation
		int total = 0;
		//roll attack vs defence
		//if success, add result to total
		//and trigger animation
		//return total
		int c = 0;
		for (int i = 0; i < m_numsprites; i++)
		{
			if (m_sprites[i]->isVisible() && m_sprites[i]->numberOfRunningActions()==0)
			{
				if (c<count)
				{
					c++;
					int r = rand() % 8;
					if (r + m_attack>(defence + 5))
					{
						total++;

					}
				}

				//play animation
				Animation *an = Animation::create();
				for (int j = 0; j < 4; j++)
				{
					SpriteFrame *frame = SpriteFrame::create("art/awayteamsprites.png", Rect(16 * m_graphic.x + ((j)*m_graphicsize.x * 16), 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
					an->addSpriteFrame(frame);
				}
				SpriteFrame *frame = SpriteFrame::create("art/awayteamsprites.png", Rect(16 * m_graphic.x, 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
				an->addSpriteFrame(frame);
				an->setDelayPerUnit(0.1F);
				m_sprites[i]->runAction(Animate::create(an));
			}
	
		}

		return total;
	}
}

bool FieldUnit::Damage(int damage)
{
	if (m_individual == true)
	{
		//subtract damage from hp
		m_hp = m_hp - damage;
		//if hp equal to or less than zero play death animation
		if (m_hp <= 0)
		{
			Animation *an = Animation::create();
			for (int j = 0; j < 4; j++)
			{
				SpriteFrame *frame = SpriteFrame::create("art/awayteamsprites.png", Rect(16 * m_graphic.x + ((j + 4)*m_graphicsize.x * 16), 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
				an->addSpriteFrame(frame);
			}

			an->setDelayPerUnit(0.1F);
			Vector <FiniteTimeAction *> actions;
			actions.pushBack(Animate::create(an));
			actions.pushBack(ToggleVisibility::create());
			m_sprites[0]->runAction(Sequence::create(actions));


			true;
		}
		//and return true
		//else return false

	}
	else
	{
		//subtract damage from hp
		m_hp = m_hp - damage;
		//find damage worth of guys visible and not currently performing an action
		//play death animation on those guys
		//if unit has no hp left, return true
		int index = 0;
		for (int i = 0; i < m_numsprites; i++)
		{
			if (m_sprites[i]->isVisible() && index<damage)
			{
				int r = rand() % 2;
				if (r == 1)
				{
					index++;
					Animation *an = Animation::create();
					for (int j = 0; j < 4; j++)
					{
						SpriteFrame *frame = SpriteFrame::create("art/awayteamsprites.png", Rect(16 * m_graphic.x + ((j + 4)*m_graphicsize.x * 16), 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
						an->addSpriteFrame(frame);
					}

					an->setDelayPerUnit(0.1F);
					Vector <FiniteTimeAction *> actions;
					actions.pushBack(Animate::create(an));
					actions.pushBack(ToggleVisibility::create());
					m_sprites[i]->runAction(Sequence::create(actions));
				}
			}
		}
		if (index < damage)
		{
			for (int i = 0; i < m_numsprites; i++)
			{
				if (m_sprites[i]->isVisible() && m_sprites[i]->numberOfRunningActions() == 0)
				{
					index++;
					Animation *an = Animation::create();
					for (int j = 0; j < 4; j++)
					{
						SpriteFrame *frame = SpriteFrame::create("art/awayteamsprites.png", Rect(16 * m_graphic.x + ((j + 4)*m_graphicsize.x * 16), 16 * m_graphic.y, 16 * m_graphicsize.x, 16 * m_graphicsize.y));
						an->addSpriteFrame(frame);
					}

					an->setDelayPerUnit(0.1F);
					Vector <FiniteTimeAction *> actions;
					actions.pushBack(Animate::create(an));
					actions.pushBack(ToggleVisibility::create());
					m_sprites[i]->runAction(Sequence::create(actions));
					if (index == damage)
					{
						break;
					}
				}
			}
		}

		if (m_hp <= 0)
		{
			return true;
		}
		//else return false
	}
	return false;


}

bool FieldUnit::Damage(int damage, bool heavy)
{
	if (heavy == m_hard)
	{
		damage = damage * 2;
	}
	return Damage(damage);
}

void FieldUnit::Effect(Vec2 target, Vec2 scatter, float s)
{
	if (target.x >= 0)
	{
		for (int i = 0; i < m_numsprites; i++)
		{
			if (m_sprites[i]->isVisible())
			{
				Sprite *spr = Sprite::create("art/awayteameffects.png", Rect(target.x * 16, target.y * 16, 16, 16));
				spr->setPosition(m_sprites[i]->getPosition());
				spr->setScale(s);
				spr->setZOrder(m_hp);
				this->addChild(spr);
				Animation *an = Animation::create();
				for (int i = 0; i < 4; i++)
				{
					SpriteFrame*frame = SpriteFrame::create("art/awayteameffects.png", Rect((target.x + i) * 16, target.y * 16, 16, 16));
					an->addSpriteFrame(frame);
				}
				an->setDelayPerUnit(0.2F);
				Vector<FiniteTimeAction*> actions;
				actions.pushBack(Animate::create(an));
				actions.pushBack(RemoveSelf::create());
				spr->runAction(Sequence::create(actions));
			}
		}
	}
	if (scatter.x >= 0)
	{
		for (int i = 0; i < m_hp/2; i++)
		{
			Sprite *spr = Sprite::create("art/awayteameffects.png", Rect(scatter.x * 16, scatter.y * 16, 16, 16));
			spr->setPosition(0+(rand()%16)*2,-16+(rand()%16)*2);
			spr->setScale(s);
			spr->setZOrder(m_hp);
			this->addChild(spr);
			Animation *an = Animation::create();
			for (int i = 0; i < 4; i++)
			{
				SpriteFrame*frame = SpriteFrame::create("art/awayteameffects.png", Rect((scatter.x + i) * 16, scatter.y * 16, 16, 16));
				an->addSpriteFrame(frame);
			}
			an->setDelayPerUnit(0.2F);
			Vector<FiniteTimeAction*> actions;
			actions.pushBack(Animate::create(an));
			actions.pushBack(RemoveSelf::create());
			spr->runAction(Sequence::create(actions));
		}
	}
}

void FieldUnit::Cooldown()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_status[i].duration>0)
		{
			m_status[i].duration--;
			if (m_status[i].duration == 0)
			{
				this->removeChild(m_status[i].m_sprite);
				m_status[i].m_sprite = NULL;
				switch (m_status[i].m_modifies)
				{
				case FM_ATTACK:
					m_attack -= m_status[i].strength;
					break;
				case FM_DEFENCE:
					m_defence -= m_status[i].strength;
					break;
				}
			}
		}

	}
}
void FieldUnit::AddStatus(int strength, int duration, FieldMod type)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_status[i].m_sprite == NULL)
		{
			m_status[i].m_modifies = type;
			m_status[i].duration = duration;
			m_status[i].strength = strength;
			switch (type)
			{
			case FM_ATTACK:
				if (strength>0)
				{
					m_status[i].m_sprite = Sprite::create("art/awayteamicons.png", Rect(0, 0, 16, 16));
				}
				else
				{
					m_status[i].m_sprite = Sprite::create("art/awayteamicons.png", Rect(0, 16, 16, 16));
				}
				m_attack += strength;
				break;
			case FM_DEFENCE:
				if (strength>0)
				{
					m_status[i].m_sprite = Sprite::create("art/awayteamicons.png", Rect(16, 0, 16, 16));
				}
				else
				{
					m_status[i].m_sprite = Sprite::create("art/awayteamicons.png", Rect(16, 16, 16, 16));
				}
				m_defence += strength;
				break;
			}
			m_status[i].m_sprite->setPosition(-16 + (16 * i), 16);
			this->addChild(m_status[i].m_sprite);
				break;
		}
	}
}

bool FieldUnit::getHeavy()
{
	return m_heavy;
}