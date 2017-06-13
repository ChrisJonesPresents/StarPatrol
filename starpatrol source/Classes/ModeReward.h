#ifndef MODEREWARD
#define MODEREWARD

#include "cocos2d.h"
#include "MissionNode.h"
#include "Player.h"
#include "Playership.h"
#include "Outline.h"
#include "SaveLayer.h"
class ModeReward : protected SaveLayer
{

	MissionNode *m_stage;
	Player *m_player;
	Outline m_outline;
	int m_heldsprite;

	Sprite *m_descriptionframe;
	Label *m_description;

	Sprite **m_officers;
	Sprite **m_officerslots;
	Sprite *m_rewards[3];
	Sprite *m_rewardslots[3];
	void MoveReward(Vec2 p);
public:

	static cocos2d::Scene* createScene(Player *playerfile,MissionNode *stage);

	~ModeReward();
	virtual bool init();

	void Setup(Player *playerfile, MissionNode *stage);

	CREATE_FUNC(ModeReward);

	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
	void ButtonCallback0(Ref *pSender);

	void Save(FILE *file);
	void Setup(Player *playerfile, MissionNode *stage, CBReader *reader);
	static cocos2d::Scene* createLoadScene( MissionNode *stage,CBReader *reader);
	void ReturntoLayer(){};

};



#endif