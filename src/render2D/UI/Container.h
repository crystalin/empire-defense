#ifndef __CONTAINER_UI_H__
#define __CONTAINER_UI_H__

#include "Item.h"

template<class T1>
class Container : public Item
{
public:
	Container(PositionController* positionController = new DefaultPositionController());

	virtual void update(int timeSpent);
	virtual bool contains(const Point2D<float>& mousePosition);

	virtual void mouseEnter(const Point2D<float>& mousePosition);
	virtual void mouseMove(const Point2D<float>& mousePosition);
	virtual void mouseLeave();
	virtual void mousePress(const Point2D<float>& mousePosition);
	virtual void mouseRelease(const Point2D<float>& mousePosition, bool click);

	virtual void setVisible(bool visible);
	virtual void display();
	
	virtual void clear();
	virtual void addItem(T1* item);
	virtual void removeItem(T1* item);

protected:
	virtual ~Container();

	list<T1*> items;
	T1* itemFocused;
	T1* itemPressed;
};


template<class T1>
Container<T1>::Container(PositionController* positionController) :
Item(positionController), items(), itemFocused(NULL), itemPressed(NULL)
{
}

template<class T1>
Container<T1>::~Container()
{
	for (list<T1*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		(*ci)->release();
	}
	items.clear();
}

template<class T1>
bool Container<T1>::contains(const Point2D<float>& mousePosition)
{
	if (!visible) {
		return false;
	}

	const Point2D<float>& position = positionController->getPosition();
	Point2D<float> subPosition(mousePosition - position);

	for (list<T1*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		T1* item = *ci;
		if (item->contains(subPosition)) {
			return true;
		}
	}

	return false;
}

template<class T1>
void Container<T1>::mouseMove(const Point2D<float>& mousePosition)
{
	if (!visible || !inside) {
		return;
	}

	const Point2D<float>& position = positionController->getPosition();
	Point2D<float> subPosition(mousePosition - position);
	Item::mouseEnter(subPosition);

	if (itemFocused!=NULL) {
		if (itemFocused->contains(subPosition)) {
			itemFocused->mouseMove(mousePosition);
			return;
		} else {
			itemFocused->mouseLeave();
			itemFocused = NULL;
		}
	}

	for (list<T1*>::iterator ci = items.begin(); ci != items.end() && itemFocused==NULL; ++ci) {
		T1* item = *ci;
		if (item->contains(subPosition)) {
			item->mouseEnter(subPosition);
			itemFocused = item;
		}
	}
}

template<class T1>
void Container<T1>::mouseEnter(const Point2D<float>& mousePosition)
{
	if (!visible || inside) {
		return;
	}

	const Point2D<float>& position = positionController->getPosition();
	Point2D<float> subPosition(mousePosition - position);
	Item::mouseEnter(subPosition);

	if (itemFocused!=NULL) {
		if (itemFocused->contains(subPosition)) {
			return;
		} else {
			itemFocused->mouseLeave();
			itemFocused = NULL;
		}
	}

	for (list<T1*>::iterator ci = items.begin(); ci != items.end() && itemFocused==NULL; ++ci) {
		T1* item = *ci;
		if (item->contains(subPosition)) {
			item->mouseEnter(subPosition);
			itemFocused = item;
		}
	}
}

template<class T1>
void Container<T1>::mouseLeave()
{
	if (!visible || !inside) return;

	Item::mouseLeave();

	if (itemFocused!=NULL) {
		itemFocused->mouseLeave();
		itemFocused = NULL;
	}
}

template<class T1>
void Container<T1>::mousePress(const Point2D<float>& mousePosition)
{
	if (!visible || !inside) return;
	
	const Point2D<float>& position = positionController->getPosition();
	Point2D<float> subPosition(mousePosition - position);
	Item::mousePress(subPosition);

	if (itemPressed!=NULL) {
		if (itemPressed->contains(subPosition)) {
			return;
		} else {
			itemPressed->mouseRelease(subPosition,false);
			itemPressed = NULL;
		}
	}

	if (itemFocused!=NULL) {
		itemFocused->mousePress(subPosition);
		itemPressed = itemFocused;
	}
}

template<class T1>
void Container<T1>::mouseRelease(const Point2D<float>& mousePosition, bool click)
{
	if (!visible || !pressed) return;

	const Point2D<float>& position = positionController->getPosition();
	Point2D<float> subPosition(mousePosition - position);
	Item::mouseRelease(subPosition,click);

	if (itemPressed!=NULL) {
		if (itemPressed->contains(subPosition)) {
			itemPressed->mouseRelease(subPosition,click);
		} else {
			itemPressed->mouseRelease(subPosition,false);
		}
		itemPressed = NULL;
	}
	return;
}

template<class T1>
void Container<T1>::clear()
{
	for (list<T1*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		(*ci)->release();
	}
	items.clear();
}

template<class T1>
void Container<T1>::addItem(T1* item) {
	item->request();
	items.push_back(item);
	item->setVisible(visible);
}

template<class T1>
void Container<T1>::removeItem(T1* item) {
	items.remove(item);
	if (itemFocused == item) {
		itemFocused = NULL;
	}
	if (itemPressed == item) {
		itemPressed = NULL;
	}
	item->release();
}

template<class T1>
void Container<T1>::update(int timeSpent)
{
	if (!visible) return;
	Item::update(timeSpent);

	for (list<T1*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		T1* item = *ci;
		item->update(timeSpent);
	}
}

template<class T1>
void Container<T1>::setVisible(bool visible)
{
	this->visible = visible;
	for (list<T1*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		T1* item = *ci;
		item->setVisible(visible);
	}
}

template<class T1>
void Container<T1>::display()
{
	if (!visible) return;

	const Point2D<float>& position = positionController->getPosition();
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);

	if (!items.empty()) {
		for (list<T1*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
			T1* item = *ci;
			item->display();
		}
	}

	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->display();
		}
	}
	glPopMatrix();
}

#endif
