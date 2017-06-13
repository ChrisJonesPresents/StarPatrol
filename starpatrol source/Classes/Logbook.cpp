#include "Logbook.h"
#include "Common.h"

Logbook::Logbook()
{

}

Logbook::~Logbook()
{

}

int Logbook::getLength()
{
	return m_logs.size();
}

LogEntry *Logbook::getEntry(int i)
{
	return m_logs.at(i);
}

bool Logbook::AddEntry(bool readonly, int timestamp, std::string string)
{
	LogEntry *log = new LogEntry();
	log->m_readonly = readonly;
	log->m_string = string;
	log->m_timestamp = timestamp;
	m_logs.pushBack(log);
	return true;
}

void Logbook::CleanLog(int timestamp)
{
	for (int i = m_logs.size() - 1; i >= 0; i--)
	{
		if (m_logs.at(i) != NULL)
		{
			if (m_logs.at(i)->m_timestamp < timestamp - 1800)
			{
				m_logs.erase(i);
			}
		}
	}

}

void Logbook::Save(FILE *file)
{
	int v = m_logs.size();
	fwrite((void*)&v, sizeof(int), 1, file);
	for (int i = 0; i < v; i++)
	{
		LogEntry *logentry = m_logs.at(i);
		
		SaveLongString(logentry->m_string,file);
		fwrite((void*)&logentry->m_readonly, sizeof(bool), 1, file);
		fwrite((void*)&logentry->m_timestamp, sizeof(int), 1, file);
	}
}

Logbook::Logbook(CBReader *reader)
{
	int v = reader->ReadBInt();
	for (int i = 0; i < v; i++)
	{
		std::string str = LoadLongString(reader);
		bool readonly = reader->ReadChar();
		int timestamp = reader->ReadBInt();
		LogEntry *log = new LogEntry();
		log->m_readonly = readonly;
		log->m_string = str;
		log->m_timestamp = timestamp;
		m_logs.pushBack(log);
	}
}