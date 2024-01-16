#ifndef __ZOOMING_DEFORMER_H__
#define __ZOOMING_DEFORMER_H__

#include "Deformer.h"

class ZoomingDeformer : public Deformer
{
public:
	ZoomingDeformer(Animation *animation, float maxZoom);

	virtual void start() { return animation->start(); currentZoom = maxZoom; }
	virtual void update(int timeSpent);
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);

	virtual Rectangle<float> getArea() { return animation->getArea(); }

protected:

	float maxZoom;
	float currentZoom;
	bool increasing;

};

#endif
