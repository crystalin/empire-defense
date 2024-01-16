#ifndef __ROTATING_DEFORMER_H__
#define __ROTATING_DEFORMER_H__

#include "Deformer.h"

class RotatingDeformer : public Deformer
{
public:
	RotatingDeformer(Animation *animation, int time = 1000, bool clockWise = true);

	virtual void start() { return animation->start(); currentRotate = 0; }
	virtual void update(int timeSpent);
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:

	bool clockWise;
	Point2D<float> shiftArea;
	float currentRotate;

};

#endif
