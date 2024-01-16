#include "render2D_common.h" 
#include "LaserPath.h" 
#include "RessourceManager.h"


LaserPath::LaserPath(int time, int loop, float decalage, float speed, int count, float length, Color color) : 
Animation(time,loop,Rectangle<float>(0,0,100,100),speed,color), decalage(decalage), length(length), dirty(false)
{
	path.reserve(count);
	laserTexture = RessourceManager::instance()->getTexture("laser.png");
	laserTexture->request();
}

LaserPath::~LaserPath() {
	laserTexture->release();
}

void LaserPath::clearPath() {
	path.clear();
}

void LaserPath::addPoint(const Point2D<float>& position)
{
	if (path.size() == path.capacity()) {
		path.reserve(path.capacity()*2);
	}
	path.push_back(position);
	dirty = true;
}

void LaserPath::calculPoints()
{
	float pointDecalage = 0;
	float pourcentDecalage = decalage/path.size();
	dirty = false;
	if (path.size() > 1) {
		for(vector<LaserPathPoint>::iterator it = path.begin(); it != path.end() - 1; it++) {

			LaserPathPoint& point = *it;
			LaserPathPoint& nextPoint = *(it+1);

			point.angleToNext = point.position.angle(nextPoint.position);
			point.distanceToNext = point.position.distance(nextPoint.position);
			point.decalage = pointDecalage * point.distanceToNext;
			point.progression = 0.0f;

			pointDecalage += pourcentDecalage;

			
		}
	}
}


void LaserPath::update(int timeSpent)
{
	if (dirty) {
		calculPoints();
	}

	Animation::update(timeSpent);
	if (path.size() > 1) {
		for(vector<LaserPathPoint>::iterator it = path.begin(); it != path.end() - 1; it++) {
			LaserPathPoint& point = *it;
			point.progression = (timeLeft/1000.0f) * point.distanceToNext + point.decalage;
			if (point.progression >= point.distanceToNext) {
				point.progression = fmod(point.progression,point.distanceToNext);
			}
		}
	}
}

void LaserPath::displayLaser(LaserPathPoint& point)
{
	glPushMatrix();
	glTranslatef(point.position.x, point.position.y, 0.0f);
	glRotatef(point.angleToNext,0,0,1);
	glTranslatef( point.progression, 0.0f, 0.0f);
	float size = min((point.distanceToNext-point.progression),min(point.progression,length));
	laserTexture->drawSquare(Rectangle<float>(-size, -length/3.0f, size, length/3.0f));
	glPopMatrix();
}

bool LaserPath::preDisplay(int addTime)
{
	glColor4fv(color.values);
	return true;
}

void LaserPath::postDisplay(int addTime)
{
	if (path.size() > 1) {
		//for_each (path.begin(), path.end() - 1, bind1st(mem_fun_ref<void,LaserPath>(&LaserPath::displayLaser),this));
		for(vector<LaserPathPoint>::iterator it = path.begin(); it != path.end() - 1; it++) {
			displayLaser(*it);
		}
	}
}

LaserPath* LaserPath::createRoundLaser(const Point2D<float>& position, float radius, int time, int loop, float decalage, float speed, int count, float length, Color color)
{
	float shareAngle = PI*2 / (float)count;
	float angle = 0;

	LaserPath* laserPath = new LaserPath(time,loop,decalage,speed,count + 1,length,color);

	for (int i=0; i<count + 1; i++) {
		float cosEcart = cos(angle)*radius;
		float sinEcart = sin(angle)*radius;

		laserPath->addPoint(Point2D<float>(position.x + cosEcart,position.y + sinEcart));

		angle += shareAngle;
	}
	return laserPath;
}