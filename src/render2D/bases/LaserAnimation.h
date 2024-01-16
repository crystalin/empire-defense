#ifndef __LASER_ANIMATION_H__
#define __LASER_ANIMATION_H__

#include "Animation.h"
#include "Color.h"
#include "Texture.h"

#define LASER_SIZE 0.2f

class LaserAnimation : public Animation
{
public:
	LaserAnimation(Point2D<float> origin, Point2D<float> destination, int time = 1000, int loop = 0, const Rectangle<float>& area = Rectangle<float>(0,0,48,16), float speed = 1.0f, Color color = Color() );
	virtual ~LaserAnimation();
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
	virtual void update(int timeSpent);
	virtual bool isFinished(){ return (distanceDone[0] >= distanceMax) && loop == 0; };

	virtual void setOrigin(const Point2D<float>& origin) { this->origin = origin; angle = origin.angle(destination); distanceMax = origin.distance(destination); }
	virtual void setDestination(const Point2D<float>& destination) { this->destination = destination; angle = origin.angle(destination); distanceMax = origin.distance(destination); }

protected:
	float angle;
	float distanceMax;
	float distanceDone[2];
	
	Point2D<float> origin;
	Point2D<float> destination;


	static Texture* laserTexture;

};

#endif
