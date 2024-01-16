#include "model_common.h"
#include "towerfactory.h"
#include "ammo.h"

string Factory<TowerTemplate, Tower>::factoryName = "Tower Factory";

void Factory<TowerTemplate, Tower>::init(ConfigReader& configReader)
{
	list <TowerTemplate*> templateList;
	istringstream iss;
	for (map<string,map<string,string>>::iterator ci = configReader.begin(); ci != configReader.end(); ++ci) {
		string name = ci->first;
		int id = 0;
		int parentId = 0;
		int ammoId = TOWER_DEFAULT_AMMO_ID;
		int cost = TOWER_DEFAULT_COST;
		int speed = TOWER_DEFAULT_SPEED;
		float visibility = TOWER_DEFAULT_VISIBILITY;
		int buildingTime = TOWER_DEFAULT_BUILDING_TIME;
		char shortcut = 0;

		map<string, string>& config = ci->second;


		configReader.read(config,"id",id);
		configReader.read(config,"parent",parentId);
		configReader.read(config,"speed",speed);
		configReader.read(config,"cost",cost);
		configReader.read(config,"visibility",visibility);
		configReader.read(config,"ammo",ammoId);
		configReader.read(config,"buildingTime",buildingTime);
		configReader.read(config,"shortcut",shortcut);

		if (id ==0 ) {
			cerr << "[TowerFactory] Missing ID for tower \"" << name << "\"" << endl;
		} else {
			if (find_if(templateList.begin(), templateList.end(), bind(&TowerTemplate::id,_1)==id) != templateList.end()) {
				cerr << "[TowerFactory] tower " << id << "already defined !!" << endl;
				break;
			}
			const AmmoTemplate* ammoTemplate = Factory<AmmoTemplate, Ammo>::getTemplate(ammoId);
			if (ammoTemplate != NULL) {
				cout << "[TowerFactory] " << shortcut << " as Shortcut for \"" << name <<  "\"" << endl;
				if (shortcut != 0) {
					/*if (parentId == 0) {
						KeyBinder::getInstance()->bindKey(shortcut, id + TOWER_COMMAND);
					} else {
						KeyBinder::getInstance()->bindKey(parentId, shortcut, id + TOWER_COMMAND);
					}*/
				}
				templateList.push_back(new TowerTemplate(id, name, parentId, speed, visibility, *ammoTemplate, cost, buildingTime));
			} else {
				cout << "[TowerFactory] Ammo " << ammoId << " is not defined for tower \"" << name <<  "\"" << endl;
			}
		}
	}
	templateList.sort(compare);
	storeTemplates(templateList);

	for (list<TowerTemplate*>::iterator it = templateList.begin(); it != templateList.end(); ++it){
		TowerTemplate* towerTemplate = *it;

		int parentId = towerTemplate->parentId;
		if (parentId == 0) {
			towerTemplate->level = 0;
			basicTemplates.push_back(towerTemplate);
			continue;
		}
	
		TowerTemplate* parentTemplate = templatesMap[parentId];
		towerTemplate->level = parentTemplate->level + 1;
		parentTemplate->subTemplates.push_back(towerTemplate);
		
	}

}