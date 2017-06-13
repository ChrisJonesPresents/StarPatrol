#ifndef MODESPACEBATTLE
#define MODESPACEBATTLE

#include "cocos2d.h"
#include <ui/UIButton.h>
#include "NPCship.h"
#include "Playership.h"
#include "StarshipBattler.h"
#include "Pbar.h"
#include <math.h>
#include "SaveLayer.h"
#include "Segbar.h"

USING_NS_CC;

class Mode_Spacebattle : protected SaveLayer
{
protected:
	//cocos2d::CCSpriteBatchNode *m_batchnodes[2];
	//1nd is for the ships
	//2rd is for effects
	Pbar *m_bar;
	bool m_ticking;
	float m_tickover;
	//float *m_command;
	unsigned char m_target;
	cocos2d::Sprite *m_tsprite;
	cocos2d::ui::Button *m_buttons[10];//8 action buttons, 2 manouver buttons
	int m_buttontranslation[10];
	int m_buttoncount;
	int m_buttontarget;
	cocos2d::Label *m_labels[4];
	Segbar*m_segbars[2];

	//0 is hull, 1 is fuel, 2 is ordinance, 3 is hull


	StarshipBattler *m_player;
	StarshipBattler **m_enemies;
	StarshipBattler **m_allies;
	//Vector<ShipStatus*> m_playerstates;
	Vector<Sprite*>m_playerstateicons;
	//Vector<ShipStatus*> m_allystates[2];
	//Vector<ShipStatus*> m_enemystates[3];

	int m_numenemies, m_numallies;
	int m_ordinance, m_ordinanceused;

	cocos2d::Sprite **m_bsprites;

	cocos2d::Vec2 getPos(int i, bool side);
	void SetupButtons();

	void Abilitychecks();

	bool Abilitycheck(Ability_Space *ab);

	std::string getWeaponstring(Ability_Space *ab);

	void LaserAttack(int target, int origin, Ability_Space *as, float mod);
	void MissileAttack(int target, int origin, Ability_Space *as, float mod);
	void ProjectileAttack(int target, int origin, Ability_Space *as, float mod);
	bool ProjectileHitChance(int target, Ability_Space *as, float mod);
	int LaserDamageCalc(int target, Ability_Space *as, float mod);
	bool MissileIntercept(int target, Ability_Space *as, float mod);
	void generateBeam(AS_effect *effect, int target, int origin, float mod);
	void generateShield(Starship *ship,Sprite *p);
	void generateImpact(AS_effect *effect, int target);
	void GenerateDamageNumber(int target, float delay, int damage);
	void GenerateExplosion(int target,float t=0.8F);
	StarshipBattler *getShip(int ship);
	void Buff(int target, Ability_Space *as);
	void Debuff(int origin,int target, Ability_Space *as);
	void Statehandler(float dt);
	void ReshuffleIcons();
	void AddIcon(Shipmod mod, int value);
	void AI();
	void NPCaction(int target, int origin, Ability_Space *as,Starship *ship);


	void Sparks(int target, int count);
	void Debris(int target);
	void CommonSetup();
public:

	static cocos2d::Scene *DebugcreateScene();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(Playership *playership, NPCship **enemyship, int numenemies, NPCship **allyship, int numallies);

	~Mode_Spacebattle();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	//void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Mode_Spacebattle);

	void Setup(Playership *ship, NPCship **enemyships, int numenemies, NPCship **allyships, int numallies);

	void ButtonCallback(Ref *pSender);

	void update(float dt);

	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void AttackConcluded();

	void ProgressCheck();

	void Save(FILE *file);

	static cocos2d::Scene  *createLoadScene(CBReader *reader);
	void Setup(CBReader *reader);
};

#endif