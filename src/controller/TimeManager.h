#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

class TimeManager abstract
{
public:
	virtual void idle(int timeSpent) = 0;
};

#endif
