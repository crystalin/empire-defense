#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "commands.h"
#include "InputParser.h"
#include "TimeManager.h"
#include "model.h"
#include "UserInterface.h"

class Player;

class EDCONTROLLER_API Controller : public TimeManager
{
public:
	Controller(Model* model, UserInterface* userInterface) : model(model), userInterface(userInterface), player(NULL) {}
	virtual ~Controller() {}

	virtual void processCommand(int ID) = 0;
	virtual void idle(int timeSpent) = 0;
	virtual void init(int argc, char** argv) = 0;
	virtual void start() = 0;
	virtual void bindKey(char key, int command) = 0;
	virtual void addInputParser(InputParser* parser) = 0;

	virtual Player* getPlayer() {return player;}

protected:

	Model* model;
	UserInterface* userInterface;
	Player* player;
};

#endif