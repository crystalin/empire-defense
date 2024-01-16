#ifndef __GLUT_WRAPPER_H__
#define __GLUT_WRAPPER_H__

#include "InputParser.h"
#include "TimeManager.h"
#include "DisplayManager.h"


class GLUTWrapper
{
public:
	static void init(int argc, char** argv);
	static void start();

	static void addInputParser(InputParser* parser);
	static void removeInputParser(InputParser* parser);
	static void addTimeManager(TimeManager* timeManager);
	static void removeTimeManager(TimeManager* timeManager);

	static void setDisplayManager(DisplayManager* displayManager);

	static int	getTime();
	static bool	isShiftPressed();
	static bool	isAltPressed();
	static bool	isControlPressed();

private:
	GLUTWrapper() {}
	
	static void staticProcessKeyboard(unsigned char key, int x, int y);
	static void staticProcessSpecialKeyboard(int key, int x, int y);
	static void staticProcessMouse(int button, int state, int x, int y);
	static void staticProcessMouseMotion(int x, int y);
	static void staticIdle(int time);
	static void staticIdle(void);
	static void staticDisplay(void);
	static void staticResize(int w, int h);

	static list<InputParser*> parsers;
	static list<TimeManager*> timeManagers;
	static DisplayManager* displayManager;

	static int timeBase;
};

#endif