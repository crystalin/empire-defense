#include "render2D_common.h"
#include "SmokeGenerator.h"
#include "RessourceManager.h"

Texture* SmokeGenerator::smokeTexture = NULL;
float SmokeGenerator::TexturePosition[TEXTURE_COUNT+1] = {0};

SmokeGenerator::SmokeGenerator(float life, const Rectangle<float>& area, const Point2D<float>& size, float speed, Color color, float xg, float yg) : 
Animation(10000,1,area,speed,color), life(life), size(size), xg(xg), yg(yg), count(0)
{
	if (smokeTexture==NULL) {
		smokeTexture = RessourceManager::instance()->getTexture("smoke.png");
	}
	smokeTexture->request();

	for (int i=0; i<TEXTURE_COUNT+1; i++) {
		TexturePosition[i] = i/(float)TEXTURE_COUNT;
	}

	for (int i=0; i<SMOKE_TEMPLATE_NUMBER; i++) {
		smokeTemplates[i] = initSmoke();
	}

	for (int i=0; i<SMOKE_PACK_NUMBER; i++) {
		SmokePack& pack = smokePacks[i];
		pack.count = 0;
		pack.emptyIndex = 0;

		for (int j=0; j<SMOKE_PACK_SIZE; j++) {
			Smoke& smoke = pack.smokes[j];
			smoke.active = false;
		}
	}
}

SmokeGenerator::~SmokeGenerator() 
{
	smokeTexture = smokeTexture->release();
}

void SmokeGenerator::addSmoke(int packNumber, const Rectangle<float>& area, int number, const Color& color) 
{
	if (packNumber >= SMOKE_PACK_NUMBER || packNumber < 0) {
		return;
	}
	SmokePack& pack = smokePacks[packNumber];
	if (pack.count>=SMOKE_PACK_SIZE) {
		return;
	}

	float middleX = area.getSizeX() / 2.0f;
	float middleY = area.getSizeY() / 2.0f;

	float centerX = area.x1 + middleX;
	float centerY = area.y1 + middleY;

	for (int i=0; i<number && pack.count < SMOKE_PACK_SIZE; i++) {
		Smoke smoke = smokeTemplates[count%SMOKE_TEMPLATE_NUMBER];
		smoke.x = centerX + smoke.x * middleX;
		smoke.y = centerY + smoke.y * middleY;

		if (color.values[3] != 0) {
			smoke.colors[0] = color.values[0];
			smoke.colors[1] = color.values[1];
			smoke.colors[2] = color.values[2];
			smoke.colors[3] = color.values[3];
		}

		for (int i=(pack.emptyIndex+1)%SMOKE_PACK_SIZE; i!=pack.emptyIndex; i = (i+1)%SMOKE_PACK_SIZE) {
			if(!pack.smokes[i].active) {
				pack.emptyIndex = i;
				pack.smokes[i] = smoke;
				pack.count++;
				count++;
				break;
			}
		}
	}
}

Smoke SmokeGenerator::initSmoke()
{
	Smoke smoke;
	if (life >= 1.0f ){
		smoke.fade = myRand(1/life,1);
	} else {
		smoke.fade = myRand(1,1/life);
	}

	smoke.active = true;
	smoke.texture = (int)myRand(0,3.99f);

	float colorIntensity = myRand(-0.3,0.3);
    smoke.colors[0]=colorIntensity * color.values[0] + color.values[0] ;
    smoke.colors[1]=colorIntensity * color.values[1] + color.values[1] ;
    smoke.colors[2]=colorIntensity * color.values[2] + color.values[2] ;
    smoke.colors[3]=colorIntensity * color.values[3] + color.values[3] ;

	for (int i= 0; i<4; i++) {
		if (smoke.colors[i]<0) smoke.colors[i] = 0;
		if (smoke.colors[i]>1) smoke.colors[i] = 1;
	}

	if (smoke.colors[3]<0) smoke.colors[3] = 0.1;

    smoke.x = myRand(-1.0f,1.0f);
    smoke.y = myRand(-1.0f,1.0f);

    smoke.xi = myRand(-0.2f,0.2f) + xg;
    smoke.yi = myRand(-0.2f,0.3f) + yg;

	return smoke;
}

void SmokeGenerator::update(int timeSpent)
{
	if (state == 0) return ;
	float timeSpentf = timeSpent/1000.0f * speed;
	
	for (int i=0; i<SMOKE_PACK_NUMBER; i++) {
		SmokePack& pack = smokePacks[i];
		if (pack.count == 0){
			continue;
		}

		for (int j=0; j<SMOKE_PACK_SIZE; j++) {
			Smoke& smoke = pack.smokes[j];
			if (smoke.active == false) {
				continue;
			}

			smoke.x += smoke.xi*timeSpentf;
			smoke.y += smoke.yi*timeSpentf;

			//smoke.xi += xg*timeSpentf;
			//smoke.yi += yg*timeSpentf;

			smoke.colors[3] -=smoke.fade*timeSpentf;

			if (smoke.colors[3] <= 0.0f ) {
				smoke.active = false;
				pack.count--;
				pack.emptyIndex = i-1;
				count--;
			}
		}
	}
}

bool SmokeGenerator::preDisplay(int addTime)
{
	return true;
}

void SmokeGenerator::postDisplay(int addTime)
{
	smokeTexture->bind();

	glPushMatrix();
	glBegin(GL_QUADS);
	for (int i=0; i<SMOKE_PACK_NUMBER; i++) {
		SmokePack& pack = smokePacks[i];
		if (pack.count == 0){
			continue;
		}

		for (int j=0; j<SMOKE_PACK_SIZE; j++) {
			Smoke& smoke = pack.smokes[j];

			if (smoke.active == false) {
				continue;
			}

			float suppX = (1 - smoke.colors[3]) * 4 * size.x;
			float suppY = (1 - smoke.colors[3]) * 4 * size.y;

			float x = smoke.x;
			float y = smoke.y;

			float x2 = x+suppX;
			float y2 = y+suppY;

			int texPos = smoke.texture;

			glColor4fv(smoke.colors);

			glTexCoord2f(TexturePosition[texPos], 0.0f);
			glVertex2f(x, y);
			glTexCoord2f(TexturePosition[texPos+1], 0.0f);
			glVertex2f(x2, y);
			glTexCoord2f(TexturePosition[texPos+1], 1.0f);
			glVertex2f(x2, y2);
			glTexCoord2f(TexturePosition[texPos], 1.0f);
			glVertex2f(x, y2);
		}
	}
	glEnd();
	glPopMatrix();
}