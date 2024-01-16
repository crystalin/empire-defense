#include "render2D_common.h"
#include "RotatingDeformer.h"

RotatingDeformer::RotatingDeformer(Animation* animation, int time, bool clockWise) : 
Deformer(animation, time), currentRotate(0.0f), clockWise(clockWise)
{
}

void RotatingDeformer::update(int timeSpent) { 
	Deformer::update(timeSpent);
	if (clockWise) {
		currentRotate += (timeSpent * 360) / (float)(timeBase);
	} else {
		currentRotate -= (timeSpent * 360) / (float)(timeBase);
	}

}

bool RotatingDeformer::preDisplay(int addTime)
{
	glPushMatrix();
	animation->preDisplay(addTime);
	Rectangle<float> animArea = animation->getArea();
	shiftArea = animArea.centerize();
	animation->setArea(animArea);
	glTranslatef(shiftArea.x, shiftArea.y, 0.0f);
	glRotatef(currentRotate, 0.0f, 0.0f, 1.0f);
	return true;
}

void RotatingDeformer::postDisplay(int addTime)
{
	animation->postDisplay(addTime);
	glPopMatrix();
	animation->setArea(animation->getArea() + shiftArea);
}
