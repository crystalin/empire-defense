#ifndef __PCCONTROLLER_H__
#define __PCCONTROLLER_H__

#define MODE_NONE 0
#define MODE_BUILD 1

#include "KeyBinder.h"
#include "controller.h"

class GraphicEngine;

class EDCONTROLLER_API PCController : public Controller, public Observer
{
public:
	PCController(Model* model, UserInterface* userInterface);
	virtual ~PCController();

	virtual void processCommand(int ID);
	virtual void idle(int timeSpent);
	virtual void init(int argc, char** argv);
	virtual void start();
	virtual void bindKey(char key, int command);
	virtual void addInputParser(InputParser* parser);
	
	virtual void notifyLoadTowerTemplates(const vector<const TowerTemplate*>& towerTemplates);

	virtual Player* getPlayer() {return player;}

protected:

	KeyBinder* keyBinder;

	Party* party;
	const Object* selectedObject;
	bool constructionMode;
	const TowerTemplate* towerConstruction;
	int timeBase;
	
	void selectObject(const Case* tcase);

	// tests
	vector<Player*> players;
	
};

#endif