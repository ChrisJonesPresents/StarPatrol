#include "NPCship.h"

#include "ReferenceLibrary.h"

NPCship::NPCship()
{

}

NPCship::NPCship(CBReader *cbr)
{
	//load graphics
	m_graphicpos = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
	m_graphicsize = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
	//load hp
	m_hp = cbr->ReadInt(); m_hpmax = cbr->ReadInt();
	//load defencegraphics
	m_defencegraphic[0][0] = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
	m_defencegraphic[0][1] = cocos2d::Vec2(cbr->ReadInt(), cbr->ReadInt());
	//load defence values
	for (int i = 0; i < 3; i++)
	{
		m_defences[i] = cbr->ReadInt();
	}
	//load offences
	for (int i = 0; i < 3; i++)
	{
		m_offences[i]=cbr->ReadInt();
	}
	//load num abiilties
	m_numabilities = cbr->ReadInt();
	m_abilities = new Ability_Space*[m_numabilities];
	//load abilities
	for (int i = 0; i < m_numabilities; i++)
	{
		m_abilities[i] = ReferenceLibrary::getInstance()->getAbility(cbr->ReadInt());
	}
	std::string str = cbr->ReadString();
	m_explodes = true;
	if (str.find("NO_EXPLOSION") != std::string::npos)
	{
		m_explodes = false;
	}
}

void NPCship::Save(FILE *file)
{
	//save graphics
	float x = m_graphicpos.x; float y = m_graphicpos.y;
	fwrite((void*)&x, sizeof(float), 1, file);
	fwrite((void*)&y, sizeof(float), 1, file);
	x = m_graphicsize.x; y = m_graphicsize.y;
	fwrite((void*)&x, sizeof(float), 1, file);
	fwrite((void*)&y, sizeof(float), 1, file);
	//save hp
	fwrite((void*)&m_hp, sizeof(int), 1, file);
	//save defence graphics
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			x = m_defencegraphic[i][j].x;
			y = m_defencegraphic[i][j].y;
			fwrite((void*)&x, sizeof(float), 1, file);
			fwrite((void*)&y, sizeof(float), 1, file);
		}
	}

	//save defences
	for (int i = 0; i < 3; i++)
	{
		fwrite((void*)&m_defences[i], sizeof(int), 1, file);
	}
	//save offences
	for (int i = 0; i < 3; i++)
	{
		fwrite((void*)&m_offences[i], sizeof(int), 1, file);
	}
	//save num abilities
	fwrite((void*)&m_numabilities, sizeof(int), 1, file);
	//save ability references
	for (int i = 0; i < m_numabilities; i++)
	{
		int v = m_abilities[i]->getID();
		fwrite((void*)&v, sizeof(int), 1, file);
	}
}

void NPCship::Load(CBReader *reader)
{
	//load graphics
	m_graphicpos = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
	m_graphicsize = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
	//load hp
	m_hp = reader->ReadBInt();
	m_hpmax = m_hp;
	//load defence graphics
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_defencegraphic[i][j] = Vec2(reader->ReadBFloat(), reader->ReadBFloat());
		}
	}
	//load defences
	for (int i = 0; i < 3; i++)
	{
		m_defences[i] = reader->ReadBInt();
	}

	//load offences
	for (int i = 0; i < 3; i++)
	{
		m_offences[i] = reader->ReadBInt();
	}
	//load num abilities
	m_numabilities = reader->ReadBInt();
	//load ability references
	m_abilities = new Ability_Space*[m_numabilities];
	for (int i = 0; i < m_numabilities; i++)
	{
		m_abilities[i] = ReferenceLibrary::getInstance()->getAbility(reader->ReadBInt());
	}
}

NPCship::~NPCship()
{
	delete[] m_abilities;
}

Ability_Space *NPCship::getAbility(int i)
{
	return m_abilities[i];
}
int NPCship::getNumability()
{
	return m_numabilities;
}

bool NPCship::getExplode()
{
	return m_explodes;
}