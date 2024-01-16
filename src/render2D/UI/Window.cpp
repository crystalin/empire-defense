#include "render2D_common.h"
#include "Window.h" 
#include "TextureAnimation.h" 
#include "RessourceManager.h"

Texture* Window::texture = NULL;

Window::Window(PositionController* positionController, const Point2D<float>& size, Color color) :
Container<Item>(positionController), size(size), color(color)
{
	if (texture == NULL) {
		texture = RessourceManager::instance()->getTexture("window.png");
	}
	texture->request();

	for (int i=0; i<9; i++) {
		int U = i%3;
		int V = i/3;
		texArea[i] = Rectangle<float>(U/3.0f,V/3.0f ,(U+1)/3.0f, (V+1)/3.0f);
	}
}

Window::~Window()
{
	texture->release();
}

void Window::update(int timeSpent)
{
	Container::update(timeSpent);
	Item::update(timeSpent);
}

bool Window::contains(const Point2D<float>& mousePosition)
{
	if (!visible) return false;

	const Point2D<float>& position = positionController->getPosition();
	if (mousePosition.x >= position.x && mousePosition.x >= position.x + size.x &&
		mousePosition.y >= position.y && mousePosition.y >= position.x + size.y ) {
		return true;
	}

	return false;
}

void Window::display()
{
	if (!visible) return;

	const Point2D<float>& position = positionController->getPosition();
	glColor4fv(color.values);
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);

	float texWidth = texture->getWidth()/3.0f;
	float texHeight = texture->getHeight()/3.0f;


	bool isHorizontalBorder = size.x > texWidth*2;
	bool isVerticalBorder = size.y > texHeight*2;

	float horizontalCornerSize = texWidth;
	float verticalCornerSize = texHeight;

	float ratioW = 1.0f;
	float ratioH = 1.0f;
	
	if (!isHorizontalBorder) {
		horizontalCornerSize = size.x/2.0f;
		ratioW = horizontalCornerSize / texWidth;
	}
	
	if (!isVerticalBorder) {
		verticalCornerSize = size.y/2.0f;
		ratioH = verticalCornerSize / texHeight;
		if (ratioW > ratioH) {
			horizontalCornerSize = ratioH * texWidth;
			isHorizontalBorder = true;
		} else if (ratioW < ratioH){
			verticalCornerSize = ratioW * texHeight;
			isVerticalBorder = true;
		}
	} else if (!isHorizontalBorder) {
		verticalCornerSize = ratioW * texHeight;
		isVerticalBorder = true;
	}

	float horizontalBorderScale = isHorizontalBorder ? (size.x - 2*horizontalCornerSize) :  0.0f ;
	float verticalBorderScale = isVerticalBorder ? (size.y -  2*verticalCornerSize) :  0.0f ;

	float horizontalPos[3] = {0, horizontalCornerSize , size.x-horizontalCornerSize};
	float verticalPos[3] = {0, verticalCornerSize , size.y-verticalCornerSize};

	texture->drawSquare(Rectangle<float>(horizontalPos[0],verticalPos[0],
										horizontalPos[0]+horizontalCornerSize,
										verticalPos[0]+verticalCornerSize),
						texArea[0]);
	if (isHorizontalBorder) {
		texture->drawSquare(Rectangle<float>(horizontalPos[1], verticalPos[0],
											horizontalPos[1]+horizontalBorderScale,
											verticalPos[0]+verticalCornerSize),
							texArea[1]);
	}
		
	texture->drawSquare(Rectangle<float>(horizontalPos[2],verticalPos[0],
										horizontalPos[2]+horizontalCornerSize,
										verticalPos[0]+verticalCornerSize),
						texArea[2]);

	if (isVerticalBorder) {
		
		texture->drawSquare(Rectangle<float>(horizontalPos[0],verticalPos[1],
											horizontalPos[0]+horizontalCornerSize,
											verticalPos[1]+verticalBorderScale),
							texArea[3]);

		if (isHorizontalBorder) {
		
			texture->drawSquare(Rectangle<float>(horizontalPos[1],verticalPos[1],
												horizontalPos[1]+horizontalBorderScale,
												verticalPos[1]+verticalBorderScale),
								texArea[4]);
		}

		texture->drawSquare(Rectangle<float>(horizontalPos[2],verticalPos[1],
											horizontalPos[2]+horizontalCornerSize,
											verticalPos[1]+verticalBorderScale),
							texArea[5]);
	}


	texture->drawSquare(Rectangle<float>(horizontalPos[0],verticalPos[2],
										horizontalPos[0]+horizontalCornerSize,
										verticalPos[2]+verticalCornerSize),
						texArea[6]);

	if (isHorizontalBorder) {

		texture->drawSquare(Rectangle<float>(horizontalPos[1],verticalPos[2],
											horizontalPos[1]+horizontalBorderScale,
											verticalPos[2]+verticalCornerSize),
							texArea[7]);
	}

	texture->drawSquare(Rectangle<float>(horizontalPos[2],verticalPos[2],
										horizontalPos[2]+horizontalCornerSize,
										verticalPos[2]+verticalCornerSize),
						texArea[8]);

	if (!items.empty()) {

		for (list<Item*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
			Item* item = *ci;
			item->display();
		}
	}

	for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
		UIAction* action = *ci;
		if (action->isActive()) {
			action->display();
		}
	}
	glPopMatrix();
}