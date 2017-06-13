#include "Segbar.h"
Segbar::Segbar()
{


}

void Segbar::Setup(int height, int width, int max, int current)
{
	m_max = max;
	m_current = current;
	m_height = height;
	m_width = width;
	m_sprite = cocos2d::Sprite::create("art/bars.png");
	float p = (float)m_current / (float)m_max;
	m_sprite->setTextureRect(cocos2d::Rect(0, 16 * m_height, p * 80,16));
	m_sprite->setPosition(0,0);
	m_sprite->setAnchorPoint(cocos2d::Vec2(0, 0.5F));
	this->addChild(m_sprite);
}


void Segbar::setVariable(int c)
{
	m_current = c;
	float p = (float)m_current / (float)m_max;
	m_sprite->setTextureRect(cocos2d::Rect(0, 16 * m_height, p * 80, 16));

	
}