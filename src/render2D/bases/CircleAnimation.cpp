#include "render2D_common.h"
#include "CircleAnimation.h" 
#include "RessourceManager.h"

Texture* CircleAnimation::circleTexture = NULL;

CircleAnimation::CircleAnimation(int time, int loop, const Rectangle<float>& area, float speed, Color color) : 
Animation(time,loop,area,speed,color)
{
	if (circleTexture==NULL) {
		circleTexture = RessourceManager::instance()->getTexture("circle.png");
	}
	circleTexture->request();
}

CircleAnimation::~CircleAnimation() {
	circleTexture = circleTexture->release();
}

bool CircleAnimation::preDisplay(int addTime)
{
	glPushMatrix();
	return true;
}

void CircleAnimation::postDisplay(int addTime)
{
	int totalTime = timeLeft + addTime;
	glRotatef(((totalTime%timeBase)/(float)timeBase)*360.0f, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, circleTexture->getID());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(area.x1, area.y1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(area.x2, area.y1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(area.x2, area.y2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(area.x1, area.y2);
	glPopMatrix();
}