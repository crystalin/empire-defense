#ifndef __MENU_H__
#define __MENU_H__

#include "Button.h"
#include "Container.h"

#define MENU_HORIZONTAL 1
#define MENU_VERTICAL 2

class Menu : public Container<Button>
{
public:
	Menu(PositionController* positionController = new DefaultPositionController(), int direction = MENU_HORIZONTAL);

	virtual void clear();
	virtual void addItem(Button* button);

	virtual ostream& print(ostream& os) const { return os << "Menu " << id << " " << positionController->getPosition() ;}

	int direction;

protected:


};

#endif
