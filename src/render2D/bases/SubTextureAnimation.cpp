#include "render2D_common.h"
#include "SubTextureAnimation.h" 

SubTextureAnimation::SubTextureAnimation(Texture* texture, const Rectangle<float>& area, const Rectangle<float>& texArea, float fadeTime, int time, int loop, float size, float speed, Color color) : 
TextureAnimation(texture, fadeTime, time, loop, area, speed, color), texArea(texArea)
{
	if (fadeTime>1 || fadeTime<0) { 
		fadeTime = 0;
	} else if (fadeTime>0.5) {
		fadeTime = 1-0.5;
	}
}

bool SubTextureAnimation::preDisplay(int addTime)
{
	Color colorFinal(color);
	if (timeBase - timeLeft < fadeInLimit) {
		colorFinal.values[3] *= (timeBase - timeLeft)/(float)fadeInLimit;
	} else if (timeLeft < fadeOutLimit) {
		colorFinal.values[3] *= timeLeft/(float)fadeOutLimit;
	}
	glColor4fv(colorFinal.values);
	return true;
}

void SubTextureAnimation::postDisplay(int addTime)
{
	texture->drawSquare(area,texArea);
}