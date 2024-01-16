#include "render2D_common.h"
#include "ZoomingDeformer.h"

ZoomingDeformer::ZoomingDeformer(Animation* animation, float maxZoom) : 
Deformer(animation), maxZoom(maxZoom), currentZoom(0.0f), increasing(true)
{
}

void ZoomingDeformer::update(int timeSpent) { 
	Deformer::update(timeSpent);
	if (increasing) {
		currentZoom += timeSpent/1000.0f;
		if (currentZoom >= maxZoom) {
			currentZoom = maxZoom - (currentZoom-maxZoom);
			increasing = false;
		}
	} else {
		currentZoom -= timeSpent/1000.0f;
		if (currentZoom <= -maxZoom) {
			currentZoom = -maxZoom - (currentZoom+maxZoom);
			increasing = true;
		}
	}
}

bool ZoomingDeformer::preDisplay(int addTime)
{
	glPushMatrix();
	animation->preDisplay(addTime);
	glScalef(currentZoom, currentZoom, 1.0f);
	return true;
}


void ZoomingDeformer::postDisplay(int addTime)
{
	animation->postDisplay(addTime);
	glPopMatrix();
}