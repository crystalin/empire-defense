// MapTest.cpp : Defines the entry point for the console application.
//

#include "map.h"
#include <iostream>
#include <sstream> 

class Unit : public Walker {
public:
	Unit() : path(NULL), arrived(false), distance(0.0f) {}
	void setPath(Path* path) { 
		if (this->path != NULL) {
			this->path->removeWalker(this);
		}
		this->path = path;
		pathIterator = this->path->begin();

		path->addWalker(this);
		arrived = false;
	}

	void update(int timeSpent) {
		if (path == NULL || arrived) {
			return;
		}
		distance += timeSpent;
		while (distance > pathIterator->getDistanceToNext()) {
			distance -= pathIterator->getDistanceToNext();
			pathIterator++;
			if (path->end() == pathIterator) {
				arrived = true;
				cout << "Unit arrived" << endl;
				return;
			}
		}
		cout << "Unit [" << pathIterator->getPosition() << "] : " << distance << " / " << pathIterator->getDistanceToNext() << endl;
	}

	bool notifyRoadIsChanging() {
		path = NULL;
		arrived = false;
		return true;
	}

	bool isArrived() { return arrived; }

private:
	Path* path;
	vector<PathPoint>::iterator pathIterator;

	bool arrived;
	float distance;
};

int main(int argc, char* argv[])
{
	Map* map = new Map("long-road",22,22);

	Path& path = map->createRoad(Point2D<float>(4,3));
	path.addPoint(Point2D<float>(15,3));
	path.addPoint(Point2D<float>(15,5));
	path.addPoint(Point2D<float>(11,5));
	path.addPoint(Point2D<float>(11,1));
	path.addPoint(Point2D<float>(7,1));
	path.addPoint(Point2D<float>(7,5));
	path.addPoint(Point2D<float>(9,5));
	path.addPoint(Point2D<float>(9,8));
	path.addPoint(Point2D<float>(13,8));
	path.addPoint(Point2D<float>(13,1));
	path.addPoint(Point2D<float>(17,1));
	path.addPoint(Point2D<float>(17,10));
	path.addPoint(Point2D<float>(20,10));
	path.addPoint(Point2D<float>(20,7));
	path.addPoint(Point2D<float>(15,7));
	path.addPoint(Point2D<float>(15,10));
	path.addPoint(Point2D<float>(6,10));
	path.addPoint(Point2D<float>(6,7));
	path.addPoint(Point2D<float>(3,7));
	path.addPoint(Point2D<float>(3,12));
	path.addPoint(Point2D<float>(8,12));
	path.addPoint(Point2D<float>(8,9));
	path.addPoint(Point2D<float>(1,9));
	path.addPoint(Point2D<float>(1,14));
	path.addPoint(Point2D<float>(11,14));
	path.addPoint(Point2D<float>(11,12));
	path.addPoint(Point2D<float>(14,12));
	path.addPoint(Point2D<float>(14,14));
	path.addPoint(Point2D<float>(18,14));
	path.addPoint(Point2D<float>(18,12));
	path.addPoint(Point2D<float>(21,12));
	path.addPoint(Point2D<float>(21,17));
	path.addPoint(Point2D<float>(1,17));
	path.addPoint(Point2D<float>(1,19));
	path.addPoint(Point2D<float>(19,19));
	path.addPoint(Point2D<float>(19,15));
	path.addPoint(Point2D<float>(3,15));
	path.addPoint(Point2D<float>(3,21));
	path.addPoint(Point2D<float>(12,21));
	path.addPoint(Point2D<float>(12,16));
	path.addPoint(Point2D<float>(16,16));
	path.addPoint(Point2D<float>(16,21));
	cout << "Path Size : " << path.size() << endl;

	vector<PathPoint>::const_iterator pointIterator = path.begin();
	while (pointIterator != path.end()) {
		cout << "pathPoint : " << (*pointIterator).getPosition() << endl;
		pointIterator++;
	}

	cout << " Parsing the map cases " << endl;
	for (int i = 0; i<map->getWidth(); i++) {
		for (int j = 0; j<map->getHeight(); j++) {
			cout << map->getCase(i,j);
		}
		cout << endl;
	}

	cout << " saving to \"map.map\"" << endl;
	map->save();
	    
	cout << " loading \"map.map\"" << endl;
	Map loadedMap;
	loadedMap.load("map.map");

    // archive and stream closed when destructors are called

	cout << " Parsing the loaded map cases " << endl;
	for (int i = 0; i<loadedMap.getWidth(); i++) {
		for (int j = 0; j<loadedMap.getHeight(); j++) {
			cout << loadedMap.getCase(i,j);
		}
		cout << endl;
	}

	delete map;
	return 0;
}

