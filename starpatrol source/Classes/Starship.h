#ifndef STARSHIP
#define STARSHIP

#include "cocos2d.h"
#include "Ability_Space.h"
#include "Common.h"

#define DEF_DEFLECT 0
#define DEF_INTERCEPT 1
#define DEF_DODGE 2

class ShipStatus : public cocos2d::Object
{
public:
	ShipStatus(AS_buffeffect *effect);
	ShipStatus();

	Shipmod m_type;
	int m_value;
	float timeleft;
	int m_duration;


};

class Starship
{
protected:
	int m_hp, m_hpmax;

	cocos2d::Vec2 m_graphicpos, m_graphicsize, m_defencegraphic[2][2];
	int m_defences[3];
	int m_offences[3];

public:
	Starship();

	cocos2d::Vec2 getGraphicpos();
	cocos2d::Vec2 getGraphicsize();
	virtual cocos2d::Vec2 getDefenceGraphic(int i);
	int getHP();
	int getHPmax();


	int getDefence(int which);
	float getOffence(int which);
	void Damage(int damage);
	virtual void ApplyState(ShipStatus status, Vec2 v);
	virtual void RemoveState(ShipStatus status);
	virtual bool getExplode(){ return true; };
};

#endif