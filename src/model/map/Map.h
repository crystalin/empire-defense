#ifndef __MAP_H__
#define __MAP_H__

#include "Case.h"
#include "Path.h"

class Tower;

class EDMODEL_API MapTemplate
{
public:

	MapTemplate() : width(0), height(0), id(0), name("new map template") {}

	int width;
	int height;
	int id;

	string name;

	int getRoadCount() const					{ return roads.size(); }
	const Path& getRoad(int roadNumber) const	{ return roads[roadNumber]; }
	const vector<Path>& getRoads() const		{ return roads; }

	Path& createRoad(const Point2D<float>& startPoint);

	bool save();
	bool load(string filename);

private:
	vector<Path> roads;

	bool validate();

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
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

	friend class boost::serialization::access;

};

class EDMODEL_API Map {
public:
	Map(Party* party, const MapTemplate& mapTemplate);

	const Case& getCase(int x, int y) const { return table[x][y];	}
	void addTower(int x, int y, Tower* tower) { table[x][y].setTower(tower); }
	void removeTower(int x, int y) { table[x][y].setTower(NULL); }
		
	int getRoadCount() const					{ return mapTemplate.getRoadCount(); }
	const Path& getRoad(int roadNumber) const	{ return mapTemplate.getRoad(roadNumber); }
	const vector<Path>& getRoads() const		{ return mapTemplate.getRoads(); }
	int getWidth() const						{ return mapTemplate.width; }
	int getHeight() const						{ return mapTemplate.height; }
	const string& getName() const				{ return mapTemplate.name; }

	void blockRoadCases();
	void reset();

private:
	Party* party;
	const MapTemplate& mapTemplate;
	vector<vector<Case>> table;

	friend class CaseRoadBlocker;
};

#endif