#ifndef FIELDUNIT
#define FIELDUNIT

#include "Common.h"
#include "cocos2d.h"
#include "CBReader.h"

class FU_Status
{
public:
	int duration, strength;
	FieldMod m_modifies;
	Sprite *m_sprite;
};

class FieldUnit : public Node
{
protected:
	int m_hp,m_attack,m_defence;
	bool m_individual,m_hard,m_heavy;
	Vec2 m_graphic, m_graphicsize;
	Sprite **m_sprites;
	int m_numsprites;
	FieldType m_type;
	FU_Status m_status[4];
	std::string m_name;
public:
	FieldUnit();
	void Setup(Vec2 graphic, Vec2 size, bool individual,bool hard, bool heavy, int hp, int attack, int defence, FieldType type);
	void Setup(CBReader *reader, int index,int hp);
	void Gensprites();
	~FieldUnit();
	void Add(int hp);
	FieldType getFieldType();
	Vec2 getGraphic();
	CREATE_FUNC(FieldUnit);
	void ModAttack(int attack);
	void ModDefence(int defence);
	int getAttack();
	int getDefence();
	int getHP();
	bool getIndividual();

	int Attack(int defence, int count);
	virtual bool Damage(int damage);
	virtual bool Damage(int damage,bool heavy);
	void Effect(Vec2 target, Vec2 scatter,float s=1);

	void Cooldown();
	void AddStatus(int strength, int duration, FieldMod type);
	bool getHeavy();
	virtual void Cleanup(){};
};

#endif