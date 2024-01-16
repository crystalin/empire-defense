#ifndef __UNIT_H__
#define __UNIT_H__

#include "targetable.h"
#include "effect.h"
#include "Path.h"

#define UNIT_DEFAULT_NAME "unit"
#define UNIT_DEFAULT_LEVEL 1
#define UNIT_DEFAULT_MAX_LIFE 100.0f
#define UNIT_DEFAULT_SPEED 1.4f
#define UNIT_DEFAULT_ARMOR 0.0f
#define UNIT_DEFAULT_INVISIBLE false

class Map;

class EDMODEL_API UnitTemplate {
public :
	UnitTemplate(int id, string name, int level, float maxLife, float speed, float armor, bool invisible);

	int id;
	string name;
	int level;
	float maxLife;
	float speed;
	float armor;
	bool invisible;
};

class EDMODEL_API Unit : public Targetable, public Walker
{
public:
	

	Unit(Party* party, const UnitTemplate& unitTemplate, Path* path);
	~Unit();

	int					getType() const { return TYPE_UNIT; }
	const string&		getName() { return unitTemplate.name ; }
	int					getLevel() { return unitTemplate.level ; }
	float				getMaxLife() { return unitTemplate.maxLife ; }
	bool				isInvisible() { return unitTemplate.invisible ; }
	int					getId() { return unitTemplate.id; };

	void				setPath(Path* path);
	const PathPoint&	getPathPoint() { return *pathIterator; }
	float				getDirection() { return pathIterator->getAngleToNext(); }
	bool				update(int timeSpent);

	float				getLifePourcent() { return life/unitTemplate.maxLife; };
	float				getLife() { return life ; }
	bool				isDead() { return life <= 0.0f; };

	Effect**			getEffects() { return effects; };

	void				receiveDamages(float damage); // TODO Add Armor perceing and other effects
	void				hitted(Ammo* ammo);
	bool				isTargetable(const Point2D<float>& position, float maxDistance, bool detectInvisible, float& realSquaredDistance) const;

	void				affect(const EffectTemplate* effectTemplate);
	void				removeEffect();

	float				getSpeed();
	float				getArmor();

	
	virtual bool		notifyRoadIsChanging();

	virtual ostream& print(ostream& os) const { return os << "[Unit " << id <<" at " << position << "]";}

	const int id;

protected:

	const UnitTemplate& unitTemplate;

	float life;

	Effect* effects[EFFECT_NUMBER];

	Path* path;
	vector<PathPoint>::iterator pathIterator;
	float distanceDone;
};


#endif