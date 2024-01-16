#ifndef __TEXT_UI_H__
#define __TEXT_UI_H__

#include "Item.h"
#include "FontManager.h"
#include "Color.h"

class Text : public Item
{
public:
	Text(const string& text, Color color=Color(), const string& fontName = "normal" , float size = 1.0f, Font::Align align = Font::left, PositionController* positionController = new DefaultPositionController());

	virtual bool contains(const Point2D<float>& mousePosition);

	virtual void display();

	virtual ostream& print(ostream& os) const { return os << "Text " << id << " " << positionController->getPosition() ;}

protected:
	virtual ~Text();
	Color color;
	int callID;
	float size;

};

#endif
