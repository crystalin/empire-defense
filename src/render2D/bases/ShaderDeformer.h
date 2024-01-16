#ifndef __SHADER_ANIMATION_H__
#define __SHADER_ANIMATION_H__

#include "Deformer.h"
#include "Shader.h"

class ShaderDeformer : public Deformer
{
public:
	ShaderDeformer(Animation *animation, Shader* shader);

	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
protected:

	Shader* shader;

};
#endif
