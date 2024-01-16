#include "render2D_common.h"
#include "Text.h"

Text::Text(const string& text, Color color, const string& fontName, float size, Font::Align align, PositionController* positionController) :
Item(positionController), callID(0), color(color), size(size)
{
	const Font* font = FontManager::getFont(fontName);

	if (font == NULL) {
		cout << "font not found for text : " << text << "[" << fontName << "]" << endl;
		return;
	}

	callID = glGenLists(1);
	glNewList(callID,GL_COMPILE);
	font->renderString(0,0,text,align);
	glEndList(); 
}


Text::~Text()
{
	glDeleteLists(callID,1);
	callID=0;
}

bool Text::contains(const Point2D<float>& mousePosition)
{
	//Point2D<float> ecart = mousePosition - position;
	return false;
}

void Text::display()
{
	const Point2D<float>& position = positionController->getPosition();

	glColor4fv(color.values);
	glPushMatrix();
	glTranslatef(position.x,position.y,0.0f);
	glCallList(callID);

	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->display();
		}
	}
	glPopMatrix();
}