#ifndef MODECOLONY
#define MODECOLONY

#include <cocos2d.h>
#include "Colony.h"
#include "ShopDef.h"
#include "Player.h"
#include "ReturnableLayer.h"
#include <ui\UITextField.h>
#include <ui\UIButton.h>

USING_NS_CC;
class ModeColony : public ReturnableLayer
{
	Colony *m_colony;
	Player *m_player;
	ui::Button *m_nameframe;
	bool m_active;
	Label *m_labels[13];
	Sprite *m_sprites[8];
	Sprite *m_pad;
	Vec2 Offset(int i);
	int m_target,m_subtarget;

	//subframer
	Sprite *m_subframe;
	Sprite **m_subframeicons;
	Node *m_subroot;
	Label *m_description;
	Facility **m_facilities;
	int m_numsubframeicons;
	Label *m_numbers[8];
	void WriteDescription(bool upgrade=false);
	bool CanBuild();
	bool CanUpgrade();
public:
	CREATE_FUNC(ModeColony);

	static cocos2d::Scene* createScene(Player *playerfile, Colony *colony);

	~ModeColony();
	virtual bool init();

	void Setup(Player *playerfile, Colony *colony);
	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);
	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
	void ButtonCallback4(Ref *pSender);

	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
	void ReturntoLayer();
	void Refresh();
	void AmendServices(Facility *oldfac, Facility *newfac);
	void BuildInfoFrame();
	void BuildConstructFrame();

	void BuildWindowCallback0(Ref *pSender);

	void SubwindowClose(Ref *pSender);
	void InfoWindowCallback0(Ref *pSender);
	void InfoWindowCallback1(Ref *pSender);
	void InfoWindowCallback2(Ref *pSender);
	void RefreshIcons();
	void Save(FILE *file){};
};
#endif