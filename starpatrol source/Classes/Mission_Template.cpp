#include "Mission_Template.h"

Template_Stage::Template_Stage(CBReader *cbr)
{
	
	//read tag
	m_tag = cbr->ReadString();
	//read tags
	while (true)
	{
		std::string str = cbr->ReadString();
		if (str.find("#") != std::string::npos)
		{
			break;
		}
		else
		{
			m_tag.append(" ");
			m_tag.append(str);
		}

	}
	//read type
	m_type = StrtoStageType(cbr->ReadString());
	
	//read variables
	for (int i = 0; i < 7; i++)
	{
		m_variables[i] = cbr->ReadInt();
	}
}

Mission_Template::Mission_Template(CBReader *cbr)
{
	cbr->ReadString();
	//read num tags
	m_numtags = cbr->ReadInt();
	m_tags = new std::string[m_numtags];
	for (int i = 0; i < m_numtags; i++)
	{
		m_tags[i] = cbr->ReadString();
		//read tags
		while (true)
		{
			std::string str = cbr->ReadString();
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
	m_difficulty = cbr->ReadInt();
	//load stages
	m_numstages = cbr->ReadInt();
	m_numstagechoices = new int[m_numstages];
	m_stages = new Template_Stage **[m_numstages];

	for (int i = 0; i < m_numstages; i++)
	{
		m_numstagechoices[i] = cbr->ReadInt();
		m_stages[i] = new Template_Stage *[m_numstagechoices[i]];
		for (int j = 0; j < m_numstagechoices[i]; j++)
		{
			m_stages[i][j] = new Template_Stage(cbr);
		}

	}

}

Mission_Template::~Mission_Template()
{
	delete []m_tags;

	for (int i = 0; i < m_numstages; i++)
	{
		for (int j = 0; j < m_numstagechoices[i]; j++)
		{
			delete m_stages[i][j];
		}
		delete m_stages[i];
	}
	delete[] m_stages;
	delete[]m_numstagechoices;

}
