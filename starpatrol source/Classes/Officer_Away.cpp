#include "Officer_Away.h"
#include "ReferenceLibrary.h"
#include "GameManager.h"

Officer_Away::Officer_Away(Officer *officer, Sprite *sprite)
{

	m_officer = officer;
	m_sprite = sprite;
	if (m_officer == NULL)
	{
		//build a dummy officer 'redshirt'
		m_officer = new Officer(ReferenceLibrary::getInstance()->getOfficer(0));
	}
	m_energy = m_officer->getStat(4);
	m_health = m_officer->getStat(STAT_HEALTH);
	m_targetchance = 50;

	for (int i = 0; i < 4; i++)
	{
		m_statadjustments[i] = 0;
		m_cooldowns[i] = 0;
		m_effects[i].m_duration = 0;
	}

}

int Officer_Away::getMaxEP()
{
	return m_officer->getStat(4);
}

void Officer_Away::setEnergy(int value)
{
	for (int i = 0; i < 4; i++)
	{
		m_cooldowns[i] = 0;
	}
	m_energy = value;

}
void  Officer_Away::Heal(int hp)
{
	m_health += hp;
	if (m_health>getMaxHP())
	{
		m_health = getMaxHP();
	}
}

void Officer_Away::setSprite(Sprite *sprite)
{
	m_sprite = sprite;
}

Officer_Away::~Officer_Away()
{
	if (m_officer != NULL)
	{
		//only delete a redshirt officer
		if (m_officer->getDefinition()->getName().find("ensign")!=std::string::npos)
		{
			delete m_officer;
		}
	}

}

Officer *Officer_Away::getOfficer()
{
	return m_officer;
}

int Officer_Away::getChance()
{
	return m_targetchance;
}

void Officer_Away::RemoveState(Status_Effect effect)
{
	switch (effect.m_stat)
	{
	case AMT_ATTACK:
		m_statadjustments[0] -= effect.m_effect;
		break;
	case AMT_DEFENCE_ENE:
		m_statadjustments[1] -= effect.m_effect;
		break;
	case AMT_SPECIAL:
		m_statadjustments[2] += effect.m_effect;
		break;

	case AMT_HEALTH:
		m_health -= effect.m_effect;
		break;

	case AMT_ENERGY:
		m_energy -= effect.m_effect;
		break;
	case AMT_TARGETRATE:
		m_targetchance -= effect.m_effect;
		break;
	}

}

void Officer_Away::AddState(Status_Effect effect)
{
	switch (effect.m_stat)
	{
	case AMT_ATTACK:
		m_statadjustments[0] += effect.m_effect;
		break;
	case AMT_DEFENCE_ENE:
		m_statadjustments[1] += effect.m_effect;
		break;
	case AMT_SPECIAL:
		m_statadjustments[2] += effect.m_effect;
		break;

	case AMT_HEALTH:
		m_health += effect.m_effect;
		break;

	case AMT_ENERGY:
		m_energy += effect.m_effect;
		break;

	case AMT_TARGETRATE:
		m_targetchance += effect.m_effect;
		break;
	}

}

int Officer_Away::getStat(int i)
{
	return m_statadjustments[i] + m_officer->getStat(i);

}

int Officer_Away::getAttack()
{
	return getStat(0);
}

int Officer_Away::getDefence(FieldAttack FA)
{
	return getStat(1);
}

FieldAttack Officer_Away::getAttackType()
{
	return m_officer->getDefinition()->getAttackType();
}

void Officer_Away::Defend()
{
	Status_Effect effect;
	effect.m_duration = 1;
	effect.m_stat = AMT_DEFENCE_ENE;
	effect.m_effect = m_officer->getStat(1) / 2;
	AddState(effect);
}

void Officer_Away::Recover()
{

}

bool Officer_Away::Update(Sprite *sprite, Label *label)
{

	for (int i = 0; i < 4; i++)
	{
		if (m_cooldowns[i]>0)
		{
			m_cooldowns[i]--;
		}
	}
	return Entity_Away::Update(sprite,label);
}



int Officer_Away::getSpecial()
{
	return getStat(2);
}

Sprite *Officer_Away::getSprite()
{
	return m_sprite;
}

int Officer_Away::getMaxHP()
{
	return getStat(3);
}

Vec2 Officer_Away::getCenter()
{
	return Vec2(64, 64);
}

void Officer_Away::Save(FILE *file)
{
	//save reference to officer slot on the player's ship
	//or -1 if its a redshirt
	if (m_officer->getDefinition()->getName().find("ensign") != std::string::npos)
	{
		int v = 64;
		fwrite((void*)&v, sizeof(int), 1, file);
	}
	else
	{
		//find matching officer on the ship
		Player *player = GameManager::getInstance()->getPlayer();
		for (int i = 0; i < player->getNumOfficers(); i++)
		{
			if (player->getOfficer(i) == m_officer)
			{
				fwrite((void*)&i, sizeof(int), 1, file);
				break;
			}
		}
		//save officer index

	}
	//save hp
	fwrite((void*)&m_health, sizeof(int), 1, file);
	//save energy
	fwrite((void*)&m_energy, sizeof(int), 1, file);

	//save status effects
	for (int i = 0; i < 4; i++)
	{
		fwrite((void*)&m_effects[i].m_duration, sizeof(int), 1, file);
		if (m_effects[i].m_duration>0)
		{
			fwrite((void*)&m_effects[i].m_effect, sizeof(int), 1, file);
			fwrite((void*)&m_effects[i].m_stat, sizeof(int), 1, file);
		}
		fwrite((void*)&m_cooldowns[i], sizeof(int), 1, file);
		fwrite((void*)&m_statadjustments[i], sizeof(int), 1, file);
	}

	fwrite((void*)&m_targetchance, sizeof(int), 1, file);
}
Officer_Away::Officer_Away(CBReader *reader)
{
	//reconnect with officer or initialize redshirt
	int v = reader->ReadBInt();
	if (v == 64)
	{
		m_officer = new Officer(ReferenceLibrary::getInstance()->getOfficer(0));
		m_officer->setName(ReferenceLibrary::getName("ensign"));
	}
	else
	{
		m_officer = GameManager::getInstance()->getPlayer()->getOfficer(v);
	}
	//load hp
	m_health = reader->ReadBInt();
	//load energy
	m_energy = reader->ReadBInt();
	//load status effects
	for (int i = 0; i < 4; i++)
	{
		m_effects[i].m_duration = reader->ReadBInt();
		if (m_effects[i].m_duration>0)
		{
			m_effects[i].m_effect = reader->ReadBInt();
			m_effects[i].m_stat = (AwayModType)reader->ReadBInt();
		}
		m_cooldowns[i] = reader->ReadBInt();
		m_statadjustments[i] = reader->ReadBInt();
	}

	m_targetchance = reader->ReadBInt();
}

void Officer_Away::Flash()
{
	Sprite *sprite = Sprite::create("art/white.png");
	sprite->setScale(m_sprite->getContentSize().width);
	m_sprite->addChild(sprite);
	sprite->setPosition(m_sprite->getContentSize().width/ 2, m_sprite->getContentSize().height/ 2);
	Vector <FiniteTimeAction *> action;
	float s = m_sprite->getScale();
	action.pushBack(FadeIn::create(0.2F));
	action.pushBack(FadeOut::create(0.2F));
	action.pushBack(RemoveSelf::create());
	Sequence *seq = Sequence::create(action);
	sprite->runAction(seq);

}

std::string  Officer_Away::getName()
{
	return m_officer->getName();
}