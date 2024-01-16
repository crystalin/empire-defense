#ifndef __TEXTURE_ANIMATION_H__
#define __TEXTURE_ANIMATION_H__

#include "Animation.h"
#include "Texture.h"

class TextureAnimation : public Animation
{
public:
	TextureAnimation(Texture* texture, float fadeTime = 0.2f, int time = 1000, int loop = 1, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), float speed = 1.0f, Color color = Color());
	TextureAnimation(Texture* texture, float fadeTime = 0.0f, float fadeOutTime = 0.2f, int time = 1000, int loop = 1, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), float speed = 1.0f, Color color = Color());
	virtual ~TextureAnimation();

	virtual void start() { state = 1; timeLeft = timeBase; };
	virtual void stop() { state = 0; timeLeft = fadeOutLimit; };

	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:

	int fadeInLimit;
	int fadeOutLimit;

	Texture* texture;

};

#endif
