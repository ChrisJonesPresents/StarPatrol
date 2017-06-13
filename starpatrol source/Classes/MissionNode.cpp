#include "MissionNode.h"

MissionNode::MissionNode()
{
	for (int i = 0; i < 7; i++)
	{
		m_variables[i] = 0;
	}
	m_dc = 1;
}

MissionNode::MissionNode(MissionNode_Template *nodetemplate, int DC)
{
	for (int i = 0; i < 7; i++)
	{
		m_variables[i]=nodetemplate->m_variables[i];
	}
	m_tag = nodetemplate->m_tag;
	m_dc = nodetemplate->m_DC;
	m_type = nodetemplate->m_type;
	for (int i = 0; i < 2; i++)
	{
		if (nodetemplate->m_numchoice[i]>0)
		{
			int count = 0;
			for (int j = 0; j < nodetemplate->m_numchoice[i]; j++)
			{
				if (nodetemplate->m_choices[i][j]->m_DC <= DC)
				{
					count++;
				}
			}
			if (count>1)
			{
				int r = rand() % count;
				m_choices[i] = new MissionNode(nodetemplate->m_choices[i][r], DC);
			}
			else
			{
				m_choices[i] = new MissionNode(nodetemplate->m_choices[i][0], DC);
			}
		}
		else
		{
			m_choices[i] = NULL;
		}

	}
}

void MissionNode::Save(FILE *file)
{
	fwrite((void*)&m_uid, sizeof(int), 1, file);
	fwrite((void*)&m_dc, sizeof(int), 1, file);
	fwrite((void*)&m_type, sizeof(int), 1, file);

	SaveLongString(m_tag, file);

	for (int i = 0; i < 7; i++)
	{
		fwrite((void*)&m_variables[i], sizeof(int), 1, file);
	}

	for (int i = 0; i < 2; i++)
	{
		if (m_choices[i] != NULL)
		{
			char v = 42;
			fwrite((void*)&v, sizeof(char), 1, file);
			m_choices[i]->Save(file);
		}
		else
		{
			char v = 17;
			fwrite((void*)&v, sizeof(char), 1, file);
		}
	}

}

MissionNode::MissionNode(CBReader *reader)
{
	m_uid = reader->ReadBInt();
	m_dc = reader->ReadBInt();
	m_type = (MStageType)reader->ReadBInt();
	m_tag = LoadLongString(reader);
	for (int i = 0; i < 7; i++)
	{
		m_variables[i] = reader->ReadBInt();
	}
	for (int i = 0; i < 2; i++)
	{
		char v = reader->ReadChar();
		if (v == 42)
		{
			m_choices[i] = new MissionNode(reader);
		}
		else
		{
			m_choices[i] = NULL;
		}
	}
}

MissionNode *MissionNode::FindNode(int id)
{
	if (m_uid == id)
	{
		return this;
	}
	if (m_choices[0] != NULL)
	{
		MissionNode *node = m_choices[0]->FindNode(id);
		if (node != NULL)
		{
			return node;
		}

	}
	if (m_choices[1] != NULL)
	{
		MissionNode *node = m_choices[1]->FindNode(id);
		if (node != NULL)
		{
			return node;
		}

	}
	return NULL;
}
