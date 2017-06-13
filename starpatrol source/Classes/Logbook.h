#ifndef LOGBOOK
#define LOGBOOK

#include <string>
#include <cocos2d.h>
#include "CBReader.h"

USING_NS_CC;

class LogEntry : public cocos2d::Object
{
public:
	bool m_readonly;
	std::string m_string;
	int m_timestamp;

};

class Logbook
{
	Vector <LogEntry*> m_logs;

public:
	Logbook();
	~Logbook();

	int getLength();
	LogEntry *getEntry(int i);
	bool AddEntry(bool readonly, int timestamp, std::string string);
	void CleanLog(int timestamp);

	void Save(FILE *file);
	Logbook(CBReader *reader);
};

#endif