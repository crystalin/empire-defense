#ifndef __SMOKE_GENERATOR_H__
#define __SMOKE_GENERATOR_H__

#include "Animation.h"
#include "Texture.h"

#define SMOKE_TEMPLATE_NUMBER 40
#define SMOKE_PACK_SIZE 20
#define SMOKE_PACK_NUMBER 40
#define TEXTURE_COUNT 4


class Smoke
{
public:
	int texture;
	bool active;
	float fade;
	float colors[4];
	float x, y;
	float xi, yi;
};

class SmokePack
{
public:
	int count;
	int emptyIndex;
	Smoke smokes[SMOKE_PACK_SIZE];
};

class SmokeGenerator : public Animation
{
public:
	SmokeGenerator(float life = 1.0f, const Rectangle<float>& area = Rectangle<float>(0,0,100,100), const Point2D<float>& size = Point2D<float>(32,32), float speed = 1.0f, Color color = Color(0.8f, 0.2f, 0.2f, 0.9f), float xg = 0.0f, float yg = 0.0f );
	virtual ~SmokeGenerator();


	virtual void addSmoke(int packNumber, const Rectangle<float>& area = Rectangle<float>(0,0,64,64), int number = 4, const Color& color = Color(0,0,0,0));
	
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);

	virtual void update(int timeSpent);
	virtual bool isFinished() { return false; }

	virtual int getCount () { return count; }
protected:

	SmokePack smokePacks[40];
	Point2D<float> size;
	float xg, yg;
	float life;

	int count;

	static Texture* smokeTexture;

	Smoke initSmoke();

	Smoke smokeTemplates[SMOKE_TEMPLATE_NUMBER];
	static float TexturePosition[TEXTURE_COUNT+1];

};

#endif
