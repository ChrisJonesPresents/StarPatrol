#include "CBReader.h"
#include <stdio.h>
#include <stdlib.h>

CBReader::CBReader(char *buffer, long int length)
{
	m_readposition = 0;
	m_readbuffer = buffer;
	m_length = length;
}

CBReader::CBReader(unsigned char *buffer, long int length, bool strip)
{

	m_readbuffer = new char[length];
	m_length = 0;
	for (int i = 0; i < length; i++)
	{
		if (strip == true)
		{
			if (buffer[i] != '\n')
			{
				m_readbuffer[m_length] = buffer[i];
				m_length++;
			}
		}
		else
		{
			if (buffer[i] != '\n' && buffer[i] != '\r')
			{
				m_readbuffer[m_length] = buffer[i];
				m_length++;
			}
			else
			{
				m_readbuffer[m_length] = ' ';
				m_length++;
			}
		}
	}
	delete []buffer;
	m_readposition = 0;

}
CBReader::~CBReader()
{
	delete [] m_readbuffer;

}

char *CBReader::Workbuffer(int *l)
{
	char *workbuffer = &m_readbuffer[m_readposition];
	int offset = 1;
	while (true)
	{

		if (workbuffer[offset] == ' ' || m_readposition+offset>=m_length)
		{
			break;
		}
		offset++;
	}
	m_readposition += offset;
	/*
	while (true)
	{
		unsigned char c = workbuffer[m_readposition];
		if (workbuffer[m_readposition] != ' ')
		{
			break;
		}
		m_readposition++;
	}
	*/
	*l = offset;
	return reinterpret_cast<char *>(workbuffer);


}

int CBReader::ReadInt()
{
	int l = 0;
	char *buffer = Workbuffer(&l);
	int value=std::atoi(buffer);
	eatWhilespace();
	return value;
}

float CBReader::ReadFloat()
{
	int l = 0;
	char *buffer = Workbuffer(&l);
	float value = std::atof(buffer);
	eatWhilespace();
	return value;
}

short CBReader::ReadBShort()
{
	char *buffer = &m_readbuffer[m_readposition];
	short v = 0;
	memcpy(&v, buffer, sizeof(short));
	m_readposition += sizeof(short);
	return v;



}

int CBReader::ReadBInt()
{
	char *buffer = &m_readbuffer[m_readposition];
	int v = 0;
	memcpy(&v, buffer, sizeof(int));
	m_readposition += sizeof(int);
	return v;
}

float CBReader::ReadBFloat()
{
	char *buffer = &m_readbuffer[m_readposition];
	float v = 0;
	memcpy(&v, buffer, sizeof(float));
	m_readposition += sizeof(float);
	return v;

}

char CBReader::ReadChar()
{
	char buffer[16];
	for (int i = m_readposition; i < m_readposition + 16; i++)
	{
		buffer[i-m_readposition] = m_readbuffer[i];
	}
	char b = m_readbuffer[m_readposition];
	m_readposition++;
	return b;
}

std::string CBReader::ReadString()
{
	int l = 0;
	char *buffer = Workbuffer(&l);
	std::string str = buffer;
	str = str.substr(0, l);
	eatWhilespace();
	return str;
}

bool CBReader::Readmore()
{
	if (m_length <= m_readposition)
	{
		return false;
	}
	return true;
}

void CBReader::TutSkip()
{
	while (m_readbuffer[m_readposition] != '#')
	{
		m_readposition++;
	}
}

void CBReader::eatWhilespace()
{
	while (m_readbuffer[m_readposition] == ' ')
	{
		m_readposition++;
	}
}

void CBReader::setReadposition(int pos)
{
	m_readposition = pos;
}
bool CBReader::Safetycheck()
{
	if (m_readposition >= m_length)
	{
		return true;
	}
	return false;
}
