#ifndef __PATH_POINT_H__
#define __PATH_POINT_H__

#include "point.h"

class PathPoint {
public :
	PathPoint(const Point2D<float>& point, int index) : 
	   position(point), angleToNext(0.0f), distanceToNext(0.0f), ratio(0,0)
	   {}

	void setNextPoint(const PathPoint& pathPoint) {
		distanceToNext = position.distance(pathPoint.position);
		angleToNext = position.angle(pathPoint.position);
		Point2D<float> coef = pathPoint.position - position;
		if (coef.x == 0.0f) {
			ratio.x = 0.0f;
			ratio.y = coef.y < 0 ? -1.0f : 1.0f;
		} else if (coef.y == 0.0f) {
			ratio.x = coef.x < 0 ? -1.0f : 1.0f;
			ratio.y = 0.0f;
		} else {
			ratio.x = coef.x / distanceToNext;
			ratio.y = coef.y / distanceToNext;
		}
		//cout << position << " distance to the next : " << distanceToNext << endl;
	}
	inline const Point2D<float>&	getPosition() const			{ return position; }
	inline const Point2D<float>&	getRatio() const			{ return ratio; }
	inline float					getAngleToNext() const		{ return angleToNext; }
	inline float					getDistanceToNext() const	{ return distanceToNext; }

private:
	Point2D<float> position;
	float angleToNext;
	float distanceToNext;
	Point2D<float> ratio;
	    
	PathPoint(){}

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & position;
		ar & angleToNext;
		ar & distanceToNext;
        ar & ratio;
	}

};
inline bool operator==(const PathPoint& p1, const PathPoint& p2) { return (p1.getPosition()==p2.getPosition()); };
inline bool operator==(const PathPoint& p1, const Point2D<float>& p2) { return (p1.getPosition()==p2); };

#endif