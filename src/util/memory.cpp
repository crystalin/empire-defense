#include "model_common.h"
using namespace std;

typedef struct {
	int		address;
	int		size;
	string	file;
	int		line;
} ALLOC_INFO;

typedef list<ALLOC_INFO*> AllocList;

AllocList *allocList;

ofstream out("memory.log");

void EDMODEL_API AddTrack(int addr,  int asize,  const char *fname, int lnum)
{
	static bool inside = false;
	if (inside) {
		return;
	}
	inside = true;
	ALLOC_INFO *info;

	if(!allocList) {
		allocList = new (AllocList);
	}
	info = new(ALLOC_INFO);
	info->address = addr;
	info->file = fname;
	info->line = lnum;
	info->size = asize;
	allocList->insert(allocList->begin(), info);
	out << "[Memory] new " << info->size  << "o for " << info->file << "[l." << info->line << "]" << endl;
	inside = false;
};

void EDMODEL_API RemoveTrack(int addr)
{
	AllocList::iterator i;

	if(!allocList)
		return;
	for(i = allocList->begin(); i != allocList->end(); i++)
	{
		if((*i)->address == addr)
		{
			out << "[Memory] delete " << (*i)->size  << "o for " << (*i)->file << "[l." << (*i)->line << "]" << endl;
			allocList->remove((*i));
			break;
		}
	}
};

void EDMODEL_API DumpUnfreed()
{
	AllocList::iterator i;
	int totalSize = 0;

	if(!allocList) {
		cout << "no memory leak\n";
		return;
	}

	for(i = allocList->begin(); i != allocList->end(); i++) {
		cout << (*i)->file << ":\t\tLINE "<<(*i)->line<<",\t\tADDRESS "<<(*i)->address <<"\t" << (*i)->size << " unfreed\n";
		totalSize += (*i)->size;
	}
	cout << "-----------------------------------------------------------\n";
	cout << "Total Unfreed: " << totalSize << " bytes\n";
};
