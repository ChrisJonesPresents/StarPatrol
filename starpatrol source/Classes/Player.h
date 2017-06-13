#ifndef PLAYER
#define PLAYER

#include "cocos2d.h"
#include "Playership.h"
#include "Star.h"
#include "Item.h"
#include "Officer.h"
#include "OfficerDef.h"
#include "Logbook.h"
#include "Potion.h"
class PotionStack
{
public:
	PotionStack(char stack, Potion *potion);
	Potion *m_potion;
	char m_stack;
};

class Player
{
	Playership *m_ship;
	short m_x0, m_y0, m_x1, m_y1;
	float m_transit, m_distance;
	Item *m_items[3];
	unsigned short m_money;
	Officer **m_officers;
	unsigned short m_numofficers;
	unsigned short m_level;
	unsigned short m_exp;
	Logbook m_logbook;

	unsigned short m_numpotions;
	PotionStack **m_potions;

public:
	Player();
	~Player();

	Vec2 getPos0();
	Vec2 getPos1();
	float getTransit();
	Playership *getShip();
	void setPos(Vec2 p);

	Item *getItem(int i);
	void setItem(int i, Item *item);
	int getMoney();

	Officer *getOfficer(int i);
	void setOfficer(Officer *officer, int slot);
	int getNumOfficers();
	void setMoney(int money);
	int getXP();
	void addXP(int v);
	int getLevel();
	void addLevel(int v);
	Logbook *getLogbook();
	void Save(FILE *file);
	Player(CBReader*reader);

	void NewCaptain(int x, int y, std::string capname, std::string shipname, bool early, int timestamp);
	PotionStack *getPotion(int i);
	int getNumPotion();
	void setPotion(PotionStack *stack, int slot);
	void CompactPotions();
	void ResetPotions(Potion *potions[3]);
	void PotionCheck();
};

#endif