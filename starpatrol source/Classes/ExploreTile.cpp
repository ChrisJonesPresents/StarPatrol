#include "ExploreTile.h"


ExploreTile::ExploreTile()
{
	for (int i = 0; i < 6; i++)
	{
		m_enemies[i] = NULL;
	}
	for (int i = 0; i < 4; i++)
	{
		m_connections[i] = false;
	}
	m_visited = false;
	m_goal = false;
	m_traptreasure = 0;

}

ExploreTile::~ExploreTile()
{
	for (int i = 0; i < 6; i++)
	{
		if (m_enemies[i] != NULL)
		{
			delete m_enemies[i];
		}
	}
}

void ExploreTile::Save(FILE *file)
{
	for (int i = 0; i < 6; i++)
	{
		if (m_enemies[i]!=NULL)
		{
			char v = 44;
			fwrite((void*)&v, sizeof(char), 1, file);
			m_enemies[i]->Save(file);
		}
			else
			{
				char v = 19;
				fwrite((void*)&v, sizeof(char), 1, file);
			}
	}
	for (int i = 0; i < 4; i++)
	{
		fwrite((void*)&m_connections[i], sizeof(bool), 1, file);
	}
	//save visited
	fwrite((void*)&m_visited, sizeof(bool), 1, file);

	//save goals
	fwrite((void*)&m_goal, sizeof(bool), 1, file);
	//save traptreasure
	fwrite((void*)&m_traptreasure, sizeof(int), 1, file);

}
ExploreTile::ExploreTile(CBReader *reader,CBReader *datareader)
{
	for (int i = 0; i < 6; i++)
	{
		char v = reader->ReadChar();
		if (v == 44)
		{
			m_enemies[i] = new Enemy_Away(reader,datareader);
		}
		else
		{
			m_enemies[i] = NULL;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		m_connections[i] = reader->ReadChar();
	}
	m_visited = reader->ReadChar();
	m_goal = reader->ReadChar();
	m_traptreasure = reader->ReadBInt();
}