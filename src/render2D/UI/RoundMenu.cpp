#include "render2D_common.h"
#include "Texture.h" 
#include "RoundMenu.h" 
#include "LaserAnimation.h" 
#include "RessourceManager.h" 

RoundMenu::RoundMenu(PositionController* positionController) :
Menu(positionController), dirty(false), laserTexture(NULL), translateTime(0)
{
	laserTexture = RessourceManager::instance()->getTexture("laser.png");
	laserTexture->request();
}

RoundMenu::~RoundMenu() 
{
	laserTexture->release();
}

void RoundMenu::clear() {
	Container::clear();
	dirty = true;
}

void RoundMenu::addItem(Button* button) {
	Container::addItem(button);
	dirty = true;
}

void RoundMenu::calculPositions() {
	
	dirty = false;
	int count = items.size();
	float shareAngle = PI*2 / (float)count;
	float angle = 0;

	//cout << " position : " << position << endl;
	for (list<Button*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		Button* button = *ci;
		float ecart = button->getSize()*0.9 + 30;
		int cosEcart = cos(angle)*ecart;
		int sinEcart = sin(angle)*ecart;

		//cout << " cosEcart : " << cosEcart << endl << " sinEcart : " << sinEcart  << endl;
		button->setPosition(Point2D<float>(cosEcart,sinEcart));
		angle += shareAngle;
	}
}

void RoundMenu::update(int timeSpent)
{
	translateTime += timeSpent;
	if (translateTime>ROUND_MENU_LASER_TIME) {
		translateTime -= ROUND_MENU_LASER_TIME;
	}
	if (!visible) return;
	if (dirty) {
		calculPositions();
	}

	Item::update(timeSpent);
	for (list<Button*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
		Button* button = *ci;
		button->update(timeSpent);
	}
}

void RoundMenu::display()
{
	if (!visible) return;

	const Point2D<float>& position = positionController->getPosition();
	Point2D<float> previousPosition;

	if (!items.empty()) {
		glPushMatrix();
		glTranslatef(position.x, position.y, 0);

		/*for (list<Animation*>::iterator ci = linkAnimations.begin(); ci != linkAnimations.end(); ++ci) {
			(*ci)->display();
		}*/

		int itemsCount = items.size();
		if (itemsCount > 1) {
			float decalage = translateTime / (float)ROUND_MENU_LASER_TIME;
			previousPosition = (items.back())->getPosition();
			for (list<Button*>::iterator ci = items.begin(); ci != items.end(); ++ci) {
			
				Button* button = *ci;
				const Point2D<float>& currentPosition = button->getPosition();
				glColor4f(0.1f, 1.0f, 0.1f, 1.0f);
				//glColor3fv(button->getColor().values);
				glPushMatrix();
				float distance = previousPosition.distance(currentPosition);
				glTranslatef(previousPosition.x, previousPosition.y, 0.0f);
				glRotatef(previousPosition.angle(currentPosition),0,0,1);
				glTranslatef( decalage * distance, 0.0f, 0.0f);
				float distanceDone = decalage * distance;
				float size = min(distance-distanceDone,min(distanceDone,48.0f));
				float halfSize = size;
				laserTexture->drawSquare(Rectangle<float>(-halfSize, -16.0f, halfSize, 16.0f));
				glPopMatrix();
				previousPosition = currentPosition;
				decalage += 1.0f/(float)itemsCount;
				if (decalage > 1.0f) {
					decalage -= 1.0f;
				}
			}
		}
		for_each(items.begin(), items.end(),mem_fun(&Item::display));


		for (list<UIAction*>::iterator ci = actions.begin(); ci != actions.end(); ++ci) {
			UIAction* action = *ci;
			if (action->isActive()) {
				action->display();
			}
		}
		glPopMatrix();
	}
}