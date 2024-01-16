#ifndef __EFFECT_FACTORY_H__
#define __EFFECT_FACTORY_H__
#include "factory.h"
#include "effect.h"
/*
#include "model_common.h"
#include "effect.h"

#define MAX_SPELLS 100

typedef struct TIMED_EFFECT{
	int timeLeft;
	int timeBase;
	const Effect* effect;
} TimedEffect;

class EffectFactory
{
public:
	static void addEffect(int ID, Effect* effect);
	static const Effect* getEffect(int ID);

	static void readEffects(ConfigReader& configReader);
	static void destroy();
	static const list<const Effect*>& getBasicEffects() { return basicEffects; }
	static const list<const Effect*>& getAllEffects() { return allEffects; }

protected:
	static Effect* effectsMap[MAX_SPELLS];
	static list<const Effect*> basicEffects;
	static list<const Effect*> allEffects;
};
*/
#endif