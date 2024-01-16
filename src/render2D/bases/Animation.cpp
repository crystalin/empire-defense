#include "render2D_common.h"
#include "Animation.h" 


list<Animation*> Animation::animations = list<Animation*>();

Animation::Animation(int time, int loop, const Rectangle<float>& area, float speed, Color color) :
timeLeft(time*speed), loop(loop), timeBase(time*speed), state(1), area(area), speed(speed), color(color)
{
	if (time<=10) {
		this->timeBase = 100;
		this->timeLeft = 100;
	}
}
Animation::~Animation() {
}

void Animation::update(int timeSpent)
{
	if (state == 0) return ;

	timeLeft -= timeSpent*speed;
	if (timeLeft <= 0 && loop == 1) {
		timeLeft = (timeLeft % timeBase) + timeBase;
	}
}

void Animation::updateAnimations(int timeSpent)
{
	for (list<Animation*>::iterator ci = animations.begin(); ci != animations.end();) {
		Animation* animation = *ci;
		animation->update(timeSpent);
		if (animation->isFinished()) {
			ci = animations.erase(ci);
			delete animation;
		} else {
			++ci;
		}
	}
}

void Animation::displayAnimations()
{
	for (list<Animation*>::iterator ci = animations.begin(); ci != animations.end(); ++ci) {
		Animation* animation = *ci;
		animation->display();
	}
}

void Animation::clearAnimations()
{
	for (list<Animation*>::iterator ci = animations.begin(); ci != animations.end(); ++ci) {
		delete *ci;
	}
	animations.clear();
}