#ifndef OFFICERVIEW
#define OFFICERVIEW

#include "cocos2d.h"
#include <ui\UIButton.h>
#include "Officer.h"
#include "OfficerDef.h"
#include "ReturnableLayer.h"

class OfficerView : public ReturnableLayer
{
	Layer *m_layer;
	Officer *m_officer;
	Sprite *m_labelframes[7];
	Label *m_labels[7];
	Label *m_description;
	Sprite *m_subpanel;
	Sprite *m_panel;
	Sprite *m_frame;
	bool m_dismiss;

	void BuildSubPanel();

	Sprite *m_choiceframes[2];
	unsigned short m_selection;
	ui::Button *m_button;
public:

	virtual bool init();

	void Setup(Officer *officer, Layer *hostlayer,Label *description);

	CREATE_FUNC(OfficerView);

	void 	onTouchesBegan(const std::vector< Touch * > &touches, Event *unused_event);
	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);
	void ButtonCallback2(Ref *pSender);
	void ButtonCallback3(Ref *pSender);
	void ButtonCallback4(Ref *pSender);
	void ReturntoLayer();
	void Save(FILE *file){};
};

#endif