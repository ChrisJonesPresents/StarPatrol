#include "MissionNode_Template.h"

MissionNode_Template::MissionNode_Template(CBReader *reader)
{
	std::string str=reader->ReadString();

	m_DC = reader->ReadInt();
		m_tag = reader->ReadString();
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
				m_tag.append(" ");
				m_tag.append(str);
			}
		}
	
	m_type = StrtoStageType(reader->ReadString());
	for (int i = 0; i < 7; i++)
	{
		m_variables[i] = reader->ReadInt();
	}
	if (m_type != MST_COMPLETE)
	{

		for (int i = 0; i < 2; i++)
		{
			m_numchoice[i] = reader->ReadInt();
			if (m_numchoice[i]>0)
			{
				m_choices[i] = new MissionNode_Template *[m_numchoice[i]];
				for (int j = 0; j < m_numchoice[i]; j++)
				{
					m_choices[i][j] = new MissionNode_Template(reader);
				}
			}
			else
			{
				m_choices[i] = NULL;
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			m_numchoice[i] = NULL;
			m_choices[i] = NULL;
		}
	}

}

MissionNode_Template::~MissionNode_Template()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_numchoice[i]>0)
		{
			for (int j = 0; j < m_numchoice[i]; j++)
			{
				delete m_choices[i][j];
			}
			delete[] m_choices[i];
		}
	}
}