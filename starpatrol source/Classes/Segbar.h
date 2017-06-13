#ifndef SEGBAR
#define SEGBAR

#include "cocos2d.h"

class Segbar : public cocos2d::Node
{
	cocos2d::Sprite *m_sprite;
	int m_current, m_max, m_height,m_width;

public:
	Segbar();
	void Setup(int height, int width, int max, int current);
	CREATE_FUNC(Segbar);

	void setVariable(int c);


};

#endif
