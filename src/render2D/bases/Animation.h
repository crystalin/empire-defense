#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Color.h"

class Animation
{
public:
	Animation(int time = 1000, int loop = 0, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), float speed = 1.0f, Color color = Color());
	virtual ~Animation();

	virtual void start() { state = 1; if (timeLeft <= 0 || timeLeft > timeBase) timeLeft = timeBase; };
	virtual void stop() { state = 0; };
	virtual void update(int timeSpent);
	virtual bool preDisplay(int addTime = 0) { return true; }
	virtual void postDisplay(int addTime = 0) {}
	virtual void display(int addTime = 0) {if (!state) return; if (preDisplay(addTime)) postDisplay(addTime); }
	virtual bool isFinished(){ return (loop!=1 && timeLeft <= 0); };

	virtual int getTimeBase() { return timeBase; }
	virtual int getTimeLeft() { return timeLeft; }
	virtual int getLoop() { return loop; }
	virtual Rectangle<float> getArea() { return area; }
	virtual float getSpeed() { return speed; }
	virtual Color getColor() { return color; }
	virtual int getState() { return state; }
	
	virtual void setTimeLeft(int timeLeft) { this->timeLeft = timeLeft; }
	virtual void setLoop(int loop) { this->loop = loop; }
	virtual void setArea(const Rectangle<float>& area) { this->area = area; }
	virtual void setTime(int timeBase) { this->timeBase = timeBase; }
	virtual void setSpeed(float speed) { this->speed = speed; }

	static void addAnimation(Animation* animation) { animations.push_back(animation); }
	static void removeAnimation(Animation* animation) { animations.remove(animation); }
	static void updateAnimations(int timeSpent);
	static void displayAnimations();
	static void clearAnimations();
	static int getAnimationsCount() { return animations.size();}

protected:

	int timeBase;
	int timeLeft;
	int loop;
	Rectangle<float> area;
	float speed;
	Color color;

	int state;

	static list<Animation*> animations;

};

#endif
