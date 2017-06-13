#ifndef CBREADER
#define CBREADER

#include <string>
#include <cstdlib>

class CBReader
{
	char *m_readbuffer;
	long int m_length, m_readposition;

	char *Workbuffer(int *l);

	void eatWhilespace();
	bool Safetycheck();
public:
	CBReader(unsigned char *buffer, long int length,bool strip=false);
	CBReader(char *buffer, long int length);
	~CBReader();

	int ReadInt();
	short ReadBShort();
	int ReadBInt();
	char ReadChar();
	float ReadFloat();
	float ReadBFloat();
	std::string ReadString();
	bool Readmore();
	void TutSkip();
	void setReadposition(int pos);
};

#endif