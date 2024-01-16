#include "model_common.h"
#include "unit.h"
#include "ammo.h"
#include "Path.h"
#include "party.h"
#include "map.h"
#include "factory.h"

static int idStatic = 0;

UnitTemplate::UnitTemplate(int id, string name, int level, float maxLife, float speed, float armor, bool invisible) :
id(id), name(name), level(level), maxLife(maxLife), speed(speed), armor(armor), invisible(invisible)
{
}

Unit::Unit(Party* party, const UnitTemplate& unitTemplate, Path* path) :
Targetable(party, path->begin()->getPosition()), unitTemplate(unitTemplate), pathIterator(path->begin()), 
life(unitTemplate.maxLife), path(path), distanceDone(0.0f), id(idStatic++)
{
	for (int i=0; i<EFFECT_NUMBER; i++) {
		effects[i] = NULL;
	}
	
	path->addWalker(this);
	//cout << "[Unit] next point " << pathIterator->getPosition() << " - Angle : " << pathIterator->getAngleToNext() << endl;

	//cout << *this << " created" << endl;
}

Unit::~Unit() {
	for (int i=0; i<EFFECT_NUMBER; i++) {
		if (effects[i] != NULL) {
			Factory<EffectTemplate,Effect>::store(effects[i]);
			effects[i] = NULL;
		}
	}
	//cout << *this << " deleted" << endl;
}

/*void Unit::setDestination(const Point2D<float>& nextDestination)
{
	destination = nextDestination;
	Point2D<float> diff = destination - position;
	if (diff.x != 0.0f) {
		deplacementRatio.x = (double)diff.x / (abs(diff.x) + abs(diff.y)) ;
	} else {
		deplacementRatio.x = 0.0f;
	}
	if (diff.y != 0.0f) {
		deplacementRatio.y = (double)diff.y / (abs(diff.x) + abs(diff.y)) ;
	} else {
		deplacementRatio.y = 0.0f;
	}
	distanceToNextPoint = sqrt((diff.x*diff.x) + (diff.y*diff.y));
	directionToNextPoint = diff.angle();
}*/

bool Unit::update(int timeSpent)
{
	float currentSpeed = getSpeed();

	if (path ==NULL) { 
		notifyDeadTarget();
		return true;
	}
	if (life <= 0) {
		return true;
	}

	if (currentSpeed > 0) {

		float distance = (timeSpent*currentSpeed)/1000.0f;

		distanceDone += distance;
		
		while (distanceDone > pathIterator->getDistanceToNext()) {
			distanceDone -= pathIterator->getDistanceToNext();
			pathIterator++;
			if (path->end() == pathIterator) {
				notifyDeadTarget();
				party.escapeUnit(this);
				return true;
			}
		}
		const Point2D<float>& pathPosition = pathIterator->getPosition();
		const Point2D<float>& ratio = pathIterator->getRatio();
		position.x = pathPosition.x + distanceDone * ratio.x;
		position.y = pathPosition.y + distanceDone * ratio.y;
	}

	for (int i=0; i<EFFECT_NUMBER; i++) {
		if (effects[i] != NULL && effects[i]->update(timeSpent,*this)) {
			Factory<EffectTemplate,Effect>::store(effects[i]);
			effects[i] = NULL;
		}
	}

	if (life <= 0) {
		return true;
	}
	return false;

}

void Unit::affect(const EffectTemplate* effectTemplate)
{
	if (effectTemplate==NULL) return;

	Effect* effect = effects[effectTemplate->type];

	if (effect != NULL) {
		if (effect->getTimeLeft() <= 0 || effect->getId() < effectTemplate->id) {
			effects[effectTemplate->type] = Factory<EffectTemplate,Effect>::create(&party, *effectTemplate);
		} else if (effect->getId() == effectTemplate->id) {
			effect->restoreTime();
		}
	} else {
		effects[effectTemplate->type] = Factory<EffectTemplate,Effect>::create(&party, *effectTemplate);
	}
}

void Unit::receiveDamages(float damage)
{
	life -= max(damage - getArmor(),1.0f);
	if (life <= 0.0) {
		//cout << *this << " is dead" << endl;
		notifyDeadTarget();
		party.killUnit(this);
	}
	return;
}

void Unit::hitted(Ammo* ammo) {

	float distance = position.distance(ammo->getPosition());
	float damage = ammo->getDamage();
	float radius = ammo->getRadius();
	if (distance > 1.0f) {
		damage *= distance / radius;
	}

	const EffectTemplate* effectTemplate = ammo->getEffectTemplate();
	if (effectTemplate != NULL && (ammo->getTarget() == this || distance <= effectTemplate->radius)) {
		affect(effectTemplate);
	}
	receiveDamages(damage);
}

/*
void Unit::setPath(Path* path)
{
	if (this->path != NULL) {
		this->path->removeWalker(this);
	}
	if (path != NULL) {
		this->path = path;
		pathIterator = this->path->begin();
		path->addWalker(this);
		position = pathIterator->getPosition();
		//cout << "[Unit] next point " << pathIterator->getPosition() << " - Angle : " << pathIterator->getAngleToNext() << endl;
	}
}
*/

bool Unit::isTargetable(const Point2D<float>& position, float maxDistance, bool detectInvisible, float& realSquaredDistance) const
{
	if (unitTemplate.invisible && !detectInvisible) {
		return false;
	}
	realSquaredDistance = this->position.squaredDistance(position);

	const Effect* light = effects[EFFECT_LIGHT];
	if (light != NULL) {
		maxDistance += light->getValue();
	}

	if (realSquaredDistance <= maxDistance*maxDistance) {
		return true;
	}
	return false;
}


bool Unit::notifyRoadIsChanging() {
	path = NULL;
	return true;
}

float Unit::getSpeed()
{
	const Effect* earth = effects[EFFECT_EARTH];
	if (earth != NULL) {
		return 0;
	}

	const Effect* ice = effects[EFFECT_ICE];
	if (ice != NULL) {
		return unitTemplate.speed - (unitTemplate.speed / 100 * ice->getValue());
	}
	return unitTemplate.speed;
}

float Unit::getArmor()
{
	const Effect* poison = effects[EFFECT_POISON];
	if (poison != NULL) {
		return max(unitTemplate.armor - poison->getValue(),0.0f);
	}
	return unitTemplate.armor;
}