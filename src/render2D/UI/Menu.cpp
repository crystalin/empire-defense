#include "render2D_common.h"
#include "Menu.h" 
#include "Texture.h" 
#include "LaserAnimation.h" 

Menu::Menu(PositionController* positionController, int direction) :
Container<Button>(positionController), direction(direction)
{
}

void Menu::clear() {
	Container::clear();
}

void Menu::addItem(Button* button) {
	Container::addItem(button);
	int count = items.size() - 1;
	int pos = count * button->getSize();
	if (direction == MENU_HORIZONTAL) {
		button->setPosition(Point2D<float>(pos, 0.0f));
	} else {
		button->setPosition(Point2D<float>(0.0f, -pos));
	}
}