#ifndef __MULTI_ANIMATION_H__
#define __MULTI_ANIMATION_H__

#include "Animation.h"

class MultiAnimation : public Animation
{
public:
	MultiAnimation(Animation *animation1, Animation *animation2) : Animation(), animation1(animation1), animation2(animation2) {}
	virtual ~MultiAnimation() { delete animation1; delete animation2; }

	virtual void start() { Animation::start(); animation1->start(); animation2->start(); };
	virtual void stop() { Animation::stop(); animation1->stop(); animation2->stop(); };

	virtual void update(int timeSpent) { if(!state) return; animation1->update(timeSpent); animation2->update(timeSpent);}
	virtual void display(int addTime = 0) { if(!state) return; animation1->display(addTime); animation2->display(addTime);}
	virtual bool isFinished() { return animation1->isFinished() && animation2->isFinished(); }
	
	virtual void setTimeLeft(int timeLeft) { this->timeLeft = timeLeft; animation1->setTimeLeft(timeLeft); animation2->setTimeLeft(timeLeft);}
	virtual void setLoop(int loop) { this->loop = loop; animation1->setLoop(loop); animation2->setLoop(loop);}
	virtual void setArea(const Rectangle<float>& area) { this->area = area;  animation1->setArea(area); animation2->setArea(area);}
	virtual void setTime(int timeBase) { this->timeBase = timeBase;  animation1->setTime(timeBase); animation2->setTime(timeBase);}
	virtual void setSpeed(float speed) { this->speed = speed;  animation1->setSpeed(speed); animation2->setSpeed(speed);}

protected:

	Animation* animation1;
	Animation* animation2;

};

#endif