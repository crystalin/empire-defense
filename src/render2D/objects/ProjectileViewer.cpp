#include "render2D_common.h"
#include "ProjectileViewer.h" 
#include "ammo.h" 
#include "texture.h"
#include "RessourceManager.h"

Texture* ProjectileViewer::ballTexture = NULL;
Texture* ProjectileViewer::arrowTexture = NULL;
Texture* ProjectileViewer::laserTexture = NULL;
Texture* ProjectileViewer::missileTexture = NULL;
Texture* ProjectileViewer::shadowTexture = NULL;

void ProjectileViewer::init()
{
	ballTexture = RessourceManager::instance()->getTexture("projectile.png");
	ballTexture->request();
	arrowTexture = RessourceManager::instance()->getTexture("arrow.png");
	arrowTexture->request();
	laserTexture = RessourceManager::instance()->getTexture("laser.png");
	laserTexture->request();
	missileTexture = RessourceManager::instance()->getTexture("missile.png");
	missileTexture->request();
	shadowTexture = RessourceManager::instance()->getTexture("shadow.png");
	shadowTexture->request();
}
void ProjectileViewer::destroy()
{
	ballTexture->release();
	arrowTexture->release();
	laserTexture->release();
	missileTexture->release();
	shadowTexture->release();
}

void ProjectileViewer::display(Ammo* ammo)
{
	float height = ammo->getHeight();
	float direction = ammo->getDirection();
	const Point2D<float>& origin = ammo->getOrigin();
	const Point2D<float>& position = ammo->getPosition();
	const Point2D<float>& destination = ammo->getDestination();

	float halfSize = 0.1f;
	float shadowSize = (height)/5.0f+halfSize;


	//cout << "high : " << high << endl;

	glPushMatrix();
	glTranslatef(ammo->getPosition().x + 0.5f, ammo->getPosition().y + 0.5f,0.0f);

	switch (ammo->getType()) {
		case TYPE_AMMO_LASER : 
			{
			glRotatef(direction,0,0,1);
			/*float totalDistance = destination.distance(origin);
			if (totalDistance == 0) {
				break;
			}
			float ratioIncrease =  (totalDistance + LASER_LENGHT) / totalDistance;*/

			float distanceDone = position.distance(origin);
			float distanceLeft = destination.distance(position);

			//float distancePourcent = distanceDone / (distanceDone + distanceLeft);
			//float laserShift = LASER_LENGTH * distancePourcent;

			float laserLength = min(distanceDone,min(distanceLeft, LASER_LENGTH));
			float halfLaserLength = laserLength/2.0f;
			laserTexture->drawSquare(Rectangle<float>(-halfLaserLength, -0.3f + height, halfLaserLength, 0.3f + height));
			}
			break;
		case TYPE_AMMO_ARROW : 
			{
			glRotatef(direction,0,0,1);

			float distanceDone = position.distance(origin);
			float distanceLeft = destination.distance(position);

			float laserLength = min(distanceDone,min(distanceLeft, ARROW_LENGTH));
			float halfLaserLength = laserLength/2.0f;
			arrowTexture->drawSquare(Rectangle<float>(-halfLaserLength, -0.1f + height, halfLaserLength, 0.1f + height));
			}
			break;
		default:
			ballTexture->drawSquare(Rectangle<float>(-halfSize, -halfSize + height, halfSize, halfSize + height));
			glColor4f(1.0, 1.0, 1.0, 1-shadowSize);
			shadowTexture->drawSquare(Rectangle<float>(-shadowSize, -shadowSize - 0.05f, shadowSize, shadowSize - 0.05f));
			break;
	}

	glPopMatrix();
}
