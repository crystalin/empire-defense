#ifndef __PATH_H__
#define __PATH_H__

#include "Walker.h"
#include "PathPoint.h"

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


class RoadValidator {
private:
	float width;
	float height;
	bool valid;
public:
	RoadValidator(float width, float height) : width(width), height(height), valid(true) {}

	void operator()(const Path& path) {
		valid &= for_each(path.begin(), path.end(), RoadValidator(*this)).valid;
	}
	void operator()(const PathPoint& pathPoint) {
		const Point2D<float>& position = pathPoint.getPosition();
		if (position.x < 0.0f || position.x >= width ||
			position.y < 0.0f || position.y >= height) {
				valid=false;
		}
	}

	bool isValid() { return valid; }
};

#endif