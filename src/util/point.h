#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include <math.h>

#define PI 3.14159265358979323846f
const float DEG2RAD = (float)(PI/180.0f);

template<typename T1 = float> class Point2D;

#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
#define iround(x) (x<0?(int)(ceil((x)-0.5)):(int)(floor((x)+0.5)))

template<typename T1>
class Point2D
{
public:
	Point2D<T1>() : x(0), y(0) {};
	Point2D<T1>(T1 x, T1 y) : x(x), y(y) {};
	Point2D<T1>& operator=(const Point2D<T1>& p1) { x=p1.x; y=p1.y; return *this; };

	template<typename T2>
	Point2D<T1>(const Point2D<T2>& p) : x((T1)p.x), y((T1)p.y) {};

	float angle(const Point2D<T1>& p1) const { return atan2(p1.y - y, p1.x - x) * 180 / PI; }
	float angle() const { return atan2(y, x) * 180 / PI; }
	float distance(const Point2D<T1>& p1) const { return sqrt((p1.x - x)*(p1.x - x) + (p1.y - y)*(p1.y - y)); }
	void normalize(int max) { 
		if (max == 0) {return;}
		x = x / max;
		y = y / max;
		return;
	}
	void absolute() { x = abs(x); y = abs(y); }
	float squaredDistance(const Point2D<T1>& p1) const { return (p1.x - x)*(p1.x - x) + (p1.y - y)*(p1.y - y); }
	
	float distance() const { return sqrt(x*x + y*y); }
	float squaredDistance() const { return x*x + y*y; }
	
	T1 x;
	T1 y;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & x;
		ar & y;
	}

};


template<typename T1>
Point2D<T1> operator+(const Point2D<T1>& p1, const Point2D<T1>& p2) { return Point2D<T1>(p1.x+p2.x,p1.y+p2.y); };
template<typename T1>
Point2D<T1> operator+(const Point2D<T1>& p1, T1 p2) { return Point2D<T1>(p1.x+p2,p1.y+p2); };
template<typename T1>
Point2D<T1> operator-(const Point2D<T1>& p1, const Point2D<T1>& p2) { return Point2D<T1>(p1.x-p2.x,p1.y-p2.y); };
template<typename T1>
Point2D<T1> operator-(const Point2D<T1>& p1, T1 p2) { return Point2D<T1>(p1.x-p2,p1.y-p2); };
template<typename T1>
Point2D<T1> operator*(const Point2D<T1>& p1, T1 val) { return Point2D<T1>(p1.x*val,p1.y*val); };
template<typename T1>
Point2D<T1> operator/(const Point2D<T1>& p1, T1 val) { return Point2D<T1>(p1.x/val,p1.y/val); };
template<typename T1>
bool operator==(const Point2D<T1>& p1, const Point2D<T1>& p2) { return (p1.x==p2.x) && (p1.y==p2.y); };
template<typename T1>
bool operator!=(const Point2D<T1>& p1, const Point2D<T1>& p2) { return (p1.x!=p2.x) || (p1.y!=p2.y); };
template<typename T1>
bool operator&&(const Point2D<T1>& p1, const Point2D<T1>& p2) { return (((p2.x - p1.x)*(p2.x - p1.x)) + ((p2.y - p1.y)*(p2.y - p1.y)))<0.1f; };
template<typename T1>
std::ostream& operator << (std::ostream& os, const Point2D<T1>& p1) {	os << "[" << p1.x << "," << p1.y<< "]"; return os;}

#endif