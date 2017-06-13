#ifndef PBAR
#define PBAR

#include "cocos2d.h"
using namespace NS_CC;

class Pbar
{
	cocos2d::Sprite *m_sprites[3];
	
	cocos2d::Vec2 m_position, m_size;

	float m_red, m_green;

public:

	Pbar(Vec2 p, Vec2 s, float red, float green);

	~Pbar();

	void Rebuild();

	void setGreen(float v);

	void setRed(float v);

	void Addsprites(Layer *layer);
};

#endif