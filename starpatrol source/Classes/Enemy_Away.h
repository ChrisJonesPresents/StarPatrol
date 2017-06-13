#ifndef ENEMYAWAY
#define ENEMYAWAY

#include "Entity_Away.h"
#include "CBReader.h"
#include "Ability_Away.h"

class Enemy_Data
{
public:
	Enemy_Data(CBReader * reader);
	int m_attack, m_impactdef, m_energydef,m_explosiondef, m_health, m_special;
	std::string m_name;
	Ability_Away *m_abilities[3];
	Vec2 m_graphics[2];
	FieldType m_type;
	std::string m_deaths[3];
};

class Enemy_Away : public Entity_Away
{
protected:
	Enemy_Data *m_data;
	Sprite *m_sprite;
	void RemoveState(Status_Effect effect);
	void AddState(Status_Effect effect);
	int m_index;
public:
	Enemy_Away(int index, CBReader *reader);
	~Enemy_Away();
	void BuildSprite();
	Sprite *getSprite();
	Enemy_Data *getData();
	int getAttack();
	int getDefence(FieldAttack FA);
	FieldAttack getAttackType();
	bool Update(Sprite *sprite, Label *label);
	FieldType getType();
	int getSpecial();
	Vec2 getCenter();
	 void Heal(int hp);
	 void Save(FILE *file);
	 void Flash();
	 Enemy_Away(CBReader *reader,CBReader * datareader);
	 std::string getName();
	 std::string getString();
};

#endif