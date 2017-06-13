#ifndef OFFICERAWAY
#define OFFICERAWAY

#include "Officer.h"
#include "Entity_Away.h"
#include "Common.h"

class Officer_Away : public Entity_Away
{
protected:
	int m_targetchance;
	Officer *m_officer;
	void RemoveState(Status_Effect effect);
	void AddState(Status_Effect effect);
	int m_statadjustments[5];

	Sprite *m_sprite;
public:
	Officer_Away(Officer *officer,Sprite *sprite);
	~Officer_Away();

	Officer *getOfficer();

	int getChance();
	int getStat(int i);
	int getAttack();
	int getSpecial();
	int getDefence(FieldAttack FA);
	FieldAttack getAttackType();
	void Defend();
	void Recover();
	bool Update(Sprite *sprite, Label *label);

	void setEnergy(int value);
	Sprite *getSprite();
	int getMaxHP();
	int getMaxEP();
	Vec2 getCenter();
	void setSprite(Sprite *sprite);
	void Heal(int hp);
	void Save(FILE *file);
	Officer_Away(CBReader *reader);
	void Flash();
	std::string getName();
};

#endif