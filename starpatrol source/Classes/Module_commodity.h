#ifndef MODULECOMMODITY
#define MODULECOMMODITY

#include "Module.h"

enum CargoType {CT_FUEL,CT_ORDINANCE};

class Module_commodity : public Module
{
	unsigned char m_stack;
	CargoType m_cargotype;

public:
	Module_commodity(CargoType type, unsigned char stack);

	unsigned char getStack();

	CargoType getCargoType();

	void setStack(unsigned char stack);

};

#endif