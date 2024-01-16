#ifndef __CASE_H__
#define __CASE_H__

#include "Targetable.h"

class Tower;

class EDMODEL_API Case : public Targetable
{
public:
	Case(Party* party) : Targetable(party, Point2D<float>(0,0)), tower(NULL), onRoad(false) {}
	Case(Party* party, const Point2D<float>& position) : Targetable(party,position), tower(NULL), onRoad(false) {}
	Tower* getTower() const { return tower; }
	void setTower(Tower* tower) { this->tower = tower; }
	const Point2D<float>& getPosition() const { return position; }
	void setPosition(const Point2D<float>& position) { this->position = position; }
	bool isOnRoad() const { return onRoad; }
	void setOnRoad(bool onRoad) { this->onRoad = onRoad; }

	virtual int getType() const { return TYPE_CASE; }

	virtual void hitted(Ammo* ammo) {}
	virtual bool isTargetable(const Point2D<float>& position, float distance, bool detectInvisible, float& realSquaredDistance) const {
		realSquaredDistance = this->position.squaredDistance(position);
		return realSquaredDistance <= distance*distance;
	}

	virtual ostream& print(ostream& os) const { return os << "[Case " << position.x <<", " << position.y << "]";}

private:
	Tower* tower;
	bool onRoad;
};


class CaseAffector {
private:
	int lineNumber;
	int caseNumber;
public:
	CaseAffector(int lineNumber, int startCase) : lineNumber(lineNumber), caseNumber(startCase) {}
	void operator()(Case& tcase) {
		tcase.setPosition(Point2D<float>((float)lineNumber,(float)(caseNumber++)));
	}
};

class CaseResetor {
public:
	void operator()(Case& tcase) {
		tcase.setTower(NULL);
	}
};

#endif