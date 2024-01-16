#include "render2D_common.h"
#include "LaserAnimation.h" 
#include "RessourceManager.h"

Texture* LaserAnimation::laserTexture = NULL;

LaserAnimation::LaserAnimation(Point2D<float> origin, Point2D<float> destination, int time, int loop, const Rectangle<float>& area, float speed, Color color ) : 
Animation(time,loop,area,speed,color), origin(origin), destination(destination), angle(origin.angle(destination)), distanceMax(origin.distance(destination)) 
{
	if (laserTexture==NULL) {
		laserTexture = RessourceManager::instance()->getTexture("laser.png");
	}
	laserTexture->request();
	distanceDone[0] = 0;
	distanceDone[1] = 0;
}

LaserAnimation::~LaserAnimation() {
	laserTexture = laserTexture->release();
}

void LaserAnimation::update(int timeSpent)
{
	timeLeft -= timeSpent * speed;

	float len = min(area.getSizeX(),distanceMax);
	float distanceSpent = timeSpent * speed;

	if (distanceDone[0] >= distanceMax) {
		if (loop ) {
			timeLeft += timeBase;
			distanceDone[1] = distanceDone[0] - distanceMax;
			distanceDone[0] = 0;
		}
	}
	if (distanceDone[1] >= len) {
		distanceDone[0] += distanceSpent;
		if (distanceDone[1] < distanceMax) {
			distanceDone[1] += distanceSpent;
		}
	} else {
		distanceDone[1] += distanceSpent;
	}
}

bool LaserAnimation::preDisplay(int addTime)
{
	if (distanceDone[0] > distanceMax - area.getSizeX()/15.0f ||
		distanceDone[1] < area.getSizeX()/15.0f) {
		return false;
	}

	glColor4fv(color.values);
	glPushMatrix();
	glTranslatef(origin.x,origin.y,0.0f);
	glRotatef(angle,0,0,1);
	return true;
}

void LaserAnimation::postDisplay(int addTime)
{
	float distanceDisplayDone[2];

	distanceDisplayDone[0] = min(distanceDone[0],distanceMax);
	distanceDisplayDone[1] = min(distanceDone[1],distanceMax);

	float texWidth = area.getSizeX()/3.0f;
	float texHeight = area.getSizeY();

	bool isExtended = (distanceDisplayDone[1] > (texWidth*2)) && (distanceDisplayDone[1] < (distanceMax-(texWidth*2)));

	if (!isExtended) {
		texWidth = (distanceDisplayDone[1] - distanceDisplayDone[0]) / 2.0f;
		//texHeight = texHeight * texWidth/(area.getSizeX()/3.0f);
	}
	
	float middleHeight = texHeight/2.0f;

	laserTexture->drawSquare(Rectangle<float>(distanceDisplayDone[0], -middleHeight, distanceDisplayDone[0] + texWidth, middleHeight), Rectangle<float>(0,0,1/3.0f, 1));
	if (isExtended) {
		laserTexture->drawSquare(Rectangle<float>(distanceDisplayDone[0] + texWidth , -middleHeight, distanceDisplayDone[1] - texWidth, middleHeight), Rectangle<float>(1/3.0f, 0, 2/3.0f, 1));
	}
	laserTexture->drawSquare(Rectangle<float>(distanceDisplayDone[1] - texWidth, -middleHeight, distanceDisplayDone[1], middleHeight), Rectangle<float>(2/3.0f, 0, 1.0f, 1.0f));

	glPopMatrix();
}