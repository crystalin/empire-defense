#ifndef __ITEM_H__
#define __ITEM_H__

#include "UIAction.h"
class UIObservable;

struct PositionController {
	virtual void setPosition(const Point2D<float>& position)  = 0;
	virtual const Point2D<float>& getPosition() = 0;
	virtual void update(int timeSpent) = 0;
};

struct DefaultPositionController : public PositionController {
private:
	Point2D<float> position;
public:
	DefaultPositionController(const Point2D<float>& position = Point2D<float>(0,0)) : position(position) {}
	virtual void setPosition(const Point2D<float>& position) { this->position = position; }
	virtual const Point2D<float>& getPosition() { return this->position; }
	virtual void update(int timeSpent) {}
};

template <int Distance = 10, int During = 100>
struct FloatingPositionController : public PositionController {
private:
	Point2D<float> position;
	Point2D<float> originalPosition;
	int time;
	float speed;
	int direction;
public:
	FloatingPositionController(const Point2D<float>& position = Point2D<float>(0,0)) : originalPosition(originalPosition), position(position), time(0), direction(1), speed(1.0f) {}
	virtual void setPosition(const Point2D<float>& position) { this->position = position; this->originalPosition = position; calculateFloatingPosition();}
	virtual const Point2D<float>& getPosition() { return this->position; }
	virtual void update(int timeSpent) {
		time += timeSpent * speed ;
		if (time >= During) {
			direction = -1;
		}
		if (time <= -During) {
			direction = 1;
		}
		speed = min( 1.0f, max(-1.0f, speed + direction/(float)Distance));

		calculateFloatingPosition();
	}
	void calculateFloatingPosition() {
		position.x = originalPosition.x + (int)((time/(float)During) * Distance);
	}
};

class Item
{
public:
	Item(PositionController* positionController = new DefaultPositionController());

	virtual void update(int timeSpent);
	virtual bool contains(const Point2D<float>& mousePosition) = 0;

	virtual void mouseMove(const Point2D<float>& mousePosition);
	virtual void mouseEnter(const Point2D<float>& mousePosition);
	virtual void mouseLeave();
	virtual void mousePress(const Point2D<float>& mousePosition);
	virtual void mouseRelease(const Point2D<float>& mousePosition, bool click);

	virtual bool isInside() { return inside; }
	virtual bool isPressed() { return pressed; }

	virtual void setVisible(bool visible);
	virtual bool isVisible() { return visible; }
	virtual void setEnable(bool enable);
	virtual bool isEnable() { return enable; }
	virtual void setPosition(const Point2D<float>& position) { positionController->setPosition(position); }
	virtual const Point2D<float>& getPosition() { return positionController->getPosition(); }

	virtual void display();

	virtual void addAction(UIAction* action) { actions.push_back(action); }

	virtual ostream& print(ostream& os) const { return os << "Item " << id << " " << positionController->getPosition() ;}

	static void addGlobalItem(Item* item) { globalItems.push_back(item); item->request(); }
	static void removeGlobalItem(Item* item) { globalItems.remove(item); item->release();}
	static void updateGlobalItems(int timeSpent);
	static void displayGlobalItems();
	static void clearGlobalItems();
	static const list<Item*>& getGlobalItems() {return globalItems; }

	void request() { refCount++; }
	void release() { refCount--; if (refCount ==0) delete this; }

	
	static const UIObservable* uiObservable;
protected:
	virtual ~Item();

	static list<Item*> globalItems;
	

	int id;

	PositionController* positionController;
	bool visible;
	bool inside;
	bool pressed;
	bool enable;

	list<UIAction*> actions;

	int refCount;

};

ostream& operator << (ostream& os, const Item& item);
#endif
