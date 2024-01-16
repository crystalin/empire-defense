#include "model_common.h"
#include "unitfactory.h"

string Factory<UnitTemplate, Unit>::factoryName = "Unit Factory";

void Factory<UnitTemplate, Unit>::init(ConfigReader& configReader)
{
	list <UnitTemplate*> templateList;
	istringstream iss;
	for (map<string,map<string,string>>::iterator ci = configReader.begin(); ci != configReader.end(); ++ci) {
		string name = ci->first;
		int id = 0;
		int level = UNIT_DEFAULT_LEVEL;
		float maxLife = UNIT_DEFAULT_MAX_LIFE;
		float speed = UNIT_DEFAULT_SPEED;
		float armor = UNIT_DEFAULT_ARMOR;
		bool invisible = UNIT_DEFAULT_INVISIBLE;

		map<string, string>& config = ci->second;


		configReader.read(config,"id",id);
		configReader.read(config,"level",level);
		configReader.read(config,"life",maxLife);
		configReader.read(config,"speed",speed);
		configReader.read(config,"armor",armor);
		configReader.read(config,"invisible",invisible);

		if (id ==0 ) {
			
			if (find_if(templateList.begin(), templateList.end(), bind(&UnitTemplate::id,_1)==id) != templateList.end()) {
				cerr << "[UnitTemplate] unit " << id << "already defined !!" << endl;
				break;
			}
			cerr << "[UnitTemplate] Missing ID for tower \"" << name << "\"" << endl;
		} else {
			templateList.push_back(new UnitTemplate(id, name, level, maxLife, speed, armor, invisible));
		}
	}
	templateList.sort(compare);
	storeTemplates(templateList);
}
