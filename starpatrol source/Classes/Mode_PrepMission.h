#ifndef MODEPREPMISSION
#define MODEPREPMISSION

#include "ReturnableLayer.h"
#include "cocos2d.h"
#include "CrewView.h"
#include "Player.h"
#include "MissionNode.h"
#include <ui/UIButton.h>
#include "SaveLayer.h"
#include "Prompt.h"

class Mode_PrepMission : public SaveLayer
{
	CrewView m_view;
	MissionNode *m_stage;

	Officer *m_officer[3];
	void Cleanup();
	void StartMission();
	Label *m_description;
	Sprite *m_officerslots[3];	
	Sprite *m_officersprites[3];
	Sprite *m_potionslots[3];
	Sprite *m_potionsprites[3];
	Potion *m_potions[3];


	Sprite *m_hand;
	int m_handindex;
	int m_target;
	int m_amount;

	bool m_active;
	bool PlaceOfficer(Officer *officer,int slot);

public:

	CREATE_FUNC(Mode_PrepMission);

	static cocos2d::Scene* createScene(MissionNode *stage, Player *player);

	virtual bool init();

	void Setup(MissionNode *stage, Player *player);


	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);
	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
	void ReturntoLayer();

	void Save(FILE *file);
};

#endif