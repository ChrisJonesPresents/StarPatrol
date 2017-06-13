#ifndef STARBACK
#define STARBACK

#include "cocos2d.h"

class Starback : public cocos2d::Node
{
	cocos2d::Vec2 m_p;
	cocos2d::Vector <cocos2d::Sprite*> m_stars;

public:
	Starback();
	void Regen(cocos2d::Vec2 p);
	~Starback();
	cocos2d::Vec2 PlaceStar(cocos2d::Rect r);

	CREATE_FUNC(Starback);
};

#endif