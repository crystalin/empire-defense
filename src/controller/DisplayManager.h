#ifndef __DISPLAY_MANAGER_H__
#define __DISPLAY_MANAGER_H__

class DisplayManager
{
public:
	virtual ~DisplayManager() {};
	virtual void initDisplay() = 0;
	virtual void display() = 0;
	virtual void resize(int w, int h) = 0;
};

#endif
