#ifndef NPCSHIP
#define NPCSHIP

#include "Starship.h"
#include "Ability_Space.h"
#include "CBReader.h"

class NPCship : public Starship
{
	Ability_Space **m_abilities;
	int m_numabilities;
	bool m_explodes;
public:
	NPCship();
	NPCship(CBReader *cbr);
	~NPCship();
	Ability_Space *getAbility(int i);
	int getNumability();
	void Save(FILE *file);
	void Load(CBReader *reader);
	bool getExplode();
};



#endif