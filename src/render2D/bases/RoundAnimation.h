#ifndef __ROUND_ANIMATION_H__
#define __ROUND_ANIMATION_H__

#include "Animation.h"
#include "Texture.h"

class RoundAnimation : public Animation
{
public:
	RoundAnimation(int time = 1000, int loop = 1, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), float speed = 1.0, Color color = Color());
	virtual ~RoundAnimation();
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:

	static Texture* roundTexture;

};

#endif
