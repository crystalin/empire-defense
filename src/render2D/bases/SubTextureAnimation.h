#ifndef __SUB_TEXTURE_ANIMATION_H__
#define __SUB_TEXTURE_ANIMATION_H__

#include "TextureAnimation.h"

class SubTextureAnimation : public TextureAnimation
{
public:
	SubTextureAnimation(Texture* texture, const Rectangle<float>& area, const Rectangle<float>& texArea, float fadeTime = 0.2f, int time = 1000, int loop = 1, float size = 1.0f, float speed = 1.0f, Color color = Color());

	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:

	Rectangle<float> texArea;

};

#endif
