#ifndef __AMMO_H__
#define __AMMO_H__

#include "unit.h"
#include "targeter.h"

#define AMMO_DEFAULT_NAME "ammo"
#define AMMO_DEFAULT_TYPE TYPE_AMMO_LASER
#define AMMO_DEFAULT_DAMAGE 5
#define AMMO_DEFAULT_RADIUS 0.0f
#define AMMO_DEFAULT_SPEED 3
#define AMMO_DEFAULT_TIME 5000

class Tower;
class EffectTemplate;

class EDMODEL_API AmmoTemplate {
public:
	//AmmoTemplate(int id, string name, int parentId, int time, int type, int damage, float radius, float speed);
	AmmoTemplate(int id, string name, int time, int type, float damage, float radius, float speed);

	string name;
	int id;
	//int parentId;
	int type;
	float damage;
	float radius;
	float speed;
	int time;
};

class Ammo : public Targeter, boost::noncopyable
{
public:

	Ammo(Party* party, const AmmoTemplate& ammoTemplate, const Tower& tower);
	~Ammo();

	virtual int				getType() const				{ return ammoTemplate.type; }
	int						getId()	const				{ return ammoTemplate.id; }
	//int						getParentId()		{ return ammoTemplate.parentId; }
	virtual float			getDamage()	const			{ return ammoTemplate.damage; }
	virtual float			getRadius()	const			{ return ammoTemplate.radius; }
	virtual float			getSpeed() const			{ return ammoTemplate.speed; }
	virtual int				getTime() const				{ return ammoTemplate.time; }

	virtual bool			update(int timed);
	Targetable*				getTarget()					{ return target; }
	const Point2D<float>&	getDestination() const		{ return destination; }
	const Point2D<float>&	getOrigin()	const			{ return origin; }
	float					getHeight()	const			{ return height; }
	const EffectTemplate* 	getEffectTemplate()	const	{ return effectTemplate; }
	float					getDirection() const		{ return position.angle(destination); }

	void					notifyDeadTarget(Targetable* targetable);

	//void					setTarget(Targetable* newTarget);

	const int id;

	virtual ostream& print(ostream& os) const { return os << "[Ammo " << id << " from " << origin <<" to " << destination <<" ]";}

protected:

	void hit();
	void hitAround(Targetable* targetable);

	Targetable* target;
	const AmmoTemplate& ammoTemplate;
	const EffectTemplate* effectTemplate;

	int timeLeft;
	Point2D<float> origin;
	Point2D<float> destination;
	float height;



};

#endif
