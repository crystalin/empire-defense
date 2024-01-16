#include "model_common.h"
#include "effect.h"
#include "factory.h"
#include "unit.h"

EffectTemplate::EffectTemplate(int id, string name, int parentId, int type, int time, float value, int cost, float radius) :
id(id), name(name), parentId(parentId), type(type), time(time), value(value), cost(cost), radius(radius), level(0)
{
	subTemplates.reserve(20);
	if (id <= parentId) {
		cout << " warning : Parent ID [" << parentId << "] is greater than ID [" << id << "]" << endl;
	}
}

Effect::Effect(Party* party, const EffectTemplate& effectTemplate) :
party(*party), effectTemplate(effectTemplate), timeLeft(effectTemplate.time), frequenceLeft(effectTemplate.time)
{
	//cout << *this << " created" << endl;
}

Effect::~Effect()
{
}

bool Effect::update(int timeSpent, Unit& unit)
{
	if (timeLeft <= 0) {
		return true;
	}
	timeLeft -= timeSpent;

	if (effectTemplate.type == EFFECT_FIRE) {
		frequenceLeft -= timeSpent;
		if (frequenceLeft<=0) {
			unit.receiveDamages(getValue());
			frequenceLeft+=EFFECT_FREQUENCY;
		}
	}
	return false;
}

ostream& operator << (ostream& os, const Effect& effect)
{
	return effect.print(os);
}