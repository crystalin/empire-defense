#ifndef __TOWER_H__
#define __TOWER_H__

#include "targeter.h"
#include "player.h"
#include "effect.h"

#define TOWER_DEFAULT_NAME "tower"
#define TOWER_DEFAULT_SPEED 1200
#define TOWER_DEFAULT_VISIBILITY 4.0f
#define TOWER_DEFAULT_AMMO_ID 0
#define TOWER_DEFAULT_COST 5
#define TOWER_DEFAULT_BUILDING_TIME 3000

class AmmoTemplate;
class Unit;
class Case;

class EDMODEL_API TowerTemplate {
public:
	TowerTemplate(int id, string name, int parentId, int speed, float visibility, const AmmoTemplate& ammoTemplate, int cost, int buildingTime);
	
	string name;
	int id;
	int parentId;
	const AmmoTemplate& ammoTemplate;
	int speed;
	int cost;
	int buildingTime;
	float visibility;
	int level;

	vector<const TowerTemplate*> subTemplates;
};

class EDMODEL_API Tower : public Targeter, boost::noncopyable
{
public:

	Tower(Party* party, const TowerTemplate& towerTemplate, const Case& tcase, const Player* player);
	~Tower();

	int						getType() const				{ return TYPE_TOWER; }
	float					getVisibility()	 const		{ return towerTemplate.visibility; }
	int						getId()	 const				{ return towerTemplate.id; }
	int						getParentId() const			{ return towerTemplate.parentId; }
	int						getSpeed() const			{ return towerTemplate.speed; }
	int						getCost() const				{ return towerTemplate.cost; }
	int						getLevel() const			{ return towerTemplate.level; }
	int						getBuildingTime() const		{ return towerTemplate.buildingTime; }
	int						getBuildingTimeLeft() const	{ return buildingTimeLeft; }
	float					getLoadingPourcent() const	{ return (towerTemplate.speed-max(0, timeLeft)) / (float)towerTemplate.speed; }
	const string&			getName() const				{ return towerTemplate.name; }
	const EffectTemplate*	getEffectTemplate() const	{ return effectTemplate; }
	const AmmoTemplate&		getAmmoTemplate() const		{ return towerTemplate.ammoTemplate; }
	const Case&				getCase() const				{ return tcase; }
	const Player*			getOwner() const			{ return owner; }
	Targetable*				getTarget()	 const			{ return target; }
	const vector<const TowerTemplate*>& getSubTemplates() const { return towerTemplate.subTemplates; }

	//void					setOwner(const Player* player) { owner = player; }
	void					setTarget(Targetable* newTarget);
	void					setEffectTemplate(const EffectTemplate* effectTemplate) {this->effectTemplate = effectTemplate; }

	void					notifyDeadTarget(Targetable* target);

	void					update(int timeSpent);
	void					fire();

	virtual ostream& print(ostream& os) const { return os << "[Tower " << id <<", " << towerTemplate.name <<", ]";}

	const int id;

protected:

	static bool closestTargetable(Unit* unit1, Unit* unit2);

	const TowerTemplate&	towerTemplate;
	const EffectTemplate*	effectTemplate;
	const AmmoTemplate*		ammoTemplate;

	Targetable*				target;
	const Case&				tcase;
	const Player*			owner;
	int						timeLeft;
	int						buildingTimeLeft;

	int						waitingTime;

};

#endif