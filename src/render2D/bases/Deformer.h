#ifndef __DEFORMER_H__
#define __DEFORMER_H__

#include "Animation.h"

class Deformer : public Animation
{
public:
	Deformer(Animation *animation, int time = 10000);
	virtual ~Deformer();

	virtual void start() { return animation->start(); }
	virtual void stop() { return animation->stop(); }
	virtual void update(int timeSpent);
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
	virtual bool isFinished() { return animation->isFinished(); }

	virtual int getTimeBase() { return animation->getTimeBase(); }
	virtual int getTimeLeft() { return animation->getTimeLeft(); }
	virtual int getLoop() { return animation->getLoop(); }
	virtual Rectangle<float> getArea() { return animation->getArea(); }
	virtual float getSpeed() { return animation->getSpeed(); }
	virtual Color getColor() { return animation->getColor(); }
	virtual int getState() { return animation->getState(); }
	
	virtual void setTimeLeft(int timeLeft) { this->timeLeft = timeLeft; animation->setTimeLeft(timeLeft);}
	virtual void setLoop(int loop) { this->loop = loop; animation->setLoop(loop); }
	virtual void setArea(const Rectangle<float>& area) { this->area = area; animation->setArea(area); }
	virtual void setTime(int timeBase) { this->timeBase = timeBase; animation->setTime(timeBase); }
	virtual void setSpeed(float speed) { this->speed = speed; animation->setSpeed(speed); }

	Animation* getAnimation() { return animation; }

protected:

	Animation* animation;

};

#endif
