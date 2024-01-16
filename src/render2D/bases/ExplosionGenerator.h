#ifndef __EXPLOSION_GENERATOR_H__
#define __EXPLOSION_GENERATOR_H__

#include "Animation.h"
#include "Texture.h"

#define EXPLOSION_NUMBER 40

class Shader;

typedef struct {
	int time;
	float range;
	Point2D<float> position;
	Point2D<float> mapSize;
} Explosion;

class ExplosionGenerator : public Animation
{
public:
	ExplosionGenerator(Texture* texture, int Particlecount = 30, float speed = 1.0f);
	virtual ~ExplosionGenerator();

	virtual void addExplosion(const Point2D<float>& position, float range, const Point2D<float>& mapSize);
	
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);

	virtual void update(int timeSpent);
	virtual bool isFinished() { return false; }
protected:

	Explosion explosions[EXPLOSION_NUMBER];
	int explosionNumber;
	int particleCount;
	Texture* mapTexture;
	Shader* explosionShader;

};

#endif
