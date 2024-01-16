#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Item.h"
#include "Color.h"

class Controller;

class Button : public Item
{
public:
	Button(int commandID, float size = 1, Item* content = NULL, const Color& color = Color(0.0f,0.0f,0.2f,1.0f), PositionController* positionController = new DefaultPositionController());

	void update(int timeSpent);
	bool contains(const Point2D<float>& mousePosition);
	void mousePress(const Point2D<float>& mousePosition);
	void mouseRelease(const Point2D<float>& mousePosition, bool click);

	void setVisible(bool visible);
	void display();

	int getCommandID() const { return commandID; }
	float getSize() const { return size; }
	const Color& getColor() const { return color; }
	
	virtual ostream& print(ostream& os) const { return os << "Button " << id << " " << positionController->getPosition() ;}

	static Controller* controller;

protected:
	virtual ~Button();

	Item* content;
	float size;
	int commandID;
	Color color;
	Color colorInside;
	Color colorPressed;

};

#endif
