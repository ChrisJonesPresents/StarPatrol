#ifndef STARSHIPBATTLER
#define STARSHIPBATTLER

#include "Starship.h"


class StarshipBattler
{
	Starship *m_ship;
	Vector <ShipStatus*> m_status;
	float m_command;
public:


	StarshipBattler(Starship *ship);

	Starship *getShip();

	float getCommand();

	void subtractCommand(int c);
	
	int Update(float dt);
	
	int getNumStatus();
	ShipStatus *getStatus(int i);
	void Addstatus(ShipStatus *s, Vec2 p);

	Ability_Space *getAI();

	void RandomizeCommand();

	void ConsumeOrdinance(int ordinance);

	void SaveState(FILE *file);
	void LoadState(CBReader *reader);

};


#endif