#include "render2D_common.h"
#include "TextureAnimation.h" 

TextureAnimation::TextureAnimation(Texture* texture, float fadeTime, float fadeOutTime, int time, int loop, const Rectangle<float>& area, float speed, Color color) : 
Animation(time,loop,area,speed,color), texture(texture), fadeInLimit(time*fadeTime),  fadeOutLimit(time*fadeOutTime)
{
	if (fadeInLimit>time || fadeInLimit<0) { 
		fadeInLimit = 0; 
	}
	if (fadeOutTime>time || fadeOutTime<0) { 
		fadeOutTime = 0; 
	}
	texture->request();
}

TextureAnimation::TextureAnimation(Texture* texture, float fadeTime, int time, int loop, const Rectangle<float>& area, float speed, Color color) : 
Animation(time,loop,area,speed,color), texture(texture), fadeInLimit(time*fadeTime),  fadeOutLimit(time*fadeTime)
{
	if (fadeInLimit>time || fadeInLimit<0) { 
		fadeInLimit = 0; 
	} else if (fadeInLimit>time/2.0f) {
		fadeInLimit = time - fadeInLimit;
	}
	texture->request();
}

TextureAnimation::~TextureAnimation() {
	texture->release();
}

bool TextureAnimation::preDisplay(int addTime)
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

void TextureAnimation::postDisplay(int addTime)
{
	texture->drawSquare(area);
}