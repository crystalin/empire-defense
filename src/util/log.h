#ifndef __LOG_H__
#define __LOG_H__

#ifdef _DEBUG
#define LOG_FILE(text) { Log::getStream() << text; }
#else
#define LOG_FILE(text) {}
#endif

using namespace std;

typedef enum
{
    LOG_BASIC       = 0x0001,
    LOG_EXTENDED    = 0x0002,
    LOG_DEBUG       = 0x0004,
    ADD_CONSOLE     = 0x0100,
};

class EDMODEL_API Log
{
public :
	static ofstream& getStream() { return out; }

protected :
	static ofstream out;
};

#endif