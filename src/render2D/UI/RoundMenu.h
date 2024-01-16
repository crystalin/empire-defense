#ifndef __ROUND_MENU_H__
#define __ROUND_MENU_H__

#include "Menu.h"

class Texture;

#define ROUND_MENU_LASER_TIME 300

class RoundMenu : public Menu
{
public:
	RoundMenu(PositionController* positionController = new DefaultPositionController());

	virtual void update(int timeSpent);
	virtual void display();

	virtual void clear();
	virtual void addItem(Button* button);

	virtual ostream& print(ostream& os) const { return os << "Menu " << id << " " << positionController->getPosition() ;}


protected:
	virtual ~RoundMenu();

	Texture* laserTexture;
	int translateTime;
	bool dirty;

	virtual void calculPositions();

};

#endif
