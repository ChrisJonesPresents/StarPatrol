#include "NameGenerator.h"

void NameGenerator::Expand()
{
	int n = m_number * 2;
	Name **n_names = new Name *[n];
	for (int i = 0; i < n; i++)
	{
		if (i < m_number)
		{
			n_names[i] = m_names[i];
		}
		else
		{
			n_names[i] = NULL;
		}
	}
	delete [] m_names;
	m_number = n;
	m_names = n_names;
}

NameGenerator::NameGenerator()
{
	//open name file
	ssize_t s;
	unsigned long length;
	std::string fullpath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/starnames.txt");
	unsigned char *buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "r", &s);
	length = s;
	CBReader *cbr = new CBReader(buffer, length);
	//read the hell out of it

	m_number = 32;
	m_names = new Name *[m_number];
	for (int i = 0; i < m_number; i++)
	{
		m_names[i] = NULL;
	}
	int index = 0;
	while (cbr->Readmore())
	{
		m_names[index] = new Name();
		m_names[index]->m_name = cbr->ReadString();
		m_names[index]->m_used = false;
		Name *n = m_names[index];
		index++;
		if (index == m_number)
		{
			Expand();
		}
	}
	m_count = index;
}

NameGenerator::~NameGenerator()
{
	for (int i = 0; i < m_number; i++)
	{
		if (m_names[i] != NULL)
		{
			delete m_names[i];
		}
	}
	delete[] m_names;
}

std::string NameGenerator::getName()
{
	int r = rand() % m_count;
	if (m_names[r]->m_used == true)
	{
		r++;
		if (r >= m_count)
		{
			r = 0;
		}
		while (m_names[r]->m_used == true)
		{
			r++;
			if (r >= m_count)
			{
				r = 0;
			}
		}
		m_names[r]->m_used = true;
		return m_names[r]->m_name;
	}
	m_names[r]->m_used = true;
	return m_names[r]->m_name;
}
