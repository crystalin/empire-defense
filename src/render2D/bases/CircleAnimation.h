#ifndef __CIRCLE_ANIMATION_H__
#define __CIRCLE_ANIMATION_H__

#include "Animation.h"
#include "Texture.h"

class CircleAnimation : public Animation
{
public:
	CircleAnimation(int time = 1000, int loop = 1, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), float speed = 1.0f, Color color = Color());
	virtual ~CircleAnimation();
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:
	static Texture* circleTexture;

};

#endif
