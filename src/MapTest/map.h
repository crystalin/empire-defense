#ifndef __MAP_H__
#define __MAP_H__

#include "point.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/assume_abstract.hpp>



namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, Point2D<float> & point, const unsigned int version)
{
    ar & point.x;
    ar & point.y;
}

} // namespace serialization
} // namespace boost



using namespace std;
using namespace boost;

class Tower {
};

class Case
{
public:
	Case() : position(0,0), tower(NULL) {}
	~Case() { if (tower != NULL) {delete tower;} }
	Case(const Point2D<float>& position) : position(position), tower(NULL) {}
	Tower* getTower() const { return tower; }
	void setTower(Tower* tower) { this->tower = tower; }
	const Point2D<float>& getPosition() const { return position; }
	void setPosition(const Point2D<float>& position) { this->position = position; }
	
private:
	Point2D<float> position;
	Tower* tower;
};

inline std::ostream& operator << (std::ostream& os, const Case& tcase) {	os << tcase.getPosition() << (tcase.getTower() == NULL ? "--" : "T-"); return os;}

class Walker {
public:
	virtual bool notifyRoadIsChanging() = 0;
};

class PathPoint {
public :
	PathPoint(const Point2D<float>& point, int index) : 
	   position(point), angleToNext(0.0f), distanceToNext(0.0f), ratio(0,0)
	   {}

	void setNextPoint(const PathPoint& pathPoint) {
		distanceToNext = position.distance(pathPoint.position);
		angleToNext = position.angle(pathPoint.position);
		Point2D<float> coef = pathPoint.position - position;
		cout << position << " coef : " << coef << endl;
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
		cout << position << " distance to the next : " << distanceToNext << endl;
		cout << position << " ratio : " << ratio << endl;
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


class Path : public vector<PathPoint>
{
public:
	Path(const Point2D<float>& point) { push_back(PathPoint(point,0)); }
	void addPoint(const Point2D<float>& point) { 
		if (back().getPosition() == point) {
			return;
		}
		PathPoint pathPoint(point,size());
		back().setNextPoint(pathPoint);
		push_back(pathPoint);
		for_each(list_walkers.begin(), list_walkers.end(), mem_fun(&Walker::notifyRoadIsChanging));
	}

	void removePoint(const Point2D<float>& point) {
		vector<PathPoint>::iterator it =  find(begin(), end(), point);
		if (it != end()) {
			if (it != begin() && it != end()-1) {
				(it-1)->setNextPoint(*(it+1));
			}
			erase(it);     
			list_walkers.erase(remove_if(list_walkers.begin(), list_walkers.end(), mem_fun(&Walker::notifyRoadIsChanging)),list_walkers.end());
		}
	}
 
	void addWalker(Walker* walker) { this->list_walkers.push_back(walker); }
	void removeWalker(Walker* walker) { list_walkers.erase(find(list_walkers.begin(), list_walkers.end(), walker));	}

private:
	vector<Walker*> list_walkers;
	Path() { }
	    
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & boost::serialization::base_object<vector<PathPoint>>(*this);

    }
};

class Map
{
public:

	Map();
	Map(const string& name, int width = 10, int height = 10);
	~Map();

	Path& createRoad(const Point2D<float>& startPoint) {roads.push_back(Path(startPoint)); return roads.back();}
	int getWidth() { return width; }
	int getHeight() { return height; }
	void setHeight(int height);
	void setWidth(int width);

	const Case& getCase(int x, int y) { return table[x][y];	}
	void addTower(int x, int y, Tower* tower) { table[x][y].setTower(tower); }
	void removeTower(int x, int y) { table[x][y].setTower(NULL); }

	bool validate();
	bool save();
	bool load(string filename);
private:
	vector<Path> roads;
	vector<vector<Case>> table;

	int width;
	int height; 

	string name;
	    
	friend class boost::serialization::access;
	template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & roads;
        ar & width;
        ar & height;
        ar & name;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & roads;
        ar & width;
        ar & height;
        ar & name;

		setWidth(width);
		setHeight(height);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()



};

#endif


///// TESTS /////
/*
class PathPoint {
	friend class Path;
private:
	Point2D<float> position;
	float angleToNext;
	float distanceToNext;
	bool last;
	const PathPoint* nextPoint;

	PathPoint(const Point2D<float>& point) : position(point), angleToNext(0.0f), distanceToNext(0.0f), nextPoint(NULL) {}
	PathPoint(const PathPoint& point) {}

public :

	void setNextPoint(const PathPoint* pathPoint) {
		distanceToNext = position.distance(pathPoint->position);
		angleToNext = position.angle(pathPoint->position);
		nextPoint = pathPoint;
	}
	inline const Point2D<float>&	getPosition() const			{ return position; }
	inline float					getAngleToNext() const		{ return angleToNext; }
	inline float					getDistanceToNext() const	{ return distanceToNext; }
	inline bool						isLast() const				{ return nextPoint == NULL; }
	inline const PathPoint*			next() const				{ return nextPoint; }
};
inline bool operator==(const PathPoint& p1, const PathPoint& p2) { return (p1.getPosition()==p2.getPosition()); };

class Path
{
public:
	Path(const Point2D<float>& point) : count(1) { firstPoint = new PathPoint(point); lastPoint = firstPoint; }
	void addPoint(const Point2D<float>& point) { 
		if (lastPoint->getPosition() == point) {
			return;
		}
		count++;
		PathPoint* pathPoint = new PathPoint(point);
		lastPoint->setNextPoint(pathPoint);
	}
	const PathPoint* front() { return firstPoint; }
	const PathPoint* back() { return lastPoint; }
	int size() { return count; }
private:
	PathPoint* firstPoint;
	PathPoint* lastPoint;
	int count;
};




2nd version 

class PathPoint {
private:
	Point2D<float> position;
	float angleToNext;
	float distanceToNext;
	int index;
public :
	PathPoint(const Point2D<float>& point, int index) : 
	   position(point), angleToNext(0.0f), distanceToNext(0.0f), index(index)
	   {}

	void setNextPoint(const PathPoint& pathPoint) {
		distanceToNext = position.distance(pathPoint.position);
		angleToNext = position.angle(pathPoint.position);
	}
	inline const Point2D<float>&	getPosition() const			{ return position; }
	inline float					getAngleToNext() const		{ return angleToNext; }
	inline float					getDistanceToNext() const	{ return distanceToNext; }
	inline int						getIndex() const			{ return index; }
};
inline bool operator==(const PathPoint& p1, const PathPoint& p2) { return (p1.getPosition()==p2.getPosition()); };

class Path
{
public:
	Path(const Point2D<float>& point) { pointList.push_back(PathPoint(point,0)); }
	void addPoint(const Point2D<float>& point) { 
		if (pointList.back().getPosition() == point) {
			return;
		}
		PathPoint pathPoint(point,pointList.size());
		pointList.back().setNextPoint(pathPoint);
		pointList.push_back(pathPoint);
	}
	void clear() { pointList.clear(); }

	vector<PathPoint>::const_iterator begin() { return pointList.begin(); }
	vector<PathPoint>::const_iterator end() { return pointList.end(); }
	int size() { return pointList.size(); }
private:
	vector<PathPoint> pointList;
};
*/