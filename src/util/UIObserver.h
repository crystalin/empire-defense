#ifndef __USER_INTERFACE_OBSERVER__
#define __USER_INTERFACE_OBSERVER__

class Button;

class UIObserver
{
public:
	virtual ~UIObserver() {};

	virtual void		notifyPressButton(const Button* button) {}
	virtual void		notifyReleaseButton(const Button* button) {}
	
};

#endif
