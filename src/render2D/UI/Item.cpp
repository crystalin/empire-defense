#include "render2D_common.h"
#include "Item.h" 
#include "FormGenerator.h" 

list<Item*> Item::globalItems = list<Item*>();
const UIObservable* Item::uiObservable = NULL;

Item::Item(PositionController* positionController) :
positionController(positionController), visible(true), inside(false), pressed(false), actions(), refCount(0), enable(true)
{
	static int count = 0;
	id = count ++;
	//cout << *this << " created" << endl;
}

Item::~Item()
{
	for (list<UIAction*>::const_iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		delete (*ci);
	}
	actions.clear();
	delete positionController;
	//cout << *this << " deleted" << endl;
}

void Item::update(int timeSpent)
{
	positionController->update(timeSpent);
	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->update(timeSpent);
		}
	}
}

void Item::setVisible(bool visible)
{ 
	this->visible = visible;
	if (visible == false) {
		inside = false;
		pressed = false;
	}
}

void Item::setEnable(bool enable)
{ 
	this->enable = enable;
}

void Item::mouseMove(const Point2D<float>& mousePosition)
{
}

void Item::mouseEnter(const Point2D<float>& mousePosition) 
{ 
	if (!visible) return;
	inside = true;
	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		action->mouseEnter(mousePosition);
	}
}

void Item::mouseLeave() 
{ 
	if (!visible) return;
	inside = false;
	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		action->mouseLeave();
	}
}

void Item::mousePress(const Point2D<float>& mousePosition) 
{ 
	if (!visible) return;
	pressed = true;
	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		action->mousePress(mousePosition);
	}
}

void Item::mouseRelease(const Point2D<float>& mousePosition, bool click)
{
	if (!visible) return;
	pressed = false;
	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		action->mouseRelease(mousePosition,click);
	}
}

void Item::display()
{
	glPushMatrix();
	const Point2D<float>& position = positionController->getPosition();

	if (position.x != 0.0f || position.y != 0.0f) {
		glTranslatef(position.x, position.y,0.0f);
	}
	if (inside) {
		glColor3f(0.5,1.0,0.5);
	} else {
		glColor3f(0.8,1.0,0.8);
	}
	FormGenerator::drawSquare();

	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->display();
		}
	}
	glPopMatrix();
}

void Item::updateGlobalItems(int timeSpent)
{
	for (list<Item*>::iterator ci = globalItems.begin(); ci != globalItems.end(); ++ci) {
		Item* item = *ci;
		item->update(timeSpent);
	}
}

void Item::displayGlobalItems()
{
	for (list<Item*>::iterator ci = globalItems.begin(); ci != globalItems.end(); ++ci) {
		Item* item = *ci;
		item->display();
	}
}

void Item::clearGlobalItems()
{
	for (list<Item*>::iterator ci = globalItems.begin(); ci != globalItems.end(); ++ci) {
		(*ci)->release();
	}
	globalItems.clear();
}

ostream& operator << (ostream& os, const Item& item)
{
	return item.print(os);
}