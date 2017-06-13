#include "Enemy_Away.h"
#include "ReferenceLibrary.h"

Enemy_Data::Enemy_Data(CBReader * reader)
{

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
	//stats
	m_attack = reader->ReadInt();
	m_health = reader->ReadInt();
	m_special = reader->ReadInt();
	//defences
	m_energydef = reader->ReadInt();
	m_impactdef = reader->ReadInt();
	m_explosiondef = reader->ReadInt();
	//graphics
	m_graphics[0] = Vec2(reader->ReadInt(), reader->ReadInt());
	m_graphics[1] = Vec2(reader->ReadInt(), reader->ReadInt());
	//type
	m_type = StrtoFieldType(reader->ReadString());
	//abilities
	for (int i = 0; i < 3; i++)
	{
		int v = reader->ReadInt();
		if (v == -1)
		{
			m_abilities[i] = NULL;
		}
		else
		{
			m_abilities[i] = ReferenceLibrary::getInstance()->getActorAbility(v);
		}

	}
	for (int i = 0; i < 3; i++)
	{
		m_deaths[i] = LoadLongString(reader);
	}
}

Enemy_Away::Enemy_Away(int index, CBReader *reader)
{
	m_index = index;
	//set reader to position 0
	reader->setReadposition(0);
	//find index enemy
	while (true)
	{
		std::string str = reader->ReadString();
		if (str.find("ENEMY") != std::string::npos)
		{
			int value = reader->ReadInt();
			if (value == index)
			{
				break;
			}
		}
	}
	//load enemy data
	m_data = new Enemy_Data(reader);
	//initialize health and energy
	m_health = m_data->m_health;
	m_energy = 0;
	for (int i = 0; i < 4; i++)
	{
		m_cooldowns[i] = 0;
		m_effects[i].m_duration = 0;
	}
}

void Enemy_Away::BuildSprite()
{
	m_sprite = Sprite::create("art/awayteamenemies.png", Rect(64 * m_data->m_graphics[0].x, m_data->m_graphics[0].y * 64, m_data->m_graphics[1].x * 64, m_data->m_graphics[1].y * 64));
	m_sprite->setScale(2);
	SetupIcons();
}

Enemy_Away::~Enemy_Away()
{
	delete m_data;


}

Sprite *Enemy_Away::getSprite()
{
	return m_sprite;
}

Enemy_Data *Enemy_Away::getData()
{
	return m_data;
}

void Enemy_Away::RemoveState(Status_Effect effect)
{
	switch (effect.m_stat)
	{
		case AMT_ATTACK:
			m_data->m_attack -= effect.m_effect;
			break;

		case AMT_DEFENCE_ENE:
			m_data->m_energydef -= effect.m_effect;
			break;

		case AMT_DEFENCE_IMP:
			m_data->m_impactdef -= effect.m_effect;
			break;

		case AMT_DEFENCE_EXP:
			m_data->m_explosiondef -= effect.m_effect;
			break;

		case AMT_SPECIAL:
			m_data->m_special -= effect.m_effect;
			break;

	}

}

void Enemy_Away::AddState(Status_Effect effect)
{
	switch (effect.m_stat)
	{
	case AMT_ATTACK:
		m_data->m_attack += effect.m_effect;
		break;

	case AMT_DEFENCE_ENE:
		m_data->m_energydef += effect.m_effect;
		break;

	case AMT_DEFENCE_IMP:
		m_data->m_impactdef += effect.m_effect;
		break;

	case AMT_DEFENCE_EXP:
		m_data->m_explosiondef += effect.m_effect;
		break;

	case AMT_SPECIAL:
		m_data->m_special += effect.m_effect;
		break;

	}

}

int Enemy_Away::getAttack()
{
	return m_data->m_attack;
}

int Enemy_Away::getDefence(FieldAttack FA)
{
	switch (FA)
	{
	case FA_ENERGY:
		return m_data->m_energydef;
		break; 
	case FA_IMPACT:
		return m_data->m_impactdef;
		break;
	case FA_EXPLOSION:
		return m_data->m_explosiondef;
		break;
	
	}
}

FieldAttack Enemy_Away::getAttackType()
{
	return FA_ENEMY;
}

void Enemy_Away::Heal(int hp)
{

}

bool Enemy_Away::Update(Sprite *sprite, Label *label)
{

	if (m_energy < 5)
	{
		m_energy += 1;
	}
	return Entity_Away::Update(sprite,label);
}

FieldType Enemy_Away::getType()
{
	return m_data->m_type;
}

int Enemy_Away::getSpecial()
{
	return m_data->m_special;
}

Vec2 Enemy_Away::getCenter()
{
	return Vec2(32, 32);
}

void Enemy_Away::Save(FILE *file)
{
	//save index
	fwrite((void*)&m_index, sizeof(int), 1, file);
	//save hp
	fwrite((void*)&m_health, sizeof(int), 1, file);
	//save energy
	fwrite((void*)&m_energy, sizeof(int), 1, file);

	for (int i = 0; i < 4; i++)
	{
		fwrite((void*)&m_effects[i].m_duration, sizeof(int), 1, file);
		if (m_effects[i].m_duration>0)
		{
			fwrite((void*)&m_effects[i].m_effect, sizeof(int), 1, file);
			fwrite((void*)&m_effects[i].m_stat, sizeof(int), 1, file);
		}
		fwrite((void*)&m_cooldowns[0], sizeof(int), 1, file);
	}
}

Enemy_Away::Enemy_Away(CBReader *reader,CBReader *datareader)
{
	//load index
	m_index = reader->ReadBInt();

	//re-initialize data entry
	datareader->setReadposition(0);
	//find index enemy
	while (true)
	{
		std::string str = datareader->ReadString();
		if (str.find("ENEMY") != std::string::npos)
		{
			int value = datareader->ReadInt();
			if (value == m_index)
			{
				break;
			}
		}
	}
	//load enemy data
	m_data = new Enemy_Data(datareader);
	//load hp
	m_health = reader->ReadBInt();
	//load energy
	m_energy = reader->ReadBInt();
	for (int i = 0; i < 4; i++)
	{
		m_effects[i].m_duration = reader->ReadBInt();
		if (m_effects[i].m_duration>0)
		{
			m_effects[i].m_effect = reader->ReadBInt();
			m_effects[i].m_stat = (AwayModType)reader->ReadBInt();
		}
		m_cooldowns[i] = reader->ReadBInt();
	}

}

void Enemy_Away::Flash()
{
	Vector <FiniteTimeAction *> action;
	float s = m_sprite->getScale();
	action.pushBack(ScaleTo::create(0.2F, s*1.1F));
	action.pushBack(ScaleTo::create(0.2F, s));
	Sequence *seq = Sequence::create(action);
	m_sprite->runAction(seq);
}

std::string Enemy_Away::getName()
{
	return m_data->m_name;
}

std::string Enemy_Away::getString()
{
	int r = rand() % 3;
	return m_data->m_deaths[r];
}