
#include "controller_common.h"
#include "GLUTWrapper.h"
#include "glut.h"

list<InputParser*> GLUTWrapper::parsers = list<InputParser*>();
list<TimeManager*> GLUTWrapper::timeManagers = list<TimeManager*>();
DisplayManager* GLUTWrapper::displayManager = NULL;
int GLUTWrapper::timeBase = NULL;


void GLUTWrapper::init(int argc, char** argv)
{
	glutInit(&argc, argv);

	if (displayManager) {
		displayManager->initDisplay();
	}

#ifndef PERFORMANCE_PROFILING
	glutKeyboardFunc(staticProcessKeyboard);
	glutSpecialFunc(staticProcessSpecialKeyboard);
	glutMouseFunc(staticProcessMouse);
	glutMotionFunc(staticProcessMouseMotion);
	glutPassiveMotionFunc(staticProcessMouseMotion);
#endif

#ifdef NO_FPS_LIMIT
	glutTimerFunc(10,staticIdle,10);
#else
	glutIdleFunc(staticIdle);
#endif

	glutDisplayFunc(staticDisplay);
	glutReshapeFunc(staticResize);

}

void GLUTWrapper::start() {
	timeBase = glutGet(GLUT_ELAPSED_TIME);

#ifdef PERFORMANCE_PROFILING
	while(1) {
		staticIdle(3);
	}
#else
	glutMainLoop();
#endif

}

bool GLUTWrapper::isShiftPressed() {
	return (glutGetModifiers() == GLUT_ACTIVE_SHIFT);
}
bool GLUTWrapper::isAltPressed() {
	return (glutGetModifiers() == GLUT_ACTIVE_ALT);
}
bool GLUTWrapper::isControlPressed() {
	return (glutGetModifiers() == GLUT_ACTIVE_CTRL);
}

int GLUTWrapper::getTime() {
	return glutGet(GLUT_ELAPSED_TIME);
}

void GLUTWrapper::staticProcessKeyboard(unsigned char key, int x, int y)
{
	for (list<InputParser*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
		(*it)->processKeyboard(key, x, y);
	}
}

void GLUTWrapper::staticProcessSpecialKeyboard(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_F1: key = KEY_F1; break;
		case GLUT_KEY_F2: key = KEY_F2; break;
		case GLUT_KEY_F3: key = KEY_F3; break;
		case GLUT_KEY_F4: key = KEY_F4; break;
		case GLUT_KEY_F5: key = KEY_F5; break;
		case GLUT_KEY_F6: key = KEY_F6; break;
		case GLUT_KEY_F7: key = KEY_F7; break;
		case GLUT_KEY_F8: key = KEY_F8; break;
		case GLUT_KEY_F9: key = KEY_F9; break;
		case GLUT_KEY_F10: key = KEY_F10; break;
		case GLUT_KEY_F11: key = KEY_F11; break;
		case GLUT_KEY_F12: key = KEY_F12; break;

		case GLUT_KEY_LEFT: key = KEY_LEFT; break;
		case GLUT_KEY_UP: key = KEY_UP; break;
		case GLUT_KEY_RIGHT: key = KEY_RIGHT; break;
		case GLUT_KEY_DOWN: key = KEY_DOWN; break;
		case GLUT_KEY_PAGE_UP: key = KEY_PAGE_UP; break;
		case GLUT_KEY_PAGE_DOWN: key = KEY_PAGE_DOWN; break;
		case GLUT_KEY_HOME: key = KEY_HOME; break;
		case GLUT_KEY_END: key = KEY_END; break;
		case GLUT_KEY_INSERT: key = KEY_INSERT; break;
	}
	
	for (list<InputParser*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
		(*it)->processKeyboard(key, x, y);
	}
}

void GLUTWrapper::staticProcessMouse(int button, int state, int x, int y)
{
	for (list<InputParser*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
		(*it)->processMouse(button, state, x, y);
	}
}

void GLUTWrapper::staticProcessMouseMotion(int x, int y)
{
	for (list<InputParser*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
		(*it)->processMouseMotion(x, y);
	}
}

void GLUTWrapper::staticDisplay(void) 
{
	displayManager->display();
}

void GLUTWrapper::staticResize(int w, int h) 
{
	displayManager->resize(w, h);
}

void GLUTWrapper::staticIdle(int time) 
{
	int timeSpent;

#ifdef PERFORMANCE_PROFILING
	int elapsedTime=timeBase + time;
#else
	int elapsedTime=glutGet(GLUT_ELAPSED_TIME);
#endif

	timeSpent = (elapsedTime - timeBase);
	if (timeSpent == 0) {
		return;
	}
	timeBase = elapsedTime;

	for (list<TimeManager*>::iterator it = timeManagers.begin(); it != timeManagers.end(); ++it) {
		(*it)->idle(timeSpent);
	}

#ifndef PERFORMANCE_PROFILING
	glutPostRedisplay();
	glutTimerFunc(time,staticIdle,time);
#endif

}

void GLUTWrapper::staticIdle(void) 
{
	int timeSpent;

	int elapsedTime=glutGet(GLUT_ELAPSED_TIME);
	timeSpent = (elapsedTime - timeBase);
	if (timeSpent == 0) {
		return;
	}
	timeBase = elapsedTime;

	for (list<TimeManager*>::iterator it = timeManagers.begin(); it != timeManagers.end(); ++it) {
		(*it)->idle(timeSpent);
	}
	glutPostRedisplay();
}
 
void GLUTWrapper::addInputParser(InputParser* parser)
{
	parsers.push_back(parser);
}

void GLUTWrapper::removeInputParser(InputParser* parser)
{
	parsers.remove(parser);
}
 
void GLUTWrapper::addTimeManager(TimeManager* timeManager)
{
	timeManagers.push_back(timeManager);
}

void GLUTWrapper::removeTimeManager(TimeManager* timeManager)
{
	timeManagers.remove(timeManager);
}

void GLUTWrapper::setDisplayManager(DisplayManager* displayManager)
{
	GLUTWrapper::displayManager = displayManager;
}