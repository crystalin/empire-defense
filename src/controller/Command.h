#ifndef __COMMAND_H__
#define __COMMAND_H__

class Command : 
{
public:
	Command(int type) : model(model), observer(observer) {observer->setController(this);}
	virtual ~Controller() {}

	virtual void processCommand(int ID) = 0;
	virtual void idle(void) = 0;
	virtual void start() = 0;

protected:

	Model* model;
	Observer* observer;
};

#endif