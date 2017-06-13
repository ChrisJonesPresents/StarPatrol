#include "ShopDef.h"

ShopDef::ShopDef()
{
	m_items[0] = 0;
	m_recruits[0] = 0;
	m_items[1] = 0;
	m_recruits[1] = 0;
	m_numitems = 0;
	m_numrecruits = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_supplies[i][j] = 0;
		}
	}
}


void ShopDef::Save(FILE *file)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			fwrite((void*)&m_supplies[i][j], sizeof(int), 1, file);
		}
	}
	fwrite((void*)&m_numitems, sizeof(int), 1, file);
	if (m_numitems>0)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < m_numitems; j++)
			{
				fwrite((void*)&m_items[i][j], sizeof(int), 1, file);
			}
		}
	}
	fwrite((void*)&m_numrecruits, sizeof(int), 1, file);
	if (m_numrecruits>0)
	{
		for (int i = 0; i <2; i++)
		{
			for (int j = 0; j < m_numrecruits; j++)
			{
				fwrite((void*)&m_recruits[i][j], sizeof(int), 1, file);
			}
		}
	}
}

ShopDef::ShopDef(CBReader *reader)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_supplies[i][j]=reader->ReadBInt();
		}
	}
	m_numitems = reader->ReadBInt();

	if (m_numitems>0)
	{
		m_items[0] = new int[m_numitems];
		m_items[1] = new int[m_numitems];
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < m_numitems; j++)
			{
				m_items[i][j] = reader->ReadBInt();
			}
		}
	}
	else
	{
		m_items[0] = 0;
		m_items[1] = 0;
	}
	m_numrecruits = reader->ReadBInt();

	if (m_numrecruits>0)
	{
		m_recruits[0] = new int[m_numrecruits];
		m_recruits[1] = new int[m_numrecruits];
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < m_numrecruits; j++)
			{
				m_recruits[i][j] = reader->ReadBInt();
			}
		}

	}
	else
	{
		m_recruits[0] = 0;
		m_recruits[1] = 0;
	}
}


ShopDef::~ShopDef()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_items[i] != 0)
		{
			delete[] m_items[i];
		}

		if (m_recruits[i] != 0)
		{
			delete[] m_recruits[i];
		}

	}
}

bool ShopDef::PreventDupe(int cat, int value)
{
	switch (cat)
	{
	case 0:
		//check recruits
		for (int i = 0; i < m_numrecruits; i++)
		{
			if (m_recruits[0][i] == 2 && m_recruits[1][i] == value)
			{
				return true;
			}
		}
		break;

	case 1:
		//check items

		break;

	case 2:
		//check modules
		for (int i = 0; i < m_numitems; i++)
		{
			if (m_items[0][i] == 2 && m_items[1][i]==value)
			{
				return true;
			}
		}
		break;

	case 3:
		//check potions
		for (int i = 0; i < m_numrecruits; i++)
		{
			if (m_recruits[0][i] == 1 && m_recruits[1][i] == value)
			{
				return true;
			}
		}


		break;
	}
	return false;
}

