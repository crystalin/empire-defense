#include "common.h"

float myRand(float min, float max)
{  
	return (float)(min + ((max - min) * rand()/(RAND_MAX + 1.0))); 
}

char* copyText(const char* text) {
	char* copy = NULL;
	unsigned int len = strlen(text);

	copy = new char[len + 1];
	memcpy(copy,text,len);
	copy[len] = 0;
	return copy;
}

char* readFile (const char* fileName)
{
	ifstream filestream(fileName);
	if (!filestream) {
		cout << "File \"" << fileName  << "\" not found" << endl;
		return NULL;
	}
	char *buffer;

	filestream.seekg (0, ios::end);
	int end_pos = filestream.tellg();
	filestream.seekg (0, ios::beg);
	int begin_pos = filestream.tellg();

	int length = end_pos-begin_pos;
	buffer = new char [length];

	filestream.read (buffer,length);
	buffer[length-1] = 0;
	filestream.close();

	return buffer;
}

