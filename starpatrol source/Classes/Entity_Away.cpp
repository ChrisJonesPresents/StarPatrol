#include "Entity_Away.h"
USING_NS_CC;
#include "Mode_Awayteam.h"

int Entity_Away::getHealth()
{
	return m_health;
}

int Entity_Away::getEnergy()
{
	return m_energy;
}

bool Entity_Away::AddEffect(Status_Effect effect)
{

	for (int i = 0; i < 4; i++)
	{
		if (m_effects[i].m_duration == 0)
		{
			m_effects[i].m_duration = effect.m_duration;
			m_effects[i].m_effect = effect.m_effect;
			m_effects[i].m_stat = effect.m_stat;
			AddState(m_effects[i]);
			RefreshIcon(i);
			return true;
		}
	}
	return false;
}
Status_Effect Entity_Away::getEffect(int i)
{
	return m_effects[i];

}
bool Entity_Away::RemoveEffect(AwayModType stat)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_effects[i].m_stat == stat)
		{
			m_effects[i].m_duration = 0; 
			RemoveState(m_effects[i]);
			return true;
		}
	}
	return false;
}
bool Entity_Away::Update(Sprite *sprite, Label *label)
{
	bool b = false;
	for (int i = 0; i < 4; i++)
	{
		if (m_effects[i].m_duration>0)
		{
			if (ProcessEffect(m_effects[i]))
			{
				b = true;
			}
			m_effects[i].m_duration--;
			if (m_effects[i].m_duration == 0)
			{
				RemoveState(m_effects[i]);
				m_statusIcons[i]->setVisible(false);
				b = true;

				//build sprite 
				sprite->setVisible(true);
				label->setString(m_effects[i].m_string);
				Vector <FiniteTimeAction *> action;
				action.pushBack(DelayTime::create(2.0F));
				action.pushBack(ToggleVisibility::create());
				Sequence *seq = Sequence::create(action);
				sprite->runAction(seq);

			}
		}
	}
	if (b == true)
	{
		if (m_health < 0)
		{
			m_health = 0;
		}
		if (m_energy < 0)
		{
			m_energy = 0;
		}
		return true;
	}
	return false;
}

bool Entity_Away::CanAct()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_effects[i].m_duration>0 && m_effects[i].m_stat == AMT_STUN)
		{
			return false;
		}
	}
	return true;
}

int Entity_Away::getCooldown(int index)
{
	return m_cooldowns[index];
}

void Entity_Away::setCooldown(int index, int duration)
{
	m_cooldowns[index] = duration;
}

bool Entity_Away::ProcessEffect(Status_Effect effect)
{
	switch (effect.m_stat)
	{
	case AMT_EOT_ENERGY:
		m_energy -= effect.m_effect;
		
		break;
		return true;
	case AMT_EOT_HEALTH:
		m_health -= effect.m_effect;
		return true;
		break;


	}
	return false;
}

bool Entity_Away::Damage(int damage)
{
	m_health -= damage;
	if (m_health < 0)
	{
		m_health = 0;
	}
	if (m_health < 1)
	{
		return true;
	}
	return false;
}

void Entity_Away::ApplyAbility(Entity_Away *origin, AA_Effect *effect,Layer *layer)
{
	switch (effect->m_type)
	{
	case AAT_ATTACK:
		ApplyAttack(origin, (AA_Attack*)effect, layer);
		break;
	case AAT_BUFF:
		ApplyBuff(origin, (AA_Buff *)effect,  layer);
		break;
	case AAT_DEBUFF:
		ApplyDebuff(origin, (AA_Debuff*)effect,  layer);
		break;

	case AAT_RECOVER:
		ApplyRecover(origin, (AA_Recover*)effect,  layer);
		break;

	}
}

void Entity_Away::ApplyAttack(Entity_Away *origin, AA_Attack *effect, Layer *layer)
{
	int attack = origin->getAttack();
	if (effect->m_special == true)
	{
		attack = origin->getSpecial();
	}
	int defence = getDefence(effect->m_attacktype);
	attack = attack*effect->m_multiplier;

	int damage = attack - defence;
	if (damage < 0)
	{
		damage = 0;
	}
	char buffer[4];
	Damage(damage);
	itoa(damage, &buffer[0], 10);
	Label *number = Label::createWithBMFont("art/font.fnt", buffer);
	number->setColor(Color3B(255, 0, 0));
	number->setPosition(this->getCenter());
	this->getSprite()->addChild(number);
	Vector <FiniteTimeAction *> action;
	action.pushBack(MoveBy::create(0.8F, cocos2d::Vec2(0, 64)));
	//	action.pushBack(ani);
	//	action.pushBack(DelayTime::create(0.8F));
	action.pushBack(RemoveSelf::create());
	action.pushBack(CallFunc::create(layer, SEL_CallFunc(&Mode_Awayteam::ProgressTurn)));
	Sequence *seq = Sequence::create(action);
	number->runAction(seq);

	action.clear();
	action.pushBack(TintTo::create(0.2F, 255, 0, 0));
	action.pushBack(TintTo::create(0.2F, 255, 255, 255));
	seq = Sequence::create(action);
	this->getSprite()->runAction(seq);
	action.clear();
	origin->Flash();
	/*
	float s = origin->getSprite()->getScale();
	action.pushBack(ScaleTo::create(0.2F, s*1.1F));
	action.pushBack(ScaleTo::create(0.2F, s));
	seq = Sequence::create(action);
	origin->getSprite()->runAction(seq);
	*/
	if (this->Damage(damage))
	{
		action.clear();
		action.pushBack(FadeOut::create(0.8F));
		action.pushBack(ToggleVisibility::create());
		seq = Sequence::create(action);
		this->getSprite()->runAction(seq);
	}

	action.clear();

	int x = 32*effect->m_graphic.x;
	int y = 32 *effect->m_graphic.y;
	Sprite *spr = Sprite::create("art/awayteamfx.png", Rect(x, 0, 32, 32));
	spr->setScale(4 / getSprite()->getScale());
	getSprite()->addChild(spr);
	spr->setPosition(getCenter());
	//	spr->setAnchorPoint(Vec2(0.5F, 0.5F));
	//	spr->setPosition(Vec2(defendspr->getTexture()->getPixelsWide() / 2, defendspr->getTexture()->getPixelsHigh() / 2));
	Animation *animation = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		animation->addSpriteFrame(SpriteFrame::create("art/awayteamfx.png", Rect(x + (32 * i), y, 32, 32)));
	}
	animation->setDelayPerUnit(0.1F);
	action.pushBack(Animate::create(animation));
	action.pushBack(RemoveSelf::create());
	seq = Sequence::create(action);
	spr->runAction(seq);



}

void Entity_Away::ApplyBuff(Entity_Away *origin, AA_Buff *effect, Layer *layer)
{

	Status_Effect applythis;
	applythis.m_duration = effect->m_duration;
	applythis.m_stat = effect->m_affects;
	applythis.m_effect = effect->m_strength;
	applythis.m_string = effect->m_string;
	long l = effect->m_string.find("NAME");
	applythis.m_string = applythis.m_string.replace(l, 4, getName());
	if (AddStatus(applythis))
	{
		AddState(applythis);
	}
	Vector <FiniteTimeAction *> action;
	int x = 32 * effect->m_graphic.x;
	int y = 32 * effect->m_graphic.y;
	Sprite *spr = Sprite::create("art/awayteamfx.png", Rect(x, 0, 32, 32));
	spr->setScale(4 / getSprite()->getScale());
	getSprite()->addChild(spr);
	spr->setPosition(getCenter());
	//	spr->setAnchorPoint(Vec2(0.5F, 0.5F));
	//	spr->setPosition(Vec2(defendspr->getTexture()->getPixelsWide() / 2, defendspr->getTexture()->getPixelsHigh() / 2));
	Animation *animation = Animation::create();
	for (int i = 0; i < 4; i++)
	{
		animation->addSpriteFrame(SpriteFrame::create("art/awayteamfx.png", Rect(x + (32 * i), y, 32, 32)));
	}
	animation->setDelayPerUnit(0.1F);
	action.pushBack(Animate::create(animation));
	action.pushBack(RemoveSelf::create());
	action.pushBack(CallFunc::create(layer, SEL_CallFunc(&Mode_Awayteam::ProgressTurn)));

	Sequence *seq = Sequence::create(action);
	spr->runAction(seq);

}
bool Entity_Away::AddStatus(Status_Effect effect)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_effects[i].m_duration<=0)
		{
			m_effects[i] = effect;
			return true;
		}
	}
	return false;
}

void Entity_Away::ApplyDebuff(Entity_Away *origin, AA_Debuff *effect,  Layer *layer)
{
	int r = rand() % 100;
	if (r < effect->m_chance)
	{
		Status_Effect applythis;

		applythis.m_duration = effect->m_duration;
		applythis.m_stat = effect->m_affects;
		applythis.m_effect = effect->m_strength*-1;
		if (AddStatus(applythis))
		{
			AddState(applythis);
		}
		Vector <FiniteTimeAction *> action;
		int x = 32 * effect->m_graphic.x;
		int y = 32 * effect->m_graphic.y;
		Sprite *spr = Sprite::create("art/awayteamfx.png", Rect(x, 0, 32, 32));
		spr->setScale(4 / getSprite()->getScale());
		getSprite()->addChild(spr);
		spr->setPosition(getCenter());
		//	spr->setAnchorPoint(Vec2(0.5F, 0.5F));
		//	spr->setPosition(Vec2(defendspr->getTexture()->getPixelsWide() / 2, defendspr->getTexture()->getPixelsHigh() / 2));
		Animation *animation = Animation::create();
		for (int i = 0; i < 4; i++)
		{
			animation->addSpriteFrame(SpriteFrame::create("art/awayteamfx.png", Rect(x + (32 * i), y, 32, 32)));
		}
		animation->setDelayPerUnit(0.1F);
		action.pushBack(Animate::create(animation));
		action.pushBack(RemoveSelf::create());
		Sequence *seq = Sequence::create(action);
		spr->runAction(seq);
	}
}

void Entity_Away::ApplyRecover(Entity_Away *origin, AA_Recover *effect, Layer *layer)
{
	switch (effect->m_affects)
	{
	case AMT_ENERGY:
		m_energy += effect->m_strength;
		if (m_energy > 5)
		{
			m_energy = 5;
		}
		break;
	case AMT_HEALTH:
		m_health += effect->m_strength;
		if (m_health > getMaxHP())
		{
			m_health = getMaxHP();
		}
		break;
	}
}

Sprite * Entity_Away::getStatusIcon(int i)
{
	return m_statusIcons[i];
}

void Entity_Away::SetupIcons()
{
	for (int i = 0; i < 4; i++)
	{
		m_statusIcons[i] = Sprite::create("art/awayteamstatusicons.png",Rect(0,0,32,32));
		m_statusIcons[i]->setScale(1.0F);
	}
}

void Entity_Away::RefreshIcon(int i)
{
	if (m_effects[i].m_duration>0)
	{
		m_statusIcons[i]->setVisible(true);
		ConfigSprite(i, m_effects[i].m_stat, m_effects[i].m_effect);
	}
	else
	{
		m_statusIcons[i]->setVisible(false);
	}
}

void Entity_Away::ConfigSprite(int i, AwayModType type, int value)
{
	if (value > 0)
	{
		switch (type)
		{
		case AMT_ATTACK:
			m_statusIcons[i]->setTextureRect(Rect(0, 0, 32, 32));
			break;
		case AMT_DEFENCE_ENE:
			m_statusIcons[i]->setTextureRect(Rect(0, 32, 32, 32));
			break;
		case AMT_TARGETRATE:
			m_statusIcons[i]->setTextureRect(Rect(64, 96, 32, 32));
			break;

		}
	}
	else
	{
		switch (type)
		{
		case AMT_ATTACK:
			m_statusIcons[i]->setTextureRect(Rect(32, 0, 32, 32));
			break;
		case AMT_DEFENCE_ENE:
			m_statusIcons[i]->setTextureRect(Rect(32, 32, 32, 32));
			break;
		case AMT_TARGETRATE:
			m_statusIcons[i]->setTextureRect(Rect(96, 96, 32, 32));
			break;

		}
	}

}

void Entity_Away::ModEnergy(int energy)
{
	m_energy -= energy;
}