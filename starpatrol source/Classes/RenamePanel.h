#ifndef RENAMEPANEL
#define RENAMEPANEL

#include <cocos2d.h>
#include <ui\UITextField.h>
#include "ReturnableLayer.h"
USING_NS_CC;

class RenamePanel : public Layer
{
	ui::TextField *m_textfield;
	std::string *m_string;
	std::string m_backupstring;
	ReturnableLayer *m_main;
public:
	CREATE_FUNC(RenamePanel);

	void Setup(std::string *string,std::string prompt,ReturnableLayer *main);
	void ButtonCallback0(Ref *pSender);
	void ButtonCallback1(Ref *pSender);

};

#endif