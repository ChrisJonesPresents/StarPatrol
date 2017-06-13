#include "Mode_Starscape.h"

Path::Path()
{
	for (int i = 0; i < 8; i++)
	{
		m_sprites[i] = NULL; 
	}
	m_index = 0;
}

Path::~Path()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_sprites[i] != NULL)
		{
			m_sprites[i]->getParent()->removeChild(m_sprites[i]);
			m_sprites[i] = NULL;
		}
	}
}


void Path::Clear()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_sprites[i] != NULL)
		{
			m_sprites[i]->getParent()->removeChild(m_sprites[i]);
			m_sprites[i] = NULL;
		}
	}
}
void Path::Increment()
{
	
	m_sprites[m_index]->getParent()->removeChild(m_sprites[m_index]);

	m_index++;
}

class Pathfind_node : public Object
{
public:
	int h;
	int c;
	Pathfind_node *m_origin;
	Vec2 pos;
	int steps;
};

boolean Notthere(Vector<Pathfind_node*> *openlist,Vector <Pathfind_node*> *closedlist, int x, int y)
{
	for (int i = 0; i < openlist->size(); i++)
	{
		int xe = openlist->at(i)->pos.x;
		int ye = openlist->at(i)->pos.y;
		if (x == xe && ye == y)
		{
			return false;
		}
	}
	for (int i = 0; i < closedlist->size(); i++)
	{
		int xe = closedlist->at(i)->pos.x;
		int ye = closedlist->at(i)->pos.y;
		if (y == ye && x == ye)
		{
			return false;
		}
	}
	return true;
}

Pathfind_node* GenPathnode(Vec2 destination, Star *o, Star *p, Vec2 pos)
{
	float c = o->getPosition().distance(p->getPosition());
	float h = p->getPosition().distance(destination);
	Pathfind_node *node = new Pathfind_node();
	node->c = c;
	node->h = h;
	node->pos = pos;
	return node;
}

void UpdateOpenlist(Vector<Pathfind_node*> *openlist, Vector<Pathfind_node*> *closedlist, Vec2 destination, Pathfind_node *closed, Star ***stars, int width, int height)
{
	if (closed->pos.x > 0)
	{
		if (stars[(int)closed->pos.x - 1][(int)closed->pos.y] != NULL)
		{
			if (Notthere(openlist, closedlist, closed->pos.x - 1, closed->pos.y))
			{
				Pathfind_node* pp = GenPathnode(destination, stars[(int)closed->pos.x][(int)closed->pos.y], stars[(int)closed->pos.x - 1][(int)closed->pos.y], Vec2(closed->pos.x - 1, closed->pos.y));
				pp->c = pp->c + closed->c;
				pp->steps = closed->steps + 1;
				pp->m_origin = closed;
				openlist->pushBack(pp);
			}
		}
	}
	if (closed->pos.x < width - 1)
	{
		if (stars[(int)closed->pos.x + 1][(int)closed->pos.y] != NULL)
		{
			if (Notthere(openlist, closedlist, closed->pos.x + 1, closed->pos.y))
			{
				Pathfind_node * pp = GenPathnode(destination, stars[(int)closed->pos.x][(int)closed->pos.y], stars[(int)closed->pos.x + 1][(int)closed->pos.y], Vec2(closed->pos.x + 1, closed->pos.y));
				pp->c = pp->c + closed->c;
				pp->steps = closed->steps + 1;
				pp->m_origin = closed;
				openlist->pushBack(pp);
			}
		}
	}
	if (closed->pos.y>0)
	{
		if (stars[(int)closed->pos.x ][(int)closed->pos.y-1] != NULL)
		{
			if (Notthere(openlist, closedlist, closed->pos.x, closed->pos.y))
			{
				Pathfind_node *pp = GenPathnode(destination, stars[(int)closed->pos.x][(int)closed->pos.y], stars[(int)closed->pos.x][(int)closed->pos.y - 1], Vec2(closed->pos.x, closed->pos.y - 1));
				pp->c = pp->c + closed->c;
				pp->steps = closed->steps + 1;
				pp->m_origin = closed;
				openlist->pushBack(pp);
			}
		}
	}
	if (closed->pos.y < height - 1)
	{
		if (stars[(int)closed->pos.x ][(int)closed->pos.y+1] != NULL)
		{
			if (Notthere(openlist, closedlist, closed->pos.x, closed->pos.y))
			{
				Pathfind_node *pp = GenPathnode(destination, stars[(int)closed->pos.x][(int)closed->pos.y], stars[(int)closed->pos.x][(int)closed->pos.y + 1], Vec2(closed->pos.x, closed->pos.y + 1));
				pp->c = pp->c + closed->c;
				pp->steps = closed->steps + 1;
				pp->m_origin = closed;
				openlist->pushBack(pp);
			}
		}
	}
}

bool Path::Long_gen(Node *o, Star ***stars, int width, int height, Vec2 origin, Vec2 destination)
{
	m_index = 0;
	Vector <Pathfind_node*> m_closedlist;
	Vector <Pathfind_node*> m_openlist;

	//add the origin to the closed list
	Pathfind_node *p = new Pathfind_node(); p->m_origin = NULL; p->c = 0; p->h = 0; p->pos = origin; p->steps = 0;
	m_closedlist.pushBack(p);
	//add all around the origin to the open list
	UpdateOpenlist(&m_openlist, &m_closedlist, stars[(int)destination.x][(int)destination.y]->getPosition(), p, stars, width, height);

	while (true)
	{

		float comp = 999;
		int dex = 0;
		for (int i = 0; i < m_openlist.size(); i++)
		{
			if (m_openlist.at(i)->c + m_openlist.at(i)->h < comp)
			{
				dex = i;
				comp = m_openlist.at(i)->c + m_openlist.at(i)->h;
			}
		}
		Pathfind_node *P = m_openlist.at(dex);
		if (P->h>0)
		{
			m_closedlist.pushBack(P);
			m_openlist.erase(dex);
			UpdateOpenlist(&m_openlist, &m_closedlist, stars[(int)destination.x][(int)destination.y]->getPosition(), P, stars, width, height);
		}
		else
		{
			p = P;
			break;
		}

		//then interatively pick the node on the openlist with the lowest combined h + c scores and add that to the closed list, repeating procedure

		//continue until we reach the destination
	}
	//then backtrack to generate our list of positions

	return false;
}

bool Path::Short_gen(Node *o, Star ***stars, int width, int height, Vec2 origin, Vec2 destination)
{
	Vec2 po = stars[(int)origin.y][(int)origin.x]->getPosition();
	Vec2 pd = stars[(int)destination.y][(int)destination.x]->getPosition();
	//generate a single step link to the adjacent star
	m_index = 0;
	m_sprites[0] = Sprite::create("art/galaxysprites.png", Rect(64, 0, 64, 4));
	m_sprites[0]->setAnchorPoint(Vec2(0, 0.5));
	float v = po.getDistance(pd) / 64;
	m_sprites[0]->setScaleX(v);
	m_sprites[0]->setPosition(po);
	float a = std::atan2(pd.x - po.x, pd.y - po.y);
	m_sprites[0]->setRotation((a * 57)-90);
	o->addChild(m_sprites[0]);
	m_positions[0] = destination;
	m_sprites[0]->setZOrder(0);
	return true;
}
bool Path::Generate(Node *o, Star ***stars, int width, int height, Vec2 origin, Vec2 destination)
{
	Clear();
	if (origin.getDistance(destination) < 1.5)
	{
		//create direct link
		return Short_gen(o, stars, width, height, origin, destination);
	}
	else
	{
//		Long_gen(o, stars, width, height, origin, destination);
	}

}

Vec2 Path::getStep()
{
	if (m_sprites[m_index] != NULL)
	{
		return m_positions[m_index];
	}
}