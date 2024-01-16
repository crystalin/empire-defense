#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "point.h"

template<typename T1 = float> class Rectangle;

template<typename T1>
class Rectangle
{
public:
	Rectangle<T1>() : x1(0), x2(0), y1(0), y2(0) {};
	Rectangle<T1>(T1 x1, T1 y1, T1 x2, T1 y2) : x1(x1), x2(x2), y1(y1), y2(y2) {};
	Rectangle<T1>& operator=(const Rectangle<T1>& p1) { x1=p1.x1; x2=p1.x2;  y1=p1.y1; y2=p1.y2; return *this; };
	Point2D<T1> centerize() { 
		T1 centerX = (x1 + x2) / (T1)2;
		T1 centerY = (y1 + y2) / (T1)2;
		x1 -= centerX;
		x2 -= centerX;
		y1 -= centerY;
		y2 -= centerY;
		return Point2D<T1>(centerX, centerY);
	};

	template<typename T2>
	operator const Rectangle<T2> () { return Rectangle<T2>((T2)x1, (T2)y1, (T2)x2, (T2)y2); };

	template<typename T2>
	bool contains(const Point2D<T2>& p1) { return p1.x >= x1 && p1.x <= x2 && p1.y >= y1 && p1.y <= y2; }

	T1 x1, x2;
	T1 y1, y2;

	T1 getSizeX() const { return x2 - x1; }
	T1 getSizeY() const { return y2 - y1; }

	Point2D<T1> getSize() const { return Point2D<T1>(x2-x1,y2-y1); }
	Point2D<T1> getOrigin() const { return Point2D<T1>(x1,y1); }
	Point2D<T1> getDestination() const { return Point2D<T1>(x2,y2); }

};

template<typename T1>
bool operator==(const Rectangle<T1>& p1, const Rectangle<T1>& p2) { return (p1.x1==p2.x1) && (p1.x2==p2.x2) && (p1.y1==p2.y1) && (p1.y2==p2.y2); }
template<typename T1>
Rectangle<T1> operator+(const Rectangle<T1>& p1, const Rectangle<T1>& p2) { return Rectangle<T1>(p1.x1+p2.x1,p1.y1+p2.y1,p1.x2+p2.x2,p1.y2+p2.y2); };
template<typename T1>
Rectangle<T1> operator*(const Rectangle<T1>& p1, T1 p2) { return Rectangle<T1>(p1.x1*p2,p1.y1*p2,p1.x2*p2,p1.y2*p2); };
template<typename T1>
Rectangle<T1> operator+(const Rectangle<T1>& p1, const Point2D<T1>& p2) { return Rectangle<T1>(p1.x1+p2.x,p1.y1+p2.y,p1.x2+p2.x,p1.y2+p2.y); };
template<typename T1>
Rectangle<T1> operator-(const Rectangle<T1>& p1, T1 p2) { return Rectangle<T1>(p1.x1-p2,p1.y1-p2,p1.x2-p2,p1.y2-p2); };
template<typename T1>
std::ostream& operator << (std::ostream& os, const Rectangle<T1>& r1) {	os << "[" << r1.x1 << "-" << r1.x2<< "," << r1.y1 << "-" << r1.y2<< " ]"; return os;}

#endif