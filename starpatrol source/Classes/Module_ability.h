#ifndef MODULEABILITY
#define MODULEABILITY

#include "Module.h"
#include "Ability_Space.h"

class Module_Ability : public Module
{
	Ability_Space *m_ability;

public:
	Module_Ability();
	Module_Ability(CBReader *cbr, Ability_Space **abilities);
	Ability_Space *getAbility();

};

#endif