#ifndef MODESHOP
#define MODESHOP

#include "cocos2d.h"
#include "Player.h"
#include "ShopDef.h"
#include "Outline.h"
#include <ui/UIButton.h>
#include "Segbar.h"
#include "ReturnableLayer.h"
#include "SaveLayer.h"
USING_NS_CC;

class ShopInventory : public Node
{
public:
	Sprite **m_sprites;
	float m_y;
	int m_length;
	CREATE_FUNC(ShopInventory);
	void Setup(Sprite **sprites, int numsprites);
	void Slide(Touch *touch);
	~ShopInventory();
	Sprite *m_indicator;
	int m_index;
};

class ModeShop : public SaveLayer
{
	Outline m_outline;
	Player *m_playerfile;
	Sprite *m_descriptionframe;
	Label *m_description;
	ShopDef *m_def;
	bool m_warpgate;
	Label *m_money;
	Segbar *m_bars[2];
	Label *m_labels[2];

	Sprite *m_salebox;
	Sprite *m_purchase;
	int m_saletype[2];
	ui::Button *m_buttons[4];
	void setButtonState();
	void CanbuySet();
	ShopInventory *m_inventory;
	void RecycleSaleBox();

	int m_amountpurchase;
	int m_purchasemode;
	bool m_active;

	void BuyFuel();
	void BuyAmmo();
public:

	CREATE_FUNC(ModeShop);

	static cocos2d::Scene* createScene(Player *playerfile,ShopDef *def,bool warpgate=false);
	static cocos2d::Scene* createSceneWarpgate(Player *playerfile);

	~ModeShop();
	virtual bool init();

	void Setup(Player *playerfile,ShopDef *def, bool warpgate);
	void SetupInventory();
	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);
	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
	void ButtonCallback4(Ref *pSender);
	void ReturntoLayer();

	void Save(FILE *file);
	static cocos2d::Scene *createLoadScene(CBReader *reader);
};

#endif