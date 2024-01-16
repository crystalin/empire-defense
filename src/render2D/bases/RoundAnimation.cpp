#include "render2D_common.h"
#include "RoundAnimation.h" 
#include "RessourceManager.h"

Texture* RoundAnimation::roundTexture = NULL;

RoundAnimation::RoundAnimation(int time, int loop, const Rectangle<float>& area, float speed, Color color) : 
Animation(time,loop,area,speed,color)
{
	if (roundTexture==0) {
		roundTexture = RessourceManager::instance()->getTexture("round.png");
	}
	roundTexture->request();
}

RoundAnimation::~RoundAnimation() {
	roundTexture = roundTexture->release();
}

bool RoundAnimation::preDisplay(int addTime)
{
	int totalTime = timeLeft + addTime;
	glPushMatrix();
	glColor4fv(color.values);
	glRotatef(((totalTime%timeBase)/(float)timeBase)*360.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

void RoundAnimation::postDisplay(int addTime)
{
	roundTexture->drawSquare(area);
	glPopMatrix();
}