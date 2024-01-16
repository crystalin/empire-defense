#include "render2D_common.h"
#include "AnimatedTextureAnimation.h" 

AnimatedTextureAnimation::AnimatedTextureAnimation(int steps, Texture* texture, float fadeTime, int time, int loop, const Rectangle<float>& area, float speed, Color color) : 
TextureAnimation(texture, fadeTime, time, loop, area, speed, color), steps(steps)
{
	currentStep = 0;
}

void AnimatedTextureAnimation::update(int timeSpent)
{
	TextureAnimation::update(timeSpent);
	currentStep = (1 - (timeLeft/(float) timeBase)) * steps;
}

bool AnimatedTextureAnimation::preDisplay(int addTime)
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

void AnimatedTextureAnimation::postDisplay(int addTime)
{
	
	texture->drawSquare(area,Rectangle<float>(currentStep/(float)steps, 0.0f, (currentStep+1) / (float)steps, 1.0f));
}