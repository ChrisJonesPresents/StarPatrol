#include "Starship.h"

ShipStatus::ShipStatus(AS_buffeffect *effect)
{
	m_type = effect->m_modifies;
	m_value = effect->m_effect;
	timeleft = effect->m_duration;
	m_duration = effect->m_duration;
	
}

ShipStatus::ShipStatus()
{

}

Starship::Starship()
{
	for (int i = 0; i < 3; i++)
	{
		m_defences[i] = 0;
		m_defencegraphic[0][i] = cocos2d::Vec2(-1, -1);
		m_defencegraphic[1][i] = cocos2d::Vec2(-1, -1);
		m_offences[i] = 100;
	}
}

cocos2d::Vec2 Starship::getGraphicpos()
{
	return m_graphicpos;
}	

cocos2d::Vec2 Starship::getGraphicsize()
{
	return m_graphicsize;
}

int Starship::getHP()
{
	return m_hp;
}

int Starship::getHPmax()
{
	return m_hpmax;
}

int Starship::getDefence(int which)
{
	return m_defences[which];
}

void Starship::Damage(int damage)
{
	m_hp = m_hp-damage;
	if (m_hp < 0)
	{
		m_hp = 0;
	}
}

cocos2d::Vec2 Starship::getDefenceGraphic(int i)
{
	if (m_defencegraphic[1][i].y>0)
	{
		return m_defencegraphic[1][i];
	}
	return m_defencegraphic[0][i];

}

void Starship::ApplyState(ShipStatus status, Vec2 v)
{
	//apply

	//change graphics under special circumstances
	switch (status.m_type)
	{
	case SM_deflect:
		m_defences[0] += status.m_value;
		m_defencegraphic[1][0] = v;
		break;
	case SM_intercept:
		m_defences[1] += status.m_value;
		m_defencegraphic[1][1] = v;
		break;

	case SM_dodge:
		m_defences[2] += status.m_value;
		break;
	case SM_BEAM:
		m_offences[0] += status.m_value;
		break;
	case SM_MISSILE:
		m_offences[1] += status.m_value;
		break;
	case SM_PROJECTILE:
		m_offences[2] += status.m_value;
	}

}
void Starship::RemoveState(ShipStatus status)
{
	switch (status.m_type)
	{
	case SM_deflect:
		m_defences[0] -= status.m_value;
		if (status.m_value > 0)
		{
			m_defencegraphic[1][0] = Vec2(-1, -1);
		}

		break;
	case SM_intercept:
		m_defences[1] -= status.m_value;
		if (status.m_value > 0)
		{
			m_defencegraphic[1][1] = Vec2(-1, -1);
		}
		break;

	case SM_dodge:
		m_defences[2] -= status.m_value;
		break;
	case SM_BEAM:
		m_offences[0] -= status.m_value;
		break;
	case SM_MISSILE:
		m_offences[1] -= status.m_value;
		break;
	case SM_PROJECTILE:
		m_offences[2] -= status.m_value;
	}
}

float Starship::getOffence(int which)
{
	float v = m_offences[which];
	return v / 100;
}

