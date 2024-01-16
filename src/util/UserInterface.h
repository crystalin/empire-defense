#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include "Observer.h"
#include "DisplayManager.h"

class Case;
class Controller;

#define ZOOM_MORE 1
#define ZOOM_LESS 2

#define MOVE_LEFT 1
#define MOVE_RIGHT 2
#define MOVE_UP 3
#define MOVE_DOWN 4

#define MENU_TOWER 1
#define MENU_EFFECT 2
#define MENU_MAIN 3
#define MENU_COMMAND 4
#define MENU_MAP 5

class UserInterface : public Observer, public DisplayManager
{
public:
	virtual ~UserInterface() {};

	virtual void		requestDisplayGrid() = 0;
	virtual void		requestReloadShaders() = 0;
	virtual void		requestReloadColors() = 0;
	virtual void		requestFullscreenChange() = 0;
	virtual void		requestZoomChange(int zoomType) = 0;
	virtual void		requestMoveMap(int direction) = 0;
	virtual const Case&	requestSelectedCase() = 0;

	virtual bool		isMenuOpen(int menuType) = 0;

	virtual void		requestOpenMenu(int menuType, Object* object = NULL) = 0;
	virtual void		requestCloseMenu(int menuType) = 0;

	virtual void		setController(Controller* controller) {}
	virtual void		update(int timeSpent) {}
	
};

#endif
