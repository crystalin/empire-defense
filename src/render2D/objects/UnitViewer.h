#ifndef __UNIT_VIEWER_H__
#define __UNIT_VIEWER_H__

#include "animation.h"

class Unit;
class Shader;
class UnitTemplate;
class Texture;

class UnitViewer
{
public:
	static void update(Unit* unit);
	static void display(Unit* unit);
	static void display(const UnitTemplate* unitTemplate);
	static void init();
	static void destroy();

protected:
	UnitViewer();
	~UnitViewer();
	
	static Texture* unitTexture;
	static Texture* unitLifeTexture;
	static Shader* unitColorShader;
};

#endif
