#include "MissionTemplate2.h"



MissionTemplate2::MissionTemplate2(CBReader *reader)
{
	std::string str=reader->ReadString();
	m_DC = reader->ReadInt();
	m_numtags = reader->ReadInt();
	m_tags = new std::string[m_numtags];
	for (int i = 0; i < m_numtags; i++)
	{
		m_tags[i] = reader->ReadString();
		//read tags
		while (true)
		{
			std::string str = reader->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_tags[i].append(" ");
				m_tags[i].append(str);
			}
		}
	}
	m_numnodes = reader->ReadInt();
	m_rootnodes = new MissionNode_Template *[m_numnodes];

	for (int i = 0; i < m_numnodes; i++)
	{
		m_rootnodes[i] = new MissionNode_Template(reader);
	}

}

MissionTemplate2::~MissionTemplate2()
{

	delete [] m_tags;
	for (int i = 0; i < m_numnodes; i++)
	{
		delete m_rootnodes[i];
	}
	delete [] m_rootnodes;
}

Mission2 *MissionTemplate2::getMission(int DC)
{

	Mission2 *mission = new Mission2(DC, NULL);
	
	//read out from that randomly so long as A the tag is the same and B the DC is lower than or equal to the DC
	AddNode(DC, mission);

	return mission;
}

void MissionTemplate2::AddNode(int DC, Mission2 *mission)
{
	//pick random tag
	int r = rand() % m_numtags;
	std::string tag = m_tags[r];
	//pick an appropriate node that has that tag and is less than the DC
	int index = 0; int count = 0;
	for (int i = 0; i < m_numnodes; i++)
	{
		if (count>0)
		{
			if (m_rootnodes[i]->m_tag.find(tag) != std::string::npos && m_rootnodes[i]->m_DC <= DC)
			{
				count++;
			}
		}
		else
		{
			if (m_rootnodes[i]->m_tag.find(tag) != std::string::npos)
			{
				count++;
				index = i;

			}
		}
	}
	if (count > 1)
	{
		r = rand() % count; r = r + index;
	}
	else
	{
		r = index;
	}
	MissionNode *node = new MissionNode(m_rootnodes[r], DC);
	mission->AttachNode(node);
}


MissionThreatTemplate::MissionThreatTemplate(CBReader *reader)
{
	std::string str = reader->ReadString();
	m_DC = reader->ReadInt();
	m_strength = reader->ReadInt();
	m_timelimit = reader->ReadInt();
	m_numtags = reader->ReadInt();
	m_tags = new std::string[m_numtags];
	for (int i = 0; i < m_numtags; i++)
	{
		m_tags[i] = reader->ReadString();
		//read tags
		while (true)
		{
			std::string str = reader->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_tags[i].append(" ");
				m_tags[i].append(str);
			}
		}
	}
	m_numnodes = reader->ReadInt();
	m_rootnodes = new MissionNode_Template *[m_numnodes];

	for (int i = 0; i < m_numnodes; i++)
	{
		m_rootnodes[i] = new MissionNode_Template(reader);
	}

	for (int i = 0; i < 3; i++)
	{
		m_prompts[i] = "";
		while (true)
		{
			std::string str = reader->ReadString();
			if (str.find("#") != std::string::npos)
			{
				break;
			}
			else
			{
				m_prompts[i].append(str);
				m_prompts[i].append(" ");
			}
		}
	}
}

MissionThreatTemplate::~MissionThreatTemplate()
{
	delete[] m_tags;
	for (int i = 0; i < m_numnodes; i++)
	{
		delete m_rootnodes[i];
	}
	delete[] m_rootnodes;
}

Mission2 *MissionThreatTemplate::getMission(int DC)
{
	Mission2 *mission = new MissionThreat(DC,NULL,m_strength,m_timelimit);

	//read out from that randomly so long as A the tag is the same and B the DC is lower than or equal to the DC
	AddNode(DC, mission);

	return mission;
}