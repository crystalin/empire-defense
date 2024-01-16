#ifndef __TARGETER__
#define __TARGETER__

#include "Object.h"
class Targetable;

class EDMODEL_API Targeter : public Object
{
public:
	Targeter(Party* party, const Point2D<float>& position = Point2D<float>(0,0)) : Object(party, position) {}
	virtual ~Targeter() {}

	virtual void notifyDeadTarget(Targetable* target) = 0; // return if the targeter is removed from the list
	virtual ostream& print(ostream& os) const { return os << "[Targeter]";}
protected:
};

inline ostream& operator << (ostream& os, const Targeter& targeter)
{
	return targeter.print(os);
}

#endif
