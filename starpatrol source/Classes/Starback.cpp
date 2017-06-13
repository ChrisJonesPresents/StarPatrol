#include "Starback.h"

USING_NS_CC;

Starback::Starback()
{
	//generate initial stars
	for (int i = 0; i<64; i++)
	{
		int r = rand() % 4;
		Sprite *spr = Sprite::create("art/galaxysprites.png",Rect(16*r,4,16,16));
		spr->setPosition(PlaceStar(Rect(0, 0, 1600, 1280)));
		this->addChild(spr);
	}
}

void Starback::Regen(Vec2 p)
{
	if (m_p.distance(p) > 128)
	{
		
	}
}

Starback::~Starback()
{

}

Vec2 Starback::PlaceStar(cocos2d::Rect r)
{

	return Vec2(r.origin.x+rand()%(int)r.size.width, r.origin.y+rand()%(int)r.size.height);
}
