#ifndef __INPUT_PARSER_H__
#define __INPUT_PARSER_H__


#define KEY_F1			1
#define KEY_F2			2
#define KEY_F3			3
#define KEY_F4			4
#define KEY_F5			5
#define KEY_F6			6
#define KEY_F7			7
#define KEY_F8			8
#define KEY_F9			9
#define KEY_F10			10
#define KEY_F11			11
#define KEY_F12			12
	
#define KEY_ESC			27

#define KEY_LEFT		20
#define KEY_UP			21
#define KEY_RIGHT		22
#define KEY_DOWN		23
#define KEY_PAGE_UP		24
#define KEY_PAGE_DOWN	25
#define KEY_HOME		26
#define KEY_END			27
#define KEY_INSERT		28

class InputParser abstract
{
public:
	virtual void processKeyboard(unsigned char key, int x, int y) {}
	virtual void processMouse(int button, int state, int x, int y) {}
	virtual void processMouseMotion(int x, int y) {}
};

#endif
