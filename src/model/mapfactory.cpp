#include "model_common.h"
#include "mapfactory.h"
#include "map.h"

string Factory<MapTemplate, Map>::factoryName = "Map Factory";

void Factory<MapTemplate, Map>::init(ConfigReader& configReader)
{
	int id = 0;
	boost::filesystem::path path = "../ressources/maps";
	if (!boost::filesystem::exists(path)) {
		return;
	}
	list <MapTemplate*> templateList;

	for(boost::filesystem::directory_iterator it(path); it != boost::filesystem::directory_iterator(); ++it) {
		if (boost::filesystem::extension(*it) == ".map") {
			MapTemplate* mapTemplate = new MapTemplate();
			mapTemplate->load(it->string());
			mapTemplate->id = id++;
			templateList.push_back(mapTemplate);
		}
	}
	cout << id << " maps found" << endl;

	storeTemplates(templateList);

}