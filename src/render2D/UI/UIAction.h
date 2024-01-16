#ifndef __UIACTION_H__
#define __UIACTION_H__

class UIAction
{
public:
	UIAction();
	virtual ~UIAction();

	virtual void update(int timeSpent) {};

	virtual void mouseEnter(const Point2D<float>& mousePosition) { active = true; };
	virtual void mouseLeave() { active = false; };
	virtual void mousePress(const Point2D<float>& mousePosition) {};
	virtual void mouseRelease(const Point2D<float>& mousePosition, bool click) {};

	virtual bool isActive() { return active; }

	virtual void display() {};

	virtual ostream& print(ostream& os) const { return os << "Action " << id ;}

protected:

	int id;
	bool active;
};

ostream& operator << (ostream& os, const UIAction& action);

#endif
