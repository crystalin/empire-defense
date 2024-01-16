#include "render2D_common.h"
#include "ShaderDeformer.h"

ShaderDeformer::ShaderDeformer(Animation *animation, Shader* shader) : 
Deformer(animation), shader(shader)
{
}

bool ShaderDeformer::preDisplay(int addTime)
{
	shader->enable();
	shader->uniform1f("time",timeLeft/1000.0);
	animation->preDisplay(addTime);
	return true;
}

void ShaderDeformer::postDisplay(int addTime)
{
	animation->postDisplay(addTime);
	shader->disable();
}