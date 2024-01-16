#ifndef __KEY_BINDER_H__
#define __KEY_BINDER_H__

#include "InputParser.h"
#include "commands.h"
#include "ConfigReader.h"

using namespace std;

class Controller;


class KeyBinder : public InputParser
{
public:

	KeyBinder();

	void bindKey(char key, int command);
	void bindKey(int localIndex, char key, int command);

	void setLocal(int localIndex);
	
	void readConfig(ConfigReader& configReader);

	static KeyBinder* getInstance() { if (!instance) instance = new KeyBinder(); return instance;}

	void addController(Controller* controller);
	void removeController(Controller* controller);

protected:
	static KeyBinder* instance;
	list<Controller*> controllers;

	void processKeyboard(unsigned char key, int x, int y);

	vector<int> globalBinding;
	map<int,vector<int>> localBindings;
	vector<int>* localBinding;
	
	map<string,int> commandNames;
	map<string,char> keyNames;

};

#endif