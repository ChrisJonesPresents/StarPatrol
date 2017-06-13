#include "Ability_Away.h"

AA_Effect::AA_Effect(CBReader*cbr)
{
	m_targetrule = TargetfromString(cbr->ReadString());
	m_targettype = StrtoFieldType(cbr->ReadString());
	m_graphic = Vec2(cbr->ReadInt(), cbr->ReadInt());
}

AA_Attack::AA_Attack(CBReader *cbr) : AA_Effect(cbr)
{
	m_type = AAT_ATTACK;
	m_special = cbr->ReadInt();
	m_multiplier = cbr->ReadFloat();
	m_attacktype = StrotoFieldattack(cbr->ReadString());

}

AA_Buff::AA_Buff(CBReader *cbr) : AA_Effect(cbr)
{
	m_type = AAT_BUFF;
	m_affects = ModTypefromString(cbr->ReadString());
	m_strength = cbr->ReadInt();
	m_duration = cbr->ReadInt();
	m_string = LoadLongString(cbr);
}

AA_Debuff::AA_Debuff(CBReader *cbr) : AA_Buff(cbr)
{
	m_type = AAT_DEBUFF;
	m_chance = cbr->ReadInt();

}

AA_Immunize::AA_Immunize(CBReader *cbr) : AA_Effect(cbr)
{
	m_type = AAT_IMMUNIZE;
}

AA_Recover::AA_Recover(CBReader *cbr) : AA_Effect(cbr)
{
	m_type = AAT_RECOVER;
	m_affects = ModTypefromString(cbr->ReadString());
	m_strength = cbr->ReadInt();
}