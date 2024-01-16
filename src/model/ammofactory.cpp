#include "model_common.h"
#include "ammofactory.h"

string Factory<AmmoTemplate, Ammo>::factoryName = "Ammo Factory";

void Factory<AmmoTemplate, Ammo>::init(ConfigReader& configReader)
{
	list <AmmoTemplate*> templateList;
	istringstream iss;
	for (map<string,map<string,string>>::iterator ci = configReader.begin(); ci != configReader.end(); ++ci) {
		string name = ci->first;
		int id = 0;
		int type = AMMO_DEFAULT_TYPE;
		float damage = AMMO_DEFAULT_DAMAGE;
		float radius = AMMO_DEFAULT_RADIUS;
		float speed = AMMO_DEFAULT_SPEED;
		int time = AMMO_DEFAULT_TIME;

		map<string, string>& config = ci->second;

		configReader.read(config,"id",id);
		configReader.read(config,"damage",damage);
		configReader.read(config,"radius",radius);
		configReader.read(config,"speed",speed);
		configReader.read(config,"time",time);

		if(config.find("type") != config.end()) {
			string stype = config["type"];
			if (stype.compare("TYPE_AMMO_ARROW") == 0) {
				type = TYPE_AMMO_ARROW;
			} else if (stype.compare("TYPE_AMMO_LASER") == 0) {
				type = TYPE_AMMO_LASER;
			} else if (stype.compare("TYPE_AMMO_MISSILE") == 0) {
				type = TYPE_AMMO_MISSILE;
			}  else if (stype.compare("TYPE_AMMO_PROJECTILE") == 0) {
				type = TYPE_AMMO_PROJECTILE;
			}else {
				iss.str(stype);
				if (!(iss >> type)) {
					cerr << "[AmmoFactory] invalid Type reading value = " << config["type"] << endl;
				}
				iss.clear();
			}
		}

		if (id ==0 ) {
			cerr << "[AmmoFactory] Missing ID for ammo \"" << name << "\"" << endl;
		} else {
			if (find_if(templateList.begin(), templateList.end(), bind(&AmmoTemplate::id,_1)==id) != templateList.end()) {
				cerr << "[AmmoFactory] Ammo " << id << "already defined !!" << endl;
				break;
			}
			templateList.push_back(new AmmoTemplate(id, name, time, type, damage, radius, speed));
		}
	}
	templateList.sort(compare);
	storeTemplates(templateList);
}