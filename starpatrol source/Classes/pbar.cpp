#include "Pbar.h"



Pbar::Pbar(cocos2d::Vec2 p,cocos2d::Vec2 s, float red, float green)
{
	m_red = red;
	m_green = green;
	m_position = p;
	m_size = s;
	m_size.x = m_size.x / 6;
	//build the three sprites
	m_sprites[0] = cocos2d::Sprite::create("art/pbar.png", cocos2d::Rect(0, 0, 6, 6));
	m_sprites[0]->setPosition(p);
	m_sprites[1] = cocos2d::Sprite::create("art/pbar.png", cocos2d::Rect(6, 0, 6, 6));
	m_sprites[1]->setPosition(p);
	m_sprites[2] = cocos2d::Sprite::create("art/pbar.png", cocos2d::Rect(12, 0, 6, 6));
	m_sprites[2]->setScaleX(m_size.x);
	m_sprites[2]->setPosition(p);

	Rebuild();
}

Pbar::~Pbar()
{


}

void Pbar::Rebuild()
{
	//rebuild sections
	m_sprites[0]->setScaleX(m_size.x*(m_green-m_red));

	
	if (m_red > 0)
	{
		m_sprites[1]->setVisible(true);
		m_sprites[1]->setPositionX(m_position.x + (m_size.x*6*(m_green - m_red)));
		m_sprites[1]->setScaleX((m_size.x*m_red));
	}
	else
	{
		m_sprites[1]->setVisible(false);
	}
	
	if (m_green < 1)
	{
		m_sprites[2]->setVisible(true);
		//m_sprites[2]->setPositionX(m_position.x + (m_size.x));
		//m_sprites[2]->setScaleX(m_size.x*(-1 +m_green));
	}
	else
	{
		m_sprites[2]->setVisible(false);
	}

}

void Pbar::setGreen(float v)
{
	m_green = v;
	Rebuild();
}

void Pbar::setRed(float v)
{
	m_red = v;
	//rebuild sections
	Rebuild();
}

void Pbar::Addsprites(Layer *layer)
{
	for (int i = 2; i >= 0; i--)
	{
		m_sprites[i]->setAnchorPoint(cocos2d::Point(0, 0.5F));
		m_sprites[i]->setScaleY(4);
		layer->addChild(m_sprites[i]);
	}

}