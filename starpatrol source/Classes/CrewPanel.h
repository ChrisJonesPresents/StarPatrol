#ifndef CREWPANEL
#define CREWPANEL

#include "cocos2d.h"
#include "Player.h"
#include "Playership.h"
#include "CrewView.h"
#include "ReturnableLayer.h"

USING_NS_CC;

class CrewPanel : public ReturnableLayer
{
	CrewView m_view;
	Label *m_description;
	Label *m_count;
	Sprite *m_imageframe;
	Sprite *m_hand;

	int m_handindex;
	int m_discard;
	bool m_active;

	void RefreshCrewcount();
	void RefreshRoster();

public:


	static cocos2d::Scene* createScene(Player *playerfile);


	virtual bool init();

	void Setup(Player *playerfile);

	CREATE_FUNC(CrewPanel);

	void ButtonCallback0(Ref *pSender);

	void ButtonCallback1(Ref *pSender);

	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);

	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void ReturntoLayer();
};

#endif