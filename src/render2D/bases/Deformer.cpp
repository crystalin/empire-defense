#include "render2D_common.h"
#include "Deformer.h"

Deformer::Deformer(Animation* animation, int time) :
Animation(time,1), animation(animation)
{
}

Deformer::~Deformer() {
	delete animation;
}

void Deformer::update(int timeSpent) { 
	Animation::update(timeSpent);
	return animation->update(timeSpent); 
}

bool Deformer::preDisplay(int addTime)
{
	animation->preDisplay(addTime);
	return true;
}

void Deformer::postDisplay(int addTime)
{
	animation->postDisplay(addTime);
}
