#include "render2D_common.h"
#include "Controller.h" 
#include "Icons.h" 
#include "Button.h" 
#include "UIObservable.h" 

Controller* Button::controller = NULL;

Button::Button(int commandID, float size, Item* content, const Color& color, PositionController* positionController) :
Item(positionController), size(size), content(content), commandID(commandID), color(color)
{
	if (content != NULL) {
		content->request();
	}
	//colorInside = (color.contrast(0.7f) + 0.3f);
	colorInside = (color + 0.4f);
	colorPressed = Color(0.3f, 0.3f, 0.4f, 1.0f);
}
Button::~Button() {
	if (content != NULL) {
		content->release();
	}
}

void Button::setVisible(bool visible)
{
	if (content != NULL) {
		content->setVisible(visible);
	}
	Item::setVisible(visible);
}

bool Button::contains(const Point2D<float>& mousePosition) { 
	const Point2D<float>& position = positionController->getPosition();
	return visible && (position.distance(mousePosition) <= size/2);
}

void Button::mousePress(const Point2D<float>& mousePosition){
	Item::mousePress(mousePosition);
	if (pressed) {
		uiObservable->notifyPressButton(this);
	}
}

void Button::mouseRelease(const Point2D<float>& mousePosition, bool click)
{
	if (!visible || !pressed) return;
	pressed = false;
	Item::mouseRelease(mousePosition, click);
	if (click && controller && enable) {
		uiObservable->notifyReleaseButton(this);
		controller->processCommand(commandID);
	}
}
void Button::update(int timeSpent)
{
	if (!visible) return;
	Item::update(timeSpent);
	if (content != NULL) {
		content->update(timeSpent);
	}
}


void Button::display()
{
	if (!visible) {
		return;
	}

	const Point2D<float>& position = positionController->getPosition();
	float halfSize = size/2;

	glPushMatrix();
	//cout << " position.x " << position.x << " size/2 " << size/2 << endl;
	glTranslatef(position.x, position.y,0.0f);
	if (enable) {
		if (pressed) {
			glColor4fv(colorPressed.values);
		} else if (inside) {
			//glColor4f(0.4,0.4,0.4, 1.0f);
			glColor3fv(colorInside.values);
		} else {
			glColor4fv(color.values);
		} 
	} else {
		glColor4f(0.6,0.2,0.2,1.0f);
	}

	Icons::displayBackground(Rectangle<float>(-halfSize, -halfSize, halfSize, halfSize), false,pressed);
	if (content != NULL) {
		//glPushMatrix();
		//glTranslatef(-halfSize, -halfSize,0.0f);
		content->display();
		//glPopMatrix();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (halfSize>60.0f) {
		Icons::displayEffect(ICON_BACKGROUND_BIG_OVER,Rectangle<float>(-halfSize, -halfSize, halfSize, halfSize));
	} else {
		Icons::displayEffect(ICON_BACKGROUND_OVER,Rectangle<float>(-halfSize, -halfSize, halfSize, halfSize));
	}

	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->display();
		}
	}
	glPopMatrix();
}
