#include "render2D_common.h"
#include "ExplosionGenerator.h"
#include "RessourceManager.h"
#include "Shader.h"

ExplosionGenerator::ExplosionGenerator(Texture* texture, int particleCount, float speed) : 
Animation(10000,1,Rectangle<float>(0,0,100,100),speed,Color(1.0f, 1.0f, 1.0f, 1.0f)), particleCount(particleCount), explosionNumber(0), mapTexture(texture), explosionShader(NULL)
{
	texture->request();
	explosionShader = new Shader("../ressources/shockwave-vert.txt", "../ressources/shockwave-frag.txt");

	for (int i=0; i<EXPLOSION_NUMBER+1; i++) {
		explosions[i].time = 0;
	}
}

ExplosionGenerator::~ExplosionGenerator() 
{
	mapTexture = mapTexture->release();
	delete explosionShader; // TODO : share shaders in RessourceManager
}

void ExplosionGenerator::addExplosion(const Point2D<float>& position, float range, const Point2D<float>& mapSize) 
{
	for (int i= (explosionNumber + 1) % EXPLOSION_NUMBER; i != explosionNumber; i = (i + 1) % EXPLOSION_NUMBER)
	{
		if (explosions[i].time <= 0) {
			explosionNumber = i;
			break;
		}
	}
	explosions[explosionNumber].position = position;
	explosions[explosionNumber].mapSize = mapSize;
	explosions[explosionNumber].time = 1000;
	explosions[explosionNumber].range = range;
	
}

void ExplosionGenerator::update(int timeSpent)
{
	if (state == 0) return ;
	timeSpent *= speed ;

	for (int i=0; i<EXPLOSION_NUMBER; i++) {
		Explosion& explosion = explosions[i];
		if (explosion.time >= 0) {
			explosion.time -= timeSpent;
		}
	}
}

bool ExplosionGenerator::preDisplay(int addTime)
{
	return true;
}

void ExplosionGenerator::postDisplay(int addTime)
{
	explosionShader->enable();
	mapTexture->bind();
	float angle = 360 / particleCount * DEG2RAD;
	glPushMatrix();
	for (int i=0; i<EXPLOSION_NUMBER; i++) {
		Explosion& explosion = explosions[i];
		//cout << "time : " << time << endl;
		if (explosion.time <= 0){
			continue;
		}

		float range = explosion.range*2.0f;
	
		float x = explosion.position.x;
		float y = explosion.position.y;

		float mapX = x / explosion.mapSize.x ;
		float mapY = y / explosion.mapSize.y ;

		float mapDistanceX = range / explosion.mapSize.x;
		float mapDistanceY = range / explosion.mapSize.y;

		float mapRange = explosion.range*1.3f / explosion.mapSize.x;

		float time = explosion.time / 1000.0f;
		time = time*time;
		glColor4f(0.8f, 0.7f, 0.5f,  time);
		time = time*time*time;
		explosionShader->uniform2f("center",mapX,mapY);
		explosionShader->uniform1f("time",mapRange-time*mapRange);

		glBegin(GL_QUADS);
		glTexCoord2f( mapX-mapDistanceX, mapY-mapDistanceY);
		glVertex2f(x-range, y-range);
		glTexCoord2f( mapX+mapDistanceX, mapY-mapDistanceY);
		glVertex2f(x+range, y-range);
		glTexCoord2f( mapX+mapDistanceX, mapY+mapDistanceY);
		glVertex2f(x+range, y+range);
		glTexCoord2f( mapX-mapDistanceX, mapY+mapDistanceY);
		glVertex2f(x-range, y+range);
		glEnd();
	}
	glPopMatrix();
	explosionShader->disable();
}