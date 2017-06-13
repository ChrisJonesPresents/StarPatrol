#ifndef NAMEGENERATOR
#define NAMEGENERATOR

#include <string>
#include "Common.h"
#include "CBReader.h"
struct Name
{
	std::string m_name;
	bool m_used;
};

class NameGenerator
{
	Name **m_names;
	int m_number,m_count;

	void Expand();

public:
	NameGenerator();
	~NameGenerator();

	std::string getName();


};


#endif 