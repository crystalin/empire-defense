#include "controller.h"
#include "GraphicEngine.h"

Controller* Controller::mainController=NULL;


Controller::Controller(Model* model, Observer* observer): model(model), observer(observer),
	mouseDown(false), xrot(0.0f), yrot(0.0f), xdiff(0.0f), ydiff(0.0f), selectedTower(NULL), selectedCase(-1)
{
	if (mainController == NULL) {
		mainController = this;
		glutKeyboardFunc(staticProcessKeyboard);
		glutSpecialFunc(staticProcessSpecialKeyboard);
		glutMouseFunc(staticProcessMouse);
		glutMotionFunc(staticProcessMouseMotion);
		glutPassiveMotionFunc(staticProcessMouseMotion);
		glutIdleFunc(staticIdle);
	}
	observer->setController(this);
}

Controller::~Controller()
{
	if (mainController == this) {
		mainController = NULL;
	}
}

void Controller::staticProcessKeyboard(unsigned char key, int x, int y)
{
	if (mainController!=NULL) {
		mainController->processKeyboard(key, x, y);
	}
}

void Controller::staticProcessSpecialKeyboard(int key, int x, int y)
{
	if (mainController!=NULL) {
		mainController->processSpecialKeyboard(key, x, y);
	}
}

void Controller::staticProcessMouse(int button, int state, int x, int y)
{
	if (mainController!=NULL) {
		mainController->processMouse(button, state, x, y);
	}
}

void Controller::staticProcessMouseMotion(int x, int y)
{
	if (mainController!=NULL) {
		mainController->processMouseMotion(x, y);
	}
}

void Controller::staticIdle(void) {
	if (mainController!=NULL) {
		mainController->idle();
	}
}

void Controller::processKeyboard(unsigned char key, int x, int y)
{
	if (key == 27) {
		delete graphicEngine;
		delete model;
		delete this;
		exit(1);
	}
	graphicEngine->processKeyboard(key, x, y);
}

void Controller::processSpecialKeyboard(int key, int x, int y)
{
	graphicEngine->processSpecialKeyboard(key, x, y);
}

void Controller::processMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	} else
		mouseDown = false;
	
	graphicEngine->processMouse(button, state, x, y);
}

void Controller::processMouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

	}
	graphicEngine->processMouseMotion(x, y);
}

void Controller::processCommand(int ID)
{
	switch (ID) {
		case COMMAND_ADD_EFFECT_ICE:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,1);
			}
			break;
		case COMMAND_ADD_EFFECT_FIRE:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,2);
			}
			break;
		case COMMAND_ADD_EFFECT_EARTH:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,3);
			}
			break;
		case COMMAND_ADD_EFFECT_POISON:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,4);
			}
			break;
		case COMMAND_ADD_EFFECT_GOLD:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,5);
			}
			break;
		case COMMAND_ADD_EFFECT_LIGHT:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,6);
			}
			break;
		case COMMAND_ADD_EFFECT_MALEDICTION:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,7);
			}
			break;
		case COMMAND_ADD_EFFECT_ELECTRICITY:
			if (selectedTower != NULL) {
				model->addTowerEffect(selectedTower,8);
			}
			break;
		case COMMAND_BUILD_TOWER_LASER:
			if (selectedCase == -1) return;
			model->buildTower(WEAPON_LASER,selectedCase%model->getMap()->getX(),selectedCase/model->getMap()->getX());
			break;
		case COMMAND_BUILD_TOWER_MISSILE:
			if (selectedCase == -1) return;
			model->buildTower(WEAPON_MISSILE,selectedCase%model->getMap()->getX(),selectedCase/model->getMap()->getX());
			break;
	}
}

void Controller::start(void) {
	
	time=glutGet(GLUT_ELAPSED_TIME);
	timebase=time;
	glutMainLoop();
}


void Controller::idle()
{
	float timeSpent;

	time=glutGet(GLUT_ELAPSED_TIME);
	timeSpent = (time - timebase);
	timebase = time;

	graphicEngine->update(timeSpent);
	model->update(timeSpent);

	glutPostRedisplay();
}

void Controller::setSelectedCase(int index) 
{
	selectedCase = index;
	if (index == -1) {
		graphicEngine->notifyUnselectCase();
	} else {
		graphicEngine->notifySelectCase(index);
	}
}