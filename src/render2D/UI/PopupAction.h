#ifndef __POPUP_ACTION_H__
#define __POPUP_ACTION_H__

#include "Item.h"
#include "UIAction.h"

class PopupAction : public UIAction
{
public:
	PopupAction(Item* item);
	virtual ~PopupAction();

	virtual void update(int timeSpent) {item->update(timeSpent); }

	virtual void mouseEnter(const Point2D<float>& mousePosition) {active = true; item->setVisible(true); }
	virtual void mouseLeave() { active = false; item->setVisible(false);  }
	virtual void mousePress(const Point2D<float>& mousePosition) {}
	virtual void mouseRelease(const Point2D<float>& mousePosition, bool click) {}

	virtual bool isActive() { return active; }

	virtual void display() { if (active) item->display(); }

protected:

	Item* item;
};

#endif
