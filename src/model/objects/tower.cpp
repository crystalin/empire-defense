#include "model_common.h"
#include "tower.h"
#include "ammo.h"
#include "case.h"
#include "unit.h"
#include "party.h"
#include "factory.h"

static int idStatic = 0;

TowerTemplate::TowerTemplate(int id, string name, int parentId, int speed, float visibility, const AmmoTemplate& ammoTemplate, int cost, int buildingTime) :
id(id), name(name), parentId(parentId), speed(speed), visibility(visibility), ammoTemplate(ammoTemplate), cost(cost), buildingTime(buildingTime), level(0)
{
	//subTemplates.reserve(20);
}

Tower::Tower(Party* party, const TowerTemplate& towerTemplate, const Case& tcase, const Player* player) : 
Targeter(party, tcase.getPosition()), towerTemplate(towerTemplate), timeLeft(towerTemplate.speed), waitingTime(0), 
effectTemplate(NULL), target(NULL), tcase(tcase), buildingTimeLeft(towerTemplate.buildingTime),
owner(player), id(idStatic++) {

}

Tower::~Tower() {
	if (target!=NULL) {
		target->removeTargeter(this);
	}
}

void Tower::update(int timeSpent)
{
	if (buildingTimeLeft>0) {
		buildingTimeLeft -= timeSpent;
		return;
	}

	float bestDistance;
	timeLeft -= timeSpent;
	if (timeLeft<=0) {
		if (waitingTime <= 0) {
			LOG_FILE(" Tower Target Changed " <<endl);
			float currentVisibility = getVisibility();
			if (target==NULL || !target->isTargetable(position,currentVisibility,false,bestDistance)) {
				Unit* bestUnit=NULL;
				bestDistance = 1000000.0f;
				const vector<Unit*>& units = party.getUnits();
				for (vector<Unit*>::const_iterator ci = units.begin(); ci != units.end(); ++ci) {
					Unit* unit = *ci;
					float SquredDistanceToUnit;
					if (unit->isTargetable(position,currentVisibility,false,SquredDistanceToUnit)) {
						if (SquredDistanceToUnit<=bestDistance) {
							bestUnit = unit;
							bestDistance = SquredDistanceToUnit;
						}
					}
				}
				if (bestUnit != target) {
					setTarget(bestUnit);
				}
			}
			if (target == NULL) {
				waitingTime = 100; // Arbitrary time to prevent cpu overload
				timeLeft = 0;
			} else {
				waitingTime = 0;
				fire();
			}
		} else {
			waitingTime -= timeSpent;
		}
	}
}

void Tower::fire()
{
	if (timeLeft<=0) {
		timeLeft += getSpeed();
		
		Ammo* ammo  = Factory<AmmoTemplate, Ammo>::create<const Tower&>(&party, towerTemplate.ammoTemplate, *this );
		//cout << "Fire Ammo [ " << ammo->id << "]" << *ammo << endl;
		party.fireAmmo(ammo);
	}
}

void Tower::notifyDeadTarget(Targetable* target)
{
	this->target = NULL;
}

void Tower::setTarget (Targetable* newTarget)
{
	if (target!=NULL) {
		target->removeTargeter(this);
	}
	target = newTarget;
	if (target!=NULL) {
		target->addTargeter(this);
	}
}