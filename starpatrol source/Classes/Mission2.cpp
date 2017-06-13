#include "Mission2.h"

Mission2::Mission2(int dc, MissionNode *root)
{
	m_dc = dc;
	m_root = root;
	m_current = root;
}
Mission2::~Mission2()
{
	if (m_root != NULL)
	{
		delete m_root;
	}
}

void Mission2::Transition(int path)
{
	switch (path)
	{
	case 0:
		
		m_current = m_current->m_choices[0];
		break;

	case 1:
		m_current = m_current->m_choices[1];
		break;

	case -1:
		m_current = m_root;
		break;
	}
}

void Mission2::Reset()
{
	m_current = m_root;
}

MissionNode *Mission2::getCurrentNode()
{
	return m_current;
}

int Mission2::getDC()
{
	return m_dc;
}

void Mission2::AttachNode(MissionNode *node)
{
	m_root = node;
	m_current = node;
	GenNodeUIDS();
}

void RecursiveUID(int *value, MissionNode *node)
{
	node->m_uid = *value;
	*value = *value + 1;
	if (node->m_choices[0] != NULL)
	{
		RecursiveUID(value, node->m_choices[0]);
	}
	if (node->m_choices[1] != NULL)
	{
		RecursiveUID(value, node->m_choices[1]);
	}
}

void Mission2::GenNodeUIDS()
{
	int v = 0;
	RecursiveUID(&v, m_root);
}

MissionThreat::MissionThreat(int dc, MissionNode *root, int strength, int timelimit)
{

	m_dc = dc;
	m_root = root;
	m_current = root;
	m_strength = strength;
	m_timelimit = timelimit;
	GenNodeUIDS();
}

MissionThreat::~MissionThreat()
{
	if (m_root != NULL)
	{
		delete m_root;
	}

}

bool MissionThreat::TimeReduce(int value)
{
	m_timelimit -= value;
	if (m_timelimit <= 0)
	{
		return true;
	}
	return false;
}

int MissionThreat::getTime()
{
	return m_timelimit;
}

int MissionThreat::getStrength()
{
	return m_strength;
}

void Mission2::Save(FILE *file)
{
	char v = 12;
	fwrite((void*)&v, sizeof(char), 1, file);
	fwrite((void*)&m_dc, sizeof(int), 1, file);
	fwrite((void*)&m_uid, sizeof(int), 1, file);
	
	if (m_root != NULL)
	{
		char v = 42;
		fwrite((void*)&v, sizeof(char), 1, file);
		int id = m_current->m_uid;
		fwrite((void*)&id, sizeof(int), 1, file);
		m_root->Save(file);
	}
	else
	{
		char v = 17;
		fwrite((void*)&v, sizeof(char), 1, file);
	}
	v = 64;
	fwrite((void*)&v, sizeof(char), 1, file);
}

Mission2::Mission2(CBReader *reader)
{
	m_dc = reader->ReadBInt();
	m_uid = reader->ReadBInt();
	char v = reader->ReadChar();
	if (v == 42)
	{
		int id = reader->ReadBInt();
		m_root = new MissionNode(reader);
		m_current = m_root->FindNode(id);
	}
	else
	{
		m_root = NULL;
		m_current = NULL;
	}
	v = reader->ReadChar();
	if (v != 64)
	{
		char fu = 111;

	}
}

Mission2 *Mission2::LoadMission(CBReader *reader)
{
	char v = reader->ReadChar();
	if (v == 32)
	{
		return new MissionThreat(reader);
	}
	if (v == 12)
	{
		return new Mission2(reader);
	}
	return NULL;
}

void MissionThreat::Save(FILE *file)
{
	char v = 32;
	fwrite((void*)&v, sizeof(char), 1, file);
	fwrite((void*)&m_dc, sizeof(int), 1, file);
	fwrite((void*)&m_uid, sizeof(int), 1, file);
	fwrite((void*)&m_timelimit, sizeof(int), 1, file);
	fwrite((void*)&m_strength, sizeof(int), 1, file);
	if (m_root != NULL)
	{
		char v = 42;
		fwrite((void*)&v, sizeof(char), 1, file);
		int id = m_current->m_uid;
		fwrite((void*)&id, sizeof(int), 1, file);
		m_root->Save(file);
	}
	else
	{
		char v = 17;
		fwrite((void*)&v, sizeof(char), 1, file);
	}
}

MissionThreat::MissionThreat(CBReader *reader)
{
	m_dc = reader->ReadBInt();
	m_uid = reader->ReadBInt();
	m_timelimit = reader->ReadBInt();
	m_strength = reader->ReadBInt();
	char v = reader->ReadChar();
	if (v == 42)
	{
		int id = reader->ReadBInt();
		m_root = new MissionNode(reader);
		m_current = m_root->FindNode(id);
	}
	else
	{
		m_root = NULL;
		m_current = NULL;
	}
}