#ifndef MODEAWAYTEAM
#define MODEAWAYTEAM

#include "cocos2d.h"
#include <ui/UIButton.h>
#include "Segbar.h"
#include "Officer.h"
#include "Officer_Away.h"
#include "Entity_Away.h"
#include "Enemy_Away.h"
#include "MissionNode.h"
#include "ReturnableLayer.h"
#include "SaveLayer.h"
#include "Potion.h"

class OfficerFrame : public cocos2d::Node
{
public:
	Sprite *m_frame;
	Label *m_label;
	Sprite *m_portrait;
	Segbar *m_bars[2];
	Officer_Away *m_officer;
	Label *m_numbers[2];
	CREATE_FUNC(OfficerFrame);
	void Setup( Officer *officer,bool battleuse=false);
	void Setup(Officer_Away *officer,bool battleuse=false);
	void RedrawBars();
};

class Mode_Awayteam : public Layer
{
	bool m_active;
	ReturnableLayer *m_host;
	Enemy_Away *m_enemies[6];
	ui::Button *m_buttons[5];
	OfficerFrame *m_frames[3];
	Label *m_prompt;
	Sprite *m_promptframe;
	int m_turnindex, m_mode;


	void ProcessAI();

	void ProcessCommand(int index);

	void RedrawBars();
	void Attack(Entity_Away *attacker, Entity_Away *defender);
	void ProcessTouch(Entity_Away *target, int index);
	void AbilityPanel();
	void MainPanel();
	void PotionPanel();
	void ProcessPotion(int index);
	void ActivatePotion(int index);
	void UsePotion(int index);
	void ProcessAbility(int index);
	bool UseAbility(Entity_Away *target, int index);
	void RunAltAbility(Entity_Away *target, int index);
	AwayAbilityTargetRule m_targetrule;
	
	bool AI_Ability(int target);

	Potion **m_potions;
public:

	~Mode_Awayteam();

	static cocos2d::Scene *DebugcreateScene();
	
	static cocos2d::Scene* createScene(int background, Officer* officers[3], Enemy_Away *enemies[6]);

	static cocos2d::Scene* createScene(Officer *officers[3], MissionNode *stage , Potion **potions);

	virtual bool init();

	CREATE_FUNC(Mode_Awayteam);

	void Setup(int background, Officer* officers[3], Enemy_Away *enemies[6], Potion **potions);
	void Setup(int background, Officer_Away *officers[3], Enemy_Away *enemies[6], Potion **potions);
	void ButtonCallback(Ref *pSender);
	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void ProgressTurn();

	void VictoryCheck();
	void CleanUp();
	void SetHost(ReturnableLayer *host);
	void Victory();
	void Defeat();
	void Save(FILE *file);

};

#endif