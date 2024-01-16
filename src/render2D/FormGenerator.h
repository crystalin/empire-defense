#ifndef __FORM_GENERATOR__
#define __FORM_GENERATOR__

#include "Model.h" 
#include "Texture.h" 
#include "Color.h" 

class FormGenerator
{
public:

	static void drawBox(float x, float y, float z, float size);
	static void drawSquare(const Rectangle<float>& area = Rectangle<float>(0,0,32,32));
	static void drawSquare(const Rectangle<float>& area, Color color);
	static void drawCircle(float radius);
	static void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);

};

#endif
