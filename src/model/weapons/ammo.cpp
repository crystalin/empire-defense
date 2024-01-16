#include "model_common.h"
#include "ammo.h" 
#include "tower.h" 
#include "effect.h"
#include "model.h" 
#include "party.h"

static int idStatic = 0;

AmmoTemplate::AmmoTemplate(int id, string name, int time, int type, float damage, float radius, float speed) :
id(id), name(name), time(time), type(type), damage(damage), radius(radius), speed(speed)
{
}

Ammo::Ammo(Party* party, const AmmoTemplate& ammoTemplate, const Tower& tower): 
Targeter(party, tower.getPosition()),ammoTemplate(ammoTemplate), target(tower.getTarget()),
effectTemplate(tower.getEffectTemplate()), timeLeft(ammoTemplate.time),
destination(target->getPosition()), origin(tower.getPosition()), height(0.1f), id(idStatic++)
{
	if (ammoTemplate.type == TYPE_AMMO_PROJECTILE) {
		target = NULL; // don't keep the target if it is a projectile
	} else {
		target->addTargeter(this);
	}
	//cout << "--===-- creating ammo " << id << "--===--" << endl;
}

Ammo::~Ammo()
{
	if (target!=NULL) {
		target->removeTargeter(this);
	}
}

/*
void Ammo::setTarget(Targetable* newTarget) 
{
	if (target!=NULL) {
		target->removeTargeter(this);
	}
	target = newTarget;
	if (target!=NULL) {
		destination = target->getPosition();
		if (ammoTemplate.type == TYPE_AMMO_PROJECTILE) {
			target = NULL; // don't keep the target if it is a projectile
		} else {
			target->addTargeter(this);
		}
	} else {
		//cout << " null target " << endl;
	}
}*/

void Ammo::hitAround(Targetable* targetable)
{
	if (target == targetable) {
		return;
	} else {
		float radius = getRadius();
		if (position.squaredDistance(targetable->getPosition()) <= radius*radius) {
			targetable->hitted(this);
		}
	}
}

void Ammo::hit(){

	//cout << "ammo[" << id << "]  hitted !! [" << origin <<" -> " << position << "]" << endl;
	float radius = getRadius();
	if (radius <= 0) {
		if (target != NULL) {
			target->removeTargeter(this);
			target->hitted(this);
			target=NULL;
		}
	} else {
		if (target != NULL) {
			target->removeTargeter(this);
			target->hitted(this);
			effectTemplate = NULL;
		}
		const vector<Unit*>& units = party.getUnits();
		for_each(units.begin(), units.end(), bind1st(mem_fun(&Ammo::hitAround),this));
		target=NULL;
	}

	party.notifyAmmoHit(this);
}


bool Ammo::update(int timeSpent)
{
	timeLeft -= timeSpent;

	if (timeLeft <= 0) {
		//cout << "ammo [" << id << "] ended by time!" <<endl ;
		if (target != NULL) {
			target->removeTargeter(this);
			target = NULL;
		}
		return true;
	}
	if (target!=NULL) {
		destination = target->getPosition();
	}

	float distanceDone = (timeSpent*ammoTemplate.speed)/1000.0f;

	Point2D<float> diff = destination - position;
	float distanceToNextPoint = diff.x*diff.x+diff.y*diff.y;

	float ratioX = 0.0f;
	float ratioY = 0.0f;

	if (diff.x != 0.0f) {
		ratioX = diff.x / (abs(diff.x) + abs(diff.y)) ;
	}
	if (diff.y != 0.0f) {
		ratioY = diff.y / (abs(diff.x) + abs(diff.y)) ;
	}

	
	if (ammoTemplate.type == TYPE_AMMO_PROJECTILE) {

		float lastHeight = height;
		float distanceAll = origin.distance(destination);
		float poucentDistanceDone = 0.0f;
		float diffY = (destination.y-origin.y) ;
		float diffX = (destination.x-origin.x) ;
		if (diffX  != 0.0f || diffY != 0.0f) {
			if (abs(diffX) < abs(diffY)) {
				poucentDistanceDone = (position.y-origin.y) / diffY;
			} else {
				poucentDistanceDone = (position.x-origin.x) / diffX;
			}
		}
		height = sin(poucentDistanceDone * PI) * (distanceAll) / getSpeed();
		float diffHeight = abs(height - lastHeight);

		distanceDone -= diffHeight;

	}

	if (distanceToNextPoint < distanceDone*distanceDone) {
		distanceDone = distanceToNextPoint;
	}

	float distanceToNextPointX = distanceDone * ratioX;
	float distanceToNextPointY = distanceDone * ratioY;

	position.x += distanceToNextPointX;
	position.y += distanceToNextPointY;
	
	if (position && destination) {
		hit();
		//cout << "ammo [" << id << "] update true after hit !" <<endl ;
		return true;
	}
	return false;
}

void Ammo::notifyDeadTarget(Targetable* targetable)
{
	//cout << *this << "notify dead target" << endl;
	this->target = NULL;
}
