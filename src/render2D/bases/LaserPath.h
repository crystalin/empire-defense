#ifndef __LASER_PATH_H__
#define __LASER_PATH_H__

#include "Animation.h"
#include "Texture.h"

class LaserPathPoint 
{
public :
	LaserPathPoint(const Point2D<float>& position) : position(position) {}
	Point2D<float> position;
	float angleToNext;
	float distanceToNext;
	float decalage;
	float progression;


};

class LaserPath : public Animation
{
public:
	LaserPath(int time = 1000, int loop = 0, float decalage = 100.0f, float speed = 1.0, int count = 10, float length = 1.0f, Color color = Color());
	~LaserPath();
	void update(int timeSpent);
	bool preDisplay(int addTime = 0);
	void postDisplay(int addTime = 0);
	void addPoint(const Point2D<float>& position);
	void clearPath();
	void calculPoints();
	void displayLaser(LaserPathPoint& point);

	static LaserPath* createRoundLaser(const Point2D<float>& position, float radius, int time = 1000, int loop = 0, float decalage = 100.0f, float speed = 1.0, int count = 10, float length = 1.0f, Color color = Color());
protected:

	bool dirty;

	float length;
	float decalage;
	vector<LaserPathPoint> path;
	Texture* laserTexture;

};

#endif
