#ifndef __TARGETABLE__
#define __TARGETABLE__

#include "object.h"
#include "party.h"
#include "targeter.h"

class Ammo;

class EDMODEL_API Targetable abstract : public Object
{
public:
	Targetable(Party* party, const Point2D<float>& position = Point2D<float>(0,0)) : Object(party, position) {list_targeters.reserve(100);}
	virtual ~Targetable() {}

	void notifyDeadTarget() {
		if (this->getType() == TYPE_UNIT) {
			party.notifyDeadUnit((Unit*)this);
		}
		for_each(list_targeters.begin(), list_targeters.end(), bind2nd(mem_fun(&Targeter::notifyDeadTarget),this));
		this->list_targeters.clear();
	}

	virtual void	hitted(Ammo* ammo) = 0;
	virtual bool	isTargetable(const Point2D<float>& position, float maxDistance, bool detectInvisible, float& realDistance) const = 0;
	virtual bool	isDead() { return false; };

	void addTargeter(Targeter* targeter)
	{
		this->list_targeters.push_back(targeter);
	}
 
	void removeTargeter(Targeter* targeter)
	{
		list_targeters.erase(find(list_targeters.begin(), list_targeters.end(), targeter)); // don't put list...end());
	}
	virtual ostream& print(ostream& os) const { return os << "[Targetable]";}

private:
	vector<Targeter*> list_targeters;
};


#endif
