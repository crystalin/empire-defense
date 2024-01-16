#ifndef __USER_INTERFACE_OBSERVABLE__
#define __USER_INTERFACE_OBSERVABLE__

#include "UIObserver.h"
class Button;

class UIObservable
{
private:
	vector<UIObserver*> list_uiobservers;
public:
	UIObservable() { list_uiobservers.reserve(5); }
	void notifyPressButton(const Button* button) const { for_each(list_uiobservers.begin(), list_uiobservers.end(), bind(&UIObserver::notifyPressButton,_1,button)); }
	void notifyReleaseButton(const Button* button) const { for_each(list_uiobservers.begin(), list_uiobservers.end(), bind(&UIObserver::notifyReleaseButton,_1,button)); }

	void addUIObserver(UIObserver* observer)
	{
		this->list_uiobservers.push_back(observer);
	}
 
	void removeUIObserver(UIObserver* observer)
	{
		list_uiobservers.erase(find(list_uiobservers.begin(), list_uiobservers.end(), observer));
	}
};

#endif
