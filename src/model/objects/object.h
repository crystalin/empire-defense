#ifndef __OBJECT_H__
#define __OBJECT_H__

#define TYPE_OBJECT 0
#define TYPE_CASE 1
#define TYPE_TOWER 2
#define TYPE_UNIT 3
#define TYPE_AMMO 4
#define TYPE_AMMO_LASER 5
#define TYPE_AMMO_MISSILE 6
#define TYPE_AMMO_PROJECTILE 7
#define TYPE_AMMO_ARROW 8

class Party;

class EDMODEL_API Object
{
public:
	Object(Party* party, const Point2D<float>& position = Point2D<float>(0,0)) : position(position), party(*party) {}
	virtual ~Object() {}

	virtual ostream& print(ostream& os) const { return os << "[Object]";}

	const Point2D<float>& getPosition() const { return position; }
	void setPosition(const Point2D<float>& position) { this->position = position; }

	virtual int getType() const {return TYPE_OBJECT; }

protected:
	Point2D<float> position;
	Party& party;
};

inline ostream& operator << (ostream& os, const Object& object)
{
	return object.print(os);
}


#endif