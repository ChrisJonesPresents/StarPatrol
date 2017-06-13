#include "Ability_Away.h"

Ability_Away::Ability_Away(CBReader *cbr, int uid)
{
	m_uid = uid;
	//name
	m_name = "";
	while (true)
	{
		std::string str = cbr->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_name.append(str + " ");
		}
	}
	m_description = "";
	while (true)
	{
		std::string str = cbr->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_description.append(str + " ");
		}
	}
	m_cost = cbr->ReadInt();
	//load cooldown
	m_cooldown = cbr->ReadInt();

	for (int i = 0; i < 2; i++)
	{
		AwayAbilityType t = TypefromString(cbr->ReadString());
		switch (t)
		{
		case AAT_ATTACK:
			m_effects[i] = new AA_Attack(cbr);
			break;
		case AAT_BUFF:
			m_effects[i] = new AA_Buff(cbr);
			break;
		case AAT_DEBUFF:
			m_effects[i] = new AA_Debuff(cbr);
			break;
		case AAT_IMMUNIZE:
			m_effects[i] = new AA_Immunize(cbr);
			break;
		case AAT_RECOVER:
			m_effects[i] = new AA_Recover(cbr);
			break;
		case AAT_NONE:
			m_effects[i] = NULL;
			break;
		}
	}
}

Ability_Away::~Ability_Away()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_effects[i] != NULL)
		{
			delete m_effects[i];
		}
	}

}


int Ability_Away::getCooldown()
{
	return m_cooldown;
}

AA_Effect *Ability_Away::getEffect(int i)
{
	return m_effects[i];
}

AwayAbilityType TypefromString(std::string str)
{
	if (str.find("ATTACK") != std::string::npos)
	{
		return AAT_ATTACK;
	}
	if (str.find("DEBUFF") != std::string::npos)
	{
		return AAT_DEBUFF;
	}
	if (str.find("BUFF") != std::string::npos)
	{
		return AAT_BUFF;
	}
	if (str.find("IMMUNIZE") != std::string::npos)
	{
		return AAT_IMMUNIZE;
	}
	if (str.find("RECOVER") != std::string::npos)
	{
		return AAT_RECOVER;
	}
	return AAT_NONE;
}

AwayAbilityTargetRule TargetfromString(std::string str)
{
	if (str.find("SELF") != std::string::npos)
	{
		return AATR_SELF;
	}
	if (str.find("ALLY") != std::string::npos)
	{
		return AATR_ALLY;
	}
	if (str.find("TARGET") != std::string::npos)
	{
		return AATR_ENEMY;
	}
	if (str.find("ALLIES") != std::string::npos)
	{
		return AATR_ALLIES;
	}
	if (str.find("ENEMIES") != std::string::npos)
	{
		return AATR_ENEMIES;
	}
}

AwayModType ModTypefromString(std::string str)
{
	if (str.find("STUN") != std::string::npos)
	{
		return AMT_STUN;
	}
	if (str.find("ATTACK") != std::string::npos)
	{
		return AMT_ATTACK;
	}
	if (str.find("DEFENCE") != std::string::npos)
	{
		return AMT_DEFENCE_ENE;
	}
	if (str.find("ENERGYDEF") != std::string::npos)
	{
		return AMT_DEFENCE_ENE;
	}
	if (str.find("TARGETRATE") != std::string::npos)
	{
		return AMT_TARGETRATE;
	}
	if (str.find("HEALTH") != std::string::npos)
	{
		return AMT_EOT_HEALTH;
	}
	if (str.find("EPUP") != std::string::npos)
	{
		return AMT_ENERGY;
	}
	if (str.find("HPUP") != std::string::npos)
	{
		return AMT_HEALTH;
	}
}

SpecialEffect EffectfromString(std::string str)
{
	if (str.find("ESCAPE") != std::string::npos)
	{
		return SE_ESCAPE;
	}

}

std::string Ability_Away::getName()
{
	return m_name;
}

std::string Ability_Away::getDescription()
{
	return m_description;
}

int Ability_Away::getCost()
{
	return m_cost;
}

int Ability_Away::getUID()
{
	return m_uid;
}