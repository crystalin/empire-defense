#include "render2D_common.h"
#include "MultiDeformer.h"

MultiDeformer::MultiDeformer(Animation* animation, Animation* decoration) :
Deformer(animation), decoration(decoration)
{
}

MultiDeformer::~MultiDeformer() {
	delete animation;
	delete decoration;
}

void MultiDeformer::update(int timeSpent) { 
	Animation::update(timeSpent);
	decoration->update(timeSpent);
	animation->update(timeSpent); 
}

bool MultiDeformer::preDisplay(int addTime)
{
	decoration->preDisplay(addTime);
	animation->preDisplay(addTime);
	return true;
}

void MultiDeformer::postDisplay(int addTime)
{
	animation->postDisplay(addTime);
	decoration->postDisplay(addTime);
}
