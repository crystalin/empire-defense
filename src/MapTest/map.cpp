#include "map.h"
#include <fstream>
#include <sstream> 
#include <iostream>


class CaseAffector {
private:
	int lineNumber;
	int caseNumber;
public:
	CaseAffector(int lineNumber, int startCase) : lineNumber(lineNumber), caseNumber(startCase) {}
	void operator()(Case& tcase) {
		tcase.setPosition(Point2D<float>((float)(caseNumber++),(float)lineNumber));
	}
};

class ResizeHeight {
private:
	int newSize;
	int lineNumber;
public:
	ResizeHeight(int newSize, int startLine) : newSize(newSize), lineNumber(startLine) {}
	void operator()(vector<Case>& line) {
		int oldSize = line.size();
		line.resize(newSize);
		for_each(line.begin(), line.end(), CaseAffector(lineNumber,0));
		++lineNumber;
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
				cout << "position " << position << " incorrect" << endl;
				valid=false;
		}
	}

	bool isValid() { return valid; }
};

Map::Map() : name(), width(0), height(0)
{
}

Map::Map(const string& name, int width, int height ) : name(name), width(width), height(height)
{
	setWidth(width);
	setHeight(height);
}

Map::~Map() 
{
}

bool Map::save(){
	if (!validate()) {
		cout << "Map incorrect, cannot be saved" << endl;
		return false;
	}

	try {
		std::ofstream ofs((name+ ".map").c_str());
		boost::archive::text_oarchive oa(ofs);
		oa << ((const Map&)(*this));
		ofs.close();
	} catch (...) {
		return false;
	}
	return true;
}

bool Map::validate() {
	return for_each(roads.begin(), roads.end(), RoadValidator(width+1.0f,height+1.0f)).isValid();
}

bool Map::load(string filename){
	try {
		std::ifstream ifs(filename.c_str());
		boost::archive::text_iarchive ia(ifs);
		// read class state from archive
		ia >> (*this);
	} catch (...) {
		return false;
	}
	return true;
}

void Map::setWidth(int width) {
	if (width < 2 || width > 1000) {
		return ;
	}
	this->width = width;
	int oldSize = table.size();
	table.resize(width);
	for_each(table.begin(), table.end(), ResizeHeight(height,0));
}

void Map::setHeight(int height) {
	if (height < 2 || height > 1000) {
		return ;
	}
	this->height = height;
	for_each(table.begin(), table.end(), ResizeHeight(height,0));
}