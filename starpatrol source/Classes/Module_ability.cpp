#include "Module_ability.h"


Module_Ability::Module_Ability()
{
	m_type = MT_ABILITY;
	m_ability = new Ability_Space();

}

Module_Ability::Module_Ability(CBReader *cbr, Ability_Space **abilities)
{
	CommonLoad(cbr);
	m_ability = abilities[cbr->ReadInt()];
	m_type = MT_ABILITY;
}

Ability_Space *Module_Ability::getAbility()
{
	return m_ability;
}