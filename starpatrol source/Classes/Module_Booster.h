#ifndef MODULEBOOSTER
#define MODULEBOOSTER

#include "Module.h"
#include "CBReader.h"
#include"Common.h"

class Module_Booster : public Module
{
	Shipmod m_modifier;
	int m_value;
	Vec2 m_defencegraphic;

public:
	Module_Booster(CBReader *cbr);

	int getValue();
	Shipmod getModier();
	Vec2 getGraphic();
};

#endif