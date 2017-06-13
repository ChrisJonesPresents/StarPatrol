#ifndef EXPLORETILE
#define EXPLORETILE

#include "Common.h"
#include "Enemy_Away.h"

class ExploreTile
{
public:
	bool m_goal, m_visited;
	Enemy_Away *m_enemies[6];
	int m_traptreasure;
	bool m_connections[4];

	ExploreTile();
	~ExploreTile();
	void Save(FILE *file);
	ExploreTile(CBReader *reader,CBReader *datareader);
};


#endif