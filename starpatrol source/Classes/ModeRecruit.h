#ifndef MODERECRUIT
#define MODECRECRUIT

#include "cocos2d.h"
#include "Player.h"
#include "ShopDef.h"
#include "ModeShop.h"
#include "CrewView.h"

class ModeRecruit : public ReturnableLayer
{
	Label *m_description;
	Label *m_count;
	Label *m_money;
	Sprite *m_imageframe;
	Sprite *m_hand;
	int m_handindex;
	int m_discard;
	bool m_active,m_Warpgate;
	CrewView m_view;
	void RefreshRoster();

	void RefreshMoney();
	void BuildInventory();
	ShopInventory *m_inventory;
	ShopDef *m_def;
public:
	CREATE_FUNC(ModeRecruit);
	~ModeRecruit();
	static cocos2d::Scene* createScene(Player *playerfile, ShopDef *def);
	static cocos2d::Scene* createSceneWarpgate(Player *playerfile);
	virtual bool init();

	void Setup(Player *playerfile, ShopDef *def, bool warpgate);
	void ReturntoLayer();

	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
	void ButtonCallback0(Ref *pSender);
};

#endif