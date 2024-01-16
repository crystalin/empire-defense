#ifndef __GRAPHIC_ENGINE__
#define __GRAPHIC_ENGINE__

#include "InputParser.h" 
#include "UserInterface.h" 
#include "UIObservable.h" 
#include "Statistics.h" 


class Controller;
class MessageManager;
class MapCatalog;
class Statistics;

class Menu;
class Button;
class Texture;
class Window;
class Viewport;

class EDVIEW_API GraphicEngine : public InputParser, public UserInterface, public UIObservable
{
public:
	GraphicEngine();
	~GraphicEngine(void);

	void update(int time);

	void setFullscreenMode(bool fullscreen);
	bool getFullscreenMode();

	void setController(Controller *controller);

	void notifyDeadUnit(const Unit* dead);
	void notifyFireAmmo(const Ammo* ammo);
	void notifyBuildTower(const Tower* tower);
	void notifyChooseTowerConstruction(const TowerTemplate* towerTemplate);
	void notifyResetGame();
	void notifyChangeMap(const Map* map);
	void notifyMessage(const string& message);
	void notifySelectObject(const Object* object);
	void notifyAmmoHit(const Ammo* ammo);
	void notifyCreateParty(Party* party);
	void notifyDestroyParty(Party* party);
	
	void notifyLoadMapTemplates(const vector<const MapTemplate*>& mapTemplates);
	void notifyLoadTowerTemplates(const vector<const TowerTemplate*>& towerTemplates);
	void notifyLoadUnitTemplates(const vector<const UnitTemplate*>& unitTemplates);
	void notifyLoadAmmoTemplates(const vector<const AmmoTemplate*>& ammoTemplates);
	void notifyLoadEffectTemplates(const vector<const EffectTemplate*>& effectTemplates);
	
	void requestDisplayGrid();
	void requestFullscreenChange();
	void requestZoomChange(int zoomType);
	void requestMoveMap(int direction);
	void requestOpenMenu(int menuType, Object* object = NULL);
	void requestCloseMenu(int menuType);
	void requestReloadShaders();
	void requestReloadColors();
	const Case& requestSelectedCase();

	
	bool isMenuOpen(int menuType);

	void processMouse(int button, int state, int x, int y);
	void processMouseMotion(int x, int y);

	void display(void);
	void resize(int w, int h);
	void initDisplay(void);

protected:

	bool isInit;

	// Model
	Party* currentParty;
	Controller* controller;

	Rectangle<int> area;
	bool fullscreenMode;

	MessageManager* messageManager;

	Statistics statistics;

	Menu* mainMenu;
	Menu* buildMenu;
	Menu* towerMenu;
	Menu* effectMenu;
	Menu* mapMenu;

	vector<Button*> basicEffectButtons;
	Button* towerButtons[100];
	Button* effectButtons[100];

	// TEST
	Window* window;
	Viewport* viewport;
	Texture* heartTexture;
	Texture* moneyTexture;
	Texture* backgroundTexture;

	Point2D<float> mousePosition;
	Point2D<float> clickPosition;
	int mouseButton;
	int mouseState;
	bool allowedToMove;
	bool moving;

};

#endif
