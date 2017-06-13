#ifndef LOGBOOKPANEL
#define LOGBOOKPANEL

#include "Logbook.h"
#include "ReturnableLayer.h"
#include "Player.h"
#include <ui/UIButton.h>

USING_NS_CC;

class LogbookPanel : public ReturnableLayer
{
	bool m_active;
	Sprite *m_slots[16];
	Sprite *m_subsprites[16];
	Label *m_labels[16];

	Label *m_description;
	int m_index;
	int m_offset;
	float m_shift;
	void Rebuild();
	void ShiftUp();
	void ShiftDown();
    public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(LogbookPanel);
	virtual bool init();

	void ButtonCallback0(Ref *pSender);

	void ButtonCallback1(Ref *pSender);

	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
	void ButtonCallback4(Ref *pSender);
	void ButtonCallback5(Ref *pSender);
	void ButtonCallback6(Ref *pSender);
	void ButtonCallback7(Ref *pSender);


	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void ReturntoLayer();
};


#endif