#ifndef ENTITYAWAY
#define ENTITYAWAY

#include "Common.h"
#include "Ability_Away.h"
class Status_Effect
{
public:
	int m_duration;
	int m_effect;
	AwayModType m_stat;
	std::string m_string;
};

class Entity_Away
{
protected:
	int m_health, m_energy;
	int m_cooldowns[4];
	Status_Effect m_effects[4];
	Sprite *m_statusIcons[4];
	bool ProcessEffect(Status_Effect effect);
	virtual void RemoveState(Status_Effect effect){};
	virtual void AddState(Status_Effect effect){};
	void ApplyAttack(Entity_Away *origin, AA_Attack *effect, Layer *layer);
	void ApplyBuff(Entity_Away *origin, AA_Buff *effect, Layer *layer);
	void ApplyDebuff(Entity_Away *origin, AA_Debuff *effect, Layer *layer);
	void ApplyRecover(Entity_Away *origin, AA_Recover *effect, Layer *layer);
	bool AddStatus(Status_Effect effect);
public:
	void SetupIcons();
	int getHealth();
	int getEnergy();
	bool AddEffect(Status_Effect effect);
	Status_Effect getEffect(int i);
	bool RemoveEffect(AwayModType stat);
	virtual bool Update(Sprite *sprite, Label *label);
	bool CanAct();
	virtual int getAttack(){ return 0; };
	virtual int getSpecial(){ return 0; };
	virtual int getDefence(FieldAttack FA){ return 0; };
	virtual void Heal(int hp){};
	virtual FieldAttack getAttackType(){ return FA_ENERGY; };
	bool Damage(int damage);
	virtual FieldType getType(){ return FT_REDSHIRT; };
	void ApplyAbility(Entity_Away *origin, AA_Effect *effect, Layer *layer);
	virtual Sprite *getSprite(){ return NULL; };
	virtual int getMaxHP(){ return 0; };
	Sprite * getStatusIcon(int i);
	void RefreshIcon(int i);
	void ConfigSprite(int i, AwayModType type, int value);
	void ModEnergy(int energy);

	int getCooldown(int index);
	void setCooldown(int index, int duration);
	
	virtual void Flash(){};
	virtual Vec2 getCenter(){
		return Vec2(0, 0);
	};
	virtual std::string getName(){ return ""; };
	virtual std::string getString(){ return ""; };
};

#endif