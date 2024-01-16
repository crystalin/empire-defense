#ifndef __EFFECT_H__
#define __EFFECT_H__

#define EFFECT_FIRE 0
#define EFFECT_ICE 1
#define EFFECT_EARTH 2
#define EFFECT_POISON 3
#define EFFECT_GOLD 4
#define EFFECT_LIGHT 5
#define EFFECT_MALEDICTION 6
#define EFFECT_ELECTRICITY 7

#define EFFECT_NUMBER 8

#define EFFECT_DEFAULT_NAME "spell"
#define EFFECT_DEFAULT_TYPE EFFECT_FIRE
#define EFFECT_DEFAULT_TIME 5000
#define EFFECT_DEFAULT_COST 5
#define EFFECT_DEFAULT_VALUE 10.0f
#define EFFECT_DEFAULT_RADIUS 0.0f

#define EFFECT_FREQUENCY 1000

class Party;
class Unit;

class EDMODEL_API EffectTemplate {
public:
	EffectTemplate(int id, string name, int parentId, int type, int time, float value, int cost, float radius);

	string name;
	int id;
	int parentId;
	int time;
	int type;
	int cost;
	int level;
	float radius;
	float value;

	vector<const EffectTemplate*> subTemplates;
};

class EDMODEL_API Effect : boost::noncopyable 
{
public:

	Effect(Party* party, const EffectTemplate& effectTemplate);
	~Effect();

	const string& getName() const { return effectTemplate.name; }
	int getType() const { return effectTemplate.type; }
	int getTime() const { return effectTemplate.time; }
	int getCost() const { return effectTemplate.cost; }
	float getRadius() const { return effectTemplate.radius; }
	int getLevel() const { return effectTemplate.level; }
	float getValue() const { return effectTemplate.value; }
	int getId() const { return effectTemplate.id; }
	int getParentId() const { return effectTemplate.parentId; }
	int getTimeLeft() const { return timeLeft; }
	bool isActive() const { return timeLeft > 0; }
	const vector<const EffectTemplate*>& getSubTemplates() const { return effectTemplate.subTemplates; }
	void restoreTime() { timeLeft = effectTemplate.time + (timeLeft % EFFECT_FREQUENCY); }

	ostream& print(ostream& os) const { return os << "[Effect " << effectTemplate.name << "] type = " << effectTemplate.type << ", time = " << effectTemplate.time << ", value = "  << effectTemplate.value << ", parent = "<< effectTemplate.parentId;}

	bool update(int timeSpent, Unit& unit);

protected:


	void destroy();
	void resurect(const EffectTemplate* effectTemplate);

	const EffectTemplate&	effectTemplate;

	int timeLeft;
	int frequenceLeft;

	Party& party;


};

ostream& operator << (ostream& os, const Effect& effect);
/*
class EffectUpdator {
private:
	int timeSpent;
	Unit& unit;
public:
	EffectUpdator(int timeSpent, Unit& unit) : timeSpent(timeSpent), unit(unit) {}
	bool operator()(Effect& effect) {
		return effect.update(timeSpent,unit);
	}
};

class EffectUpgrador {
private:
	const EffectTemplate* effectTemplate;
public:
	EffectUpgrador(const EffectTemplate* effectTemplate) : effectTemplate(effectTemplate) {}
	bool operator()(Effect& effect) {
		if (effect.getType() == effectTemplate.type) {
			if (effect.getTimeLeft() <= 0 || effect.getId() < effectTemplate.id) {
				effect = Effect(effectTemplate);
			} else if (effect.getId() == effectTemplate.id) {
				effect.restoreTime();
			}
			return true;
		}
		return false;
	}
};

class EffectGenerator {
private:
	int i;
public:
	EffectGenerator() : i(0) {}
	Effect operator()() {
		return Effect(&EffectTemplate::basicEffectTemplates[(i++) % EFFECT_NUMBER]);
	}
};*/

#endif