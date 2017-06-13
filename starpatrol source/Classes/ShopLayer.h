#ifndef SHOPLAYER
#define SHOPLAYER

#include "cocos2d.h"
#include <ui/UIButton.h>
#include "Segbar.h"

USING_NS_CC;

class ShopLayer : public Layer
{
	Layer *m_returnlayer;
	int *m_variable,m_cost,m_max;
	Label *m_counts[2];
	Segbar *m_bar;
	int m_v;
public:

	virtual bool init();
	CREATE_FUNC(ShopLayer);
	void Setup(int type, int *variable, int max, Layer *layer,int cost);

	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);
	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);

	void onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);
};


#endif