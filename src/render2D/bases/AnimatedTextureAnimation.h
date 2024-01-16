#ifndef __ANIMATED_TEXTURE_ANIMATION_H__
#define __ANIMATED_TEXTURE_ANIMATION_H__

#include "TextureAnimation.h"

class AnimatedTextureAnimation : public TextureAnimation
{
public:
	AnimatedTextureAnimation(int steps, Texture* texture, float fadeTime = 0.2f, int time = 1000, int loop = 1, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), float speed = 1.0f, Color color = Color());

	virtual void update(int timeSpent);
	virtual void start() { state = 1; timeLeft = timeBase; currentStep = 0; }
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:

	int currentStep;
	int steps;

};

#endif
