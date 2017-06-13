#ifndef SHIPPANEL
#define SHIPPANEL
#include "cocos2d.h"
#include "Player.h"
#include "Segbar.h"
#include <ui/UIButton.h>
#include "Outline.h"
USING_NS_CC;
class ShipPanel : public Layer
{
	Player *m_player;
	Outline m_outline;

	
	Segbar *m_bars[2];
	
	Label *m_description;


public:
	~ShipPanel();
	static cocos2d::Scene* createScene(Player *playerfile);

	
	virtual bool init();

	void Setup(Player *playerfile);

	CREATE_FUNC(ShipPanel);
	void ButtonCallback0(Ref *pSender);

	void ButtonCallback1(Ref *pSender);

	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
	void ButtonCallback4(Ref *pSender);

	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
	void ReturntoLayer();
};

#endif