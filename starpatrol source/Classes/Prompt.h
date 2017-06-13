#include "cocos2d.h"
#include "ReturnableLayer.h"
#ifndef PROMPT
#define PROMPT
USING_NS_CC;

class Prompt : public cocos2d::Layer
{
	ReturnableLayer *m_host;

public:

	CREATE_FUNC(Prompt);

	virtual bool init();

	void Setup(ReturnableLayer *host, std::string text);
	void onTouchesBegan(const std::vector< cocos2d::Touch * > &touches, cocos2d::Event *unused_event);
};


#endif