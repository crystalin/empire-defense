#include "model_common.h"
#include "effectfactory.h"

string Factory<EffectTemplate, Effect>::factoryName = "Effect Factory";

void Factory<EffectTemplate, Effect>::init(ConfigReader& configReader)
{
	list <EffectTemplate*> templateList;
	istringstream iss;
	for (map<string,map<string,string>>::iterator ci = configReader.begin(); ci != configReader.end(); ++ci) {
		string name = ci->first;
		int id = 0;
		int parentId = 0;
		int cost = EFFECT_DEFAULT_COST;
		float value = EFFECT_DEFAULT_VALUE;
		float radius = EFFECT_DEFAULT_RADIUS;
		int time = EFFECT_DEFAULT_TIME;
		int type = EFFECT_DEFAULT_TYPE;

		map<string, string>& config = ci->second;


		configReader.read(config,"id",id);
		configReader.read(config,"parent",parentId);
		configReader.read(config,"cost",cost);
		configReader.read(config,"radius",radius);
		configReader.read(config,"value",value);
		configReader.read(config,"time",time);
		if(config.find("type") != config.end()) {
			string stype = config["type"];
			if (stype.compare("EFFECT_FIRE") == 0) {
				type = EFFECT_FIRE;
			} else if (stype.compare("EFFECT_ICE") == 0) {
				type = EFFECT_ICE;
			} else if (stype.compare("EFFECT_EARTH") == 0) {
				type = EFFECT_EARTH;
			} else if (stype.compare("EFFECT_POISON") == 0) {
				type = EFFECT_POISON;
			} else if (stype.compare("EFFECT_GOLD") == 0) {
				type = EFFECT_GOLD;
			} else if (stype.compare("EFFECT_LIGHT") == 0) {
				type = EFFECT_LIGHT;
			} else if (stype.compare("EFFECT_MALEDICTION") == 0) {
				type = EFFECT_MALEDICTION;
			} else if (stype.compare("EFFECT_ELECTRICITY") == 0) {
				type = EFFECT_ELECTRICITY;
			} else {
				iss.str(stype);
				if (!(iss >> type)) {
					cerr << "[EffectFactory] invalid Type reading value = " << config["type"] << endl;
				}
				iss.clear();
			}
			if (type<0 ||  type>=EFFECT_NUMBER) {
				cerr << "[EffectFactory] Type " << type << " is out of range" << endl;
			}
		}

		if (id ==0 ) {
			if (find_if(templateList.begin(), templateList.end(), bind(&EffectTemplate::id,_1)==id) != templateList.end()) {
				cerr << "[EffectFactory] Effect " << id << "already defined !!" << endl;
				break;
			}
			cerr << "[EffectFactory] Missing ID for effect \"" << name << "\"" << endl;
		} else {
			templateList.push_back(new EffectTemplate(id, name, parentId, type, time, value, cost, radius));
		}
	}
	templateList.sort(compare);
	storeTemplates(templateList);
	

	for (list<EffectTemplate*>::iterator it = templateList.begin(); it != templateList.end(); ++it){
		EffectTemplate* effectTemplate = *it;

		int parentId = effectTemplate->parentId;
		if (parentId == 0) {
			effectTemplate->level = 0;
			basicTemplates.push_back(effectTemplate);
			continue;
		}
	
		EffectTemplate* parentTemplate = templatesMap[parentId];
		effectTemplate->level = parentTemplate->level + 1;
		parentTemplate->subTemplates.push_back(effectTemplate);
		
	}
}

/*Effect* EffectFactory::effectsMap[MAX_SPELLS] = {NULL};
list<const Effect*> EffectFactory::basicEffects = list<const Effect*>();
list<const Effect*> EffectFactory::allEffects = list<const Effect*>();

void EffectFactory::addEffect(int ID, Effect* effect)
{
	if (ID<0 || ID>=MAX_SPELLS) {
		cerr << "[EffectFactory] effect template id " << ID << " is out of range" << endl;
		return;
	}
	if(effectsMap[ID] != NULL) {
		cerr << "[EffectFactory] effect template id " << ID << "is already defined" << endl;
		delete effectsMap[ID];
	}
	effectsMap[ID] = effect;
	allEffects.push_back(effect);
	if (effect->getParentId() == 0) {
		basicEffects.push_back(effect);
	}
}

const Effect* EffectFactory::getEffect(int ID)
{
	if(effectsMap[ID] == NULL) {
		cerr << "[EffectFactory] effect template id " << ID << " is not defined" << endl;
	}
	return effectsMap[ID];
}


void EffectFactory::destroy()
{
	for (int i =0; i<MAX_SPELLS ; i++) {
		if (effectsMap[i] != NULL) {
			delete effectsMap[i];
		}
	}
}

void EffectFactory::readEffects(ConfigReader& configReader)
{
	istringstream iss;
	for (map<string,map<string,string>>::iterator ci = configReader.begin(); ci != configReader.end(); ++ci) {
		string name = ci->first;
		int id = 0;
		int parentID = 0;

		map<string, string>& config = ci->second;

		if(config.find("id") != config.end()) {
			iss.str(config["id"]);
			if (!(iss >> id)) {
				cerr << "[EffectFactory] invalid ID reading value = " << config["id"] << endl;
			} 
			iss.clear();
		}

		if(config.find("parent") != config.end()) {
			iss.str(config["parent"]);
			if (!(iss >> parentID)) {
				cerr << "[EffectFactory] invalid parent reading value = " << config["parent"] << endl;
			} 
			iss.clear();
		}
		if(config.find("time") != config.end()) {
			iss.str(config["time"]);
			if (!(iss >> time)) {
				cerr << "[EffectFactory] invalid Time reading value = " << config["time"] << endl;
			}
			iss.clear();
		}
		if(config.find("cost") != config.end()) {
			iss.str(config["cost"]);
			if (!(iss >> cost)) {
				cerr << "[EffectFactory] invalid Cost reading value = " << config["cost"] << endl;
			}
			iss.clear();
		}
		if(config.find("value") != config.end()) {
			iss.str(config["value"]);
			if (!(iss >> value)) {
				cerr << "[EffectFactory] invalid Value reading value = " << config["value"] << endl;
			}
			iss.clear();
		}
		
		if (id ==0) {
			cerr << "[EffectFactory] Missing ID for effect \"" << name << "\"" << endl;
		} else {
			addEffect(id, new Effect(id,parentID,name,type,time,cost,value));
		}
	}
	for (int i =0; i<MAX_SPELLS ; i++) {
		Effect* effect = effectsMap[i];
		if (effect != NULL) {
			int parentId = effect->getParentId();
			if (effect->getParentId() == 0) {
				continue;
			}
			if (parentId > 0 && parentId < MAX_SPELLS) {
				Effect* parentEffect = effectsMap[parentId];
				if (parentEffect != NULL) {
					parentEffect->addSubEffect(effect);
				} else {
					cout << "Using un unknow parent ID [" << parentId << "]" << endl;
				}
			} else {
				cout << "parent ID [" << parentId << "] is out of range [0," << MAX_SPELLS << "]" << endl;
			}
		}
	}
}*/