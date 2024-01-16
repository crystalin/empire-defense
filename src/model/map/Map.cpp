#include "model_common.h"
#include "Map.h"
#include "Path.h"

bool MapTemplate::save(){
	if (!validate()) {
		cout << "Map incorrect, cannot be saved" << endl;
		return false;
	}

	try {
		std::ofstream ofs((name+ ".map").c_str());
		boost::archive::text_oarchive oa(ofs);
		oa << ((const MapTemplate&)(*this));
		ofs.close();
	} catch (...) {
		return false;
	}
	return true;
}

bool MapTemplate::load(string filename){
	try {
		std::ifstream ifs(filename.c_str());
		if (ifs.is_open()) {
			cout << "loading the map " << filename << "... " ;
			boost::archive::text_iarchive ia(ifs);
			ia >> (*this);
			cout << "done [" <<width <<"x" << height <<"]" << endl;
		} else {
			cout << " map" << filename << " not found" << endl;
		}
	} catch (...) {
		cout << "impossible to laod the map" << filename << endl;
		return false;
	}
	return true;
}

bool MapTemplate::validate() {
	return for_each(roads.begin(), roads.end(), RoadValidator(width+1.0f,height+1.0f)).isValid();
}


Path& MapTemplate::createRoad(const Point2D<float>& startPoint) {
	roads.push_back(Path(startPoint));
	return roads.back();
}

class CaseRoadBlocker {
	Map& map;
public:
	CaseRoadBlocker(Map& map) : map(map) {}

	void operator()(const Path& path) {
		for_each(path.begin(), path.end(), CaseRoadBlocker(map));
	}

	void operator()(const PathPoint& pathPoint) {
		const Point2D<float>& position = pathPoint.getPosition();
					
		Point2D<float> ratio = pathPoint.getRatio();
		//ratio.normalize(pathPoint.getDistanceToNext());
		Point2D<int> coef = (pathPoint.getRatio()*pathPoint.getDistanceToNext()*1.01f);
		int signX = coef.x > 0 ? 1 : -1;
		int signY = coef.y > 0 ? 1 : -1;
		coef.absolute();
		//cout << "Path Point " << position << " -ratio- " << ratio << " -coef- " << coef << endl;
		for (int x = 0; x <= coef.x ; x++) {
			for (int y = 0; y <= coef.y ; y++) {

				int caseX = signX * x;
				int caseY = signY * y;

				float distance = abs((caseX * ratio.y - caseY * ratio.x));
				if (distance<= 0.75f) {
					//cout << "[" << (caseX + position.x) << ", " << (caseY + position.y) <<" ] distance : " << distance << endl;
					map.table[caseX + (int)position.x][caseY + (int)position.y].setOnRoad(true);
				}
			}
		}
	}
};

class LineInitialisator {
private:
	int lineNumber;

public:
	LineInitialisator () : lineNumber(0) {}
	void operator()(vector<Case>& line) {
		for_each(line.begin(), line.end(), CaseAffector(lineNumber,0));
		lineNumber++;
	}
};

class LineResetor {
public:
	void operator()(vector<Case>& line) {
		for_each(line.begin(), line.end(), CaseResetor());
	}
};

Map::Map(Party* party, const MapTemplate& mapTemplate) : party(party), mapTemplate(mapTemplate), table(mapTemplate.height, vector<Case>(mapTemplate.width,Case(party))){
	for_each(table.begin(), table.end(), LineInitialisator());
	blockRoadCases();
}

void Map::reset() {
	for_each(table.begin(), table.end(), LineResetor());
}

void Map::blockRoadCases() {
	const vector<Path>& roads = mapTemplate.getRoads();
	for_each(roads.begin(), roads.end(), CaseRoadBlocker(*this));
}