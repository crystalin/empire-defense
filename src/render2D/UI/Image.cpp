#include "render2D_common.h"
#include "Image.h" 
#include "FormGenerator.h" 

Image::Image(Animation* animation, PositionController* positionController) :
Item(positionController), animation(animation)
{
	animation->start();
}

Image::~Image()
{
	delete animation;
}

void Image::update(int timeSpent)
{
	if (!visible) return;
	Item::update(timeSpent);
	animation->update(timeSpent);
}

void Image::setVisible(bool visible)
{ 
	this->visible = visible;
	if (visible == false) {
		inside = false;
		pressed = false;
		animation->stop();
	} else {
		animation->start();
	}
}

void Image::display()
{
	const Point2D<float>& position = positionController->getPosition();
	bool move = position.x != 0.0f || position.y != 0.0f;
	glPushMatrix();
	if (move) {
		glTranslatef(position.x, position.y,0.0f);
	}
	animation->display();

	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->display();
		}
	}
	glPopMatrix();
}