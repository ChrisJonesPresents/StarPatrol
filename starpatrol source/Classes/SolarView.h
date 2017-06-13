#ifndef	SOLARVIEW
#define SOLARVIEW

#include "cocos2d.h"
#include "ui\UIButton.h"
#include "Star.h"

class SolarView : public Layer
{
	Layer *m_main; //so we can re-enable the layer later as we remove this one
	Star *m_star;
	Sprite *m_frame;
	Sprite **m_locations;
	Sprite **m_questionmarks;

	bool CanColonize();
	ui::Button *m_button;
	bool m_colonize;

	void Colonize(Location *lt);
public:

	~SolarView();

	virtual bool init();

	void Setup(Layer *main,Star *star);

	CREATE_FUNC(SolarView);

	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesMoved(const std::vector< Touch * > &touches, Event *unused_event);
	void 	onTouchesEnded(const std::vector< Touch * > &touches, Event *unused_event);

	void LocationTouched();

	void ButtonCallback0(Ref *pSender);

	void ButtonCallback1(Ref *pSender);
};

#endif