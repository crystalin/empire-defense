
#include "controller_common.h"
#include "KeyBinder.h"
#include "Controller.h"
using namespace std;

#define COMMAND_MACRO(command) { commandNames[#command] = command; }

KeyBinder* KeyBinder::instance = NULL;

KeyBinder::KeyBinder(): localBinding(NULL)
{

	globalBinding.resize(256);

// used to declare commands as names (see COMMAND_MACRO at the top)
#include "commands.h"

	keyNames["f1"] = KEY_F1;
	keyNames["f2"] = KEY_F2;
	keyNames["f3"] = KEY_F3;
	keyNames["f4"] = KEY_F4;
	keyNames["f5"] = KEY_F5;
	keyNames["f6"] = KEY_F6;
	keyNames["f7"] = KEY_F7;
	keyNames["f8"] = KEY_F8;
	keyNames["f9"] = KEY_F9;
	keyNames["f10"] = KEY_F10;
	keyNames["f11"] = KEY_F11;
	keyNames["f12"] = KEY_F12;
	
	keyNames["esc"] = 27;

	keyNames["left"] = KEY_LEFT;
	keyNames["up"] = KEY_UP;
	keyNames["right"] = KEY_RIGHT;
	keyNames["down"] = KEY_DOWN;
	keyNames["page_up"] = KEY_PAGE_UP;
	keyNames["page_down"] = KEY_PAGE_DOWN;
	keyNames["home"] = KEY_HOME;
	keyNames["end"] = KEY_END;
	keyNames["insert"] = KEY_INSERT;
}


void KeyBinder::addController(Controller* controller) 
{
	controllers.push_back(controller);
}
void KeyBinder::removeController(Controller* controller) 
{
	controllers.remove(controller);
}

void KeyBinder::processKeyboard(unsigned char key, int x, int y)
{
	
	//cout << "process : " << (int)key << endl;
	int command = 0;
	if (localBinding!=NULL) {
		command = (*localBinding)[key];
	}

	if (command == 0) {
		command = globalBinding[key];
	}
	//cout << "command : " << (int)command << endl;

	if (command != 0) {
		for (list<Controller*>::iterator it = controllers.begin(); it != controllers.end(); ++it) {
			(*it)->processCommand(command);
		}
	}
}

void KeyBinder::bindKey(char key, int command)
{
	if ( globalBinding[key] != 0) {
		cout << "[KeyBinder] Key " << ((int)key) << " is already bound " << endl;
	}
	globalBinding[key] = command;

	
	/* Checking the bindings
	std::ostringstream oss;
	oss << key;

	for (map<string,char>::iterator itKey = keyNames.begin(); itKey != keyNames.end(); ++itKey) {
		if (itKey->second == key) {
			oss.str(itKey->first.c_str());
			break;
		}
	}
	cout << "[KeyBinder] " << oss.str();
	oss.str("");
	oss << "[" << command << "]" ;
	
	for (map<string,int>::iterator itCommand = commandNames.begin(); itCommand != commandNames.end(); ++itCommand) {
		if (itCommand->second == command) {
			oss.str(itCommand->first.c_str());
			break;
		}
	}
	cout << " : " << oss.str() << endl;
	*/
}

void KeyBinder::bindKey(int localIndex, char key, int command)
{
	if (localBindings[localIndex].size() < 256) {
		localBindings[localIndex].resize(256);
	}
	localBindings[localIndex][key] = command;
}

void KeyBinder::setLocal(int localIndex) {
	if (localIndex == 0) {
		localBinding = NULL;
	} else {
		if (localBindings.find(localIndex) != localBindings.end()) {
			localBinding = &localBindings[localIndex];
		} else {
			localBinding = NULL;
		}
	}

}


void KeyBinder::readConfig(ConfigReader& configReader)
{
	istringstream iss;
	map<string,map<string,string>>::iterator ci = configReader.find("shortcut");
	if (ci != configReader.end()) {
		map<string,string>& shotcuts = ci->second;


		for (map<string,string>::iterator it = shotcuts.begin(); it != shotcuts.end(); ++it) {
			const string& title =  it->first;
			string content = it->second;

			transform(content.begin(), content.end(), content.begin(), toupper);
			
			char key = 0;
			int command = 0;

			if (title.size() <= 0 || content.size() <= 0) continue;

			if (title.size() == 1) {
				key = title[0];
			} else {
				for (map<string,char>::iterator itKey = keyNames.begin(); itKey != keyNames.end(); ++itKey) {
					const string& keyName = itKey->first;
					if (keyName.compare(title) == 0) {
						key = itKey->second;
						break;
					}
				}
			}

			for (map<string,int>::iterator itCommand = commandNames.begin(); itCommand != commandNames.end(); ++itCommand) {
				const string& commandName = itCommand->first;
				if (commandName.compare(content) == 0) {
					command = itCommand->second;
					break;
				}
			}

			if (key != 0 && command != 0) {
				bindKey(key,command);
			}
		}
	}
}