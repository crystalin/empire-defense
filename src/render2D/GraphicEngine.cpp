#include "render2D_common.h"
#include "GraphicEngine.h" 
#include "Controller.h"
#include "Window.h"
#include "MessageManager.h"
#include "Menu.h"
#include "RoundMenu.h"
#include "Effect.h"
#include "Tower.h"
#include "Ammo.h"
#include "Unit.h"
#include "Map.h"
#include "Button.h"
#include "Text.h"
#include "Image.h"
#include "Shader.h"
#include "PopupAction.h"
#include "MapCatalog.h"
#include "ConfigReader.h"
#include "RessourceManager.h"
#include "Viewport.h"
#include "PlayerViewer.h"
#include "UnitViewer.h"
#include "TowerViewer.h"
#include "ProjectileViewer.h"
#include "FontManager.h"
#include "ColorManager.h"
#include "Icons.h"
#include <iomanip>

GraphicEngine::GraphicEngine(): area(0,0,800,480),
fullscreenMode(false), statistics(), controller(NULL), mainMenu(NULL), buildMenu(NULL), currentParty(NULL), viewport(NULL), 
effectMenu(NULL), towerMenu(NULL) , isInit(false),  mouseButton(0), mouseState(0), messageManager(NULL), mapMenu(NULL),
heartTexture(NULL), moneyTexture(NULL), backgroundTexture(NULL), allowedToMove(true), moving(false), clickPosition(0,0), mousePosition(0,0)
{
	fill(effectButtons, effectButtons + 100, (Button*)NULL);
	fill(towerButtons, towerButtons + 100, (Button*)NULL);
	Item::uiObservable = this;
}

GraphicEngine::~GraphicEngine(void)
{
	// TEST
	if (!isInit) {
		return;
	}
	delete messageManager;
	backgroundTexture->release();
	moneyTexture->release();
	heartTexture->release();
	Animation::clearAnimations();
	Item::clearGlobalItems();
	window->release();
	if (viewport != NULL) {
		delete viewport;
	}
	
	for_each(basicEffectButtons.begin(), basicEffectButtons.end(), bind(&Button::release, _1));
	for_each(effectButtons, effectButtons + 100, if_then(_1 != (Button*)NULL, bind(&Button::release, _1)));
	for_each(towerButtons, towerButtons+ 100, if_then(_1 != (Button*)NULL, bind(&Button::release, _1)));

	PlayerViewer::destroy();
	UnitViewer::destroy();
	TowerViewer::destroy();
	ProjectileViewer::destroy();
	FontManager::destroy();
	ColorManager::destroy();
	Icons::deleteTextures();
	delete RessourceManager::instance(); // must be the last
}

void GraphicEngine::setFullscreenMode(bool fullscreen)
{
	static bool initScreen = false;
	fullscreenMode = fullscreen;

	//cout << "Mode FullScreen" << endl;
	if(fullscreen) {
		glutFullScreen();
	} else {
		if (!initScreen) {
			glutInitWindowPosition(50, 50);
			glutInitWindowSize(800, 480);
			initScreen=true;
		} else {
			glutReshapeWindow(800, 480);
			glutPositionWindow(50, 50);
		}
	}
}

bool GraphicEngine::getFullscreenMode()
{
	return fullscreenMode;
}

void GraphicEngine::update(int time) {
	statistics.update(time);

	// TEST
	Animation::updateAnimations(time);
	Item::updateGlobalItems(time);
	window->update(time);

	if (viewport != NULL) {
		viewport->update(time);
	}
	messageManager->update(time);
	/*while (animations.size() < 200 ) {
		animations.push_back(new LaserAnimation(mousePosition,Point2D<float>(myRand(0,area.x2),myRand(0,area.y2)), 1000, 0, Rectangle<float>(0,0,myRand(0,32)+48,myRand(-4,16)+16), myRand(0.2,1), Color(myRand(0.3,0.6),myRand(0.3,0.6),myRand(0.7,1),myRand(0,1))));
	}*/
}

void GraphicEngine::initDisplay(void)
{
	isInit = true;

	setFullscreenMode(fullscreenMode);

#ifdef NO_FPS_LIMIT
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_ALPHA);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);
#endif

	glutCreateWindow(TITLE);
	
	GLenum err = glewInit();
	TEST_FOR_EXCEPTION(GLEW_OK != err, "GLEW is not initialized! : [" << glewGetErrorString(err) << "]");
	TEST_FOR_EXCEPTION(!GLEW_VERSION_2_0, "OpenGL 2.0 is not supported!");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable (GL_BLEND); 
	glDisable (GL_DEPTH_TEST); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	RessourceManager::init("../ressources/");
	FontManager::init(ConfigReader("../../data/config/default.txt"));
	messageManager = new MessageManager(Point2D<float>(100,100));
	Icons::initTextures();
	ProjectileViewer::init();
	TowerViewer::init();
	UnitViewer::init();
	PlayerViewer::init();
	ColorManager::init(ConfigReader("../../data/config/colors.txt"));

	mainMenu = new RoundMenu(new DefaultPositionController(Point2D<float>(area.x2 / 2.0f,area.y2 / 2.0f)));
	effectMenu = new RoundMenu(new DefaultPositionController(Point2D<float>(100,area.y2 -300)));
	
	// TEST
	//roundAnimation = new ParticleAnimation(100,30,1000,1);
	buildMenu = new Menu(new DefaultPositionController(Point2D<float>(48,area.y2-48)), MENU_VERTICAL);
	towerMenu = new Menu(new DefaultPositionController(Point2D<float>(100,area.y2-150)), MENU_HORIZONTAL);
	towerMenu->setVisible(false);
	buildMenu->setVisible(false);

	Button* button = NULL;

	button = new Button(COMMAND_PAUSE,48,new Text("Menu",Color(1.0f, 0.8f, 0.6f),"normal",0.6,Font::middle),Color(0.3f, 0.0f, 0.0f, 1.0f));
	buildMenu->addItem(button);
	button = new Button(COMMAND_BUILD,48,new Image(new TextureAnimation(RessourceManager::instance()->getTexture("build-icon.png"), 0.0f , 1000, 1, Rectangle<float>(-16,-16,16,16))),Color(0.3f, 0.0f, 0.0f, 1.0f));
	button->addAction(new PopupAction(new Text("Build a tower",Color(),"normal",1.0f, Font::left,new DefaultPositionController(Point2D<float>(-24.0f,48)))));
	button->setPosition(Point2D<float>(50,area.y2-150));
	
	//Window* popup = new Window(Point2D<float>(-30,-30), Point2D<float>(100,20));
	//popup->addItem(new Text("Build a tower",Point2D<float>(100,20)));

	//Item::addGlobalItem(button);
	buildMenu->addItem(button);

	button = new Button(COMMAND_NONE,128,new Text("About",Color(1.0f, 1.0f, 1.0f),"big_menu",1,Font::middle),Color(0.3f, 0.8f, 0.6f, 1.0f));
	//button = new Button(COMMAND_START,128,new Text("About",Point2D<float>(128,128),Color(0.8f, 0.8f, 0.6f),1.4,Point2D<float>(-45,-30)),Color(0.4f, 0.4f, 0.4f, 1.0f));
	mainMenu->addItem(button);
	//button = new Button(COMMAND_NONE,128,new Image(new TextureAnimation(RessourceManager::instance()->getTexture("option.png"), 0.0f , 1000, 1, Rectangle<float>(-32,-32,32,32))),Color(0.0f, 0.0f, 1.0f, 1.0f));
	button = new Button(COMMAND_NONE,128,new Text("Option",Color(1.0f, 1.0f, 1.0f),"big_menu",1,Font::middle),Color(0.7f, 0.2f, 1.0f, 1.0f),new FloatingPositionController<>(Point2D<float>(0.0f, 0.0f)));
	mainMenu->addItem(button);
	button = new Button(COMMAND_START,128,new Text("START",Color(1.0f, 1.0f, 1.0f),"big_menu",1,Font::middle),Color(0.0f, 1.0f, 0.0f, 1.0f));
	mainMenu->addItem(button);
	button = new Button(COMMAND_RESUME,128,new Text("Resume",Color(1.0f, 1.0f, 1.0f),"big_menu",1,Font::middle),Color(1.0f, 0.7f, 0.2f, 1.0f));
	mainMenu->addItem(button);
	button = new Button(COMMAND_EXIT,128,new Text("EXIT",Color(1.0f, 1.0f, 1.0f),"big_menu",1,Font::middle),Color(1.0f, 0.0f, 0.0f, 1.0f));
	mainMenu->addItem(button);

	window = new Window(new DefaultPositionController(Point2D<float>(200,100)),Point2D<float>(area.x2-300,area.y2-200));
	window->request();

	Item::addGlobalItem(mainMenu);
	Item::addGlobalItem(towerMenu);
	Item::addGlobalItem(buildMenu);
	Item::addGlobalItem(effectMenu);
	

	mouseState = GLUT_UP;

	int i=0;
	heartTexture = RessourceManager::instance()->getTexture("heart.png");
	heartTexture->request();
	moneyTexture = RessourceManager::instance()->getTexture("bourse.png");
	moneyTexture->request();
	backgroundTexture = RessourceManager::instance()->getTexture("heart.png");
	backgroundTexture->request();
	
}

void GraphicEngine::setController(Controller *controller) { 
	this->controller = controller;
	Button::controller = controller;
	controller->addInputParser(this);
}


void GraphicEngine::resize(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	area.x2 = w;
	area.y2 = h;
	gluOrtho2D(0, area.x2, 0, area.y2);
	glViewport(0,0,area.x2, area.y2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	if (viewport) {
		viewport->setArea(area);
	}
	window->setPosition(Point2D<float>(area.getOrigin()));
	window->setSize(Point2D<float>(area.getSize()));
	buildMenu->setPosition(Point2D<float>(48,area.y2-48));
	towerMenu->setPosition(Point2D<float>(100,area.y2-150));
	//effectMenu->setPosition(Point2D<float>(100,area.y2-300));
	mainMenu->setPosition(Point2D<float>(area.x2 / 2.0f,area.y2 / 2.0f));
	if (mapMenu) {
		mapMenu->setPosition(Point2D<float>(area.x2 / 2.0f,area.y2 / 2.0f));
	}
}

void GraphicEngine::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glTranslatef(0.5, 0.5, 0.0);

	/*if (!model->isStarted()) {
		backgroundTexture->drawSquare(area);
	}*/
	// TEST 
	if (viewport) {
		viewport->display();
		window->display();
	}

	Item::displayGlobalItems();
	Animation::displayAnimations();

	std::ostringstream oss;

	const Font* font = FontManager::getFont("normal");

	if (currentParty != NULL && currentParty->getState() >= Party::Ingame && font != NULL) {

		const Player* player = controller->getPlayer();
		glPushMatrix();
		glTranslatef(150,area.y2 - 50,0.0f);
		PlayerViewer::display(player);

		glColor3f(1.0f, 1.0f, 0.6f);
		font->renderString(0,30, player->getName(),Font::middle);

		oss.str("");
		oss << player->getExperience() << " xp";
		font->renderString(0, 10,oss.str(),Font::middle);


		oss.str("");
		oss  << player->getPartyMoney();
		moneyTexture->drawSquare(Rectangle<float>(-16,-43, 24, -5));
		font->renderString(16,-20,oss.str());
		glPopMatrix();
		
		
		oss.str("");
		oss << "Lvl " << currentParty->getCurrentRound();
		glColor3f(0.2f, 0.8f, 0.8f);
		font->renderString(400,area.y2 - 20,oss.str());
		
		oss.str("");
		oss <<  currentParty->getLives();
		glColor3f(1.0f, 1.0f, 1.0f);
		heartTexture->drawSquare(Rectangle<float>(476,area.y2-48, 524, area.y2));
		font->renderString(500,area.y2 - 20,oss.str());
		
		oss.str("");
		oss << "Zoom : " << setprecision(2) << viewport->getZoom();
		glColor3f(0.8f, 0.7f, 0.5f);
		font->renderString(600,area.y2 - 20,oss.str());

		oss.str("");
		oss << "FPS " << fixed << setprecision(2) << statistics.getFPS();
		glColor3f(0.0f,1.0f,1.0f);
		font->renderString(600,area.y2 - 45,oss.str());
		
		oss.str("");
		oss << "Smoke : "  << viewport->smokeGenerator->getCount();
		glColor3f(0.8f, 0.4f, 0.5f);
		font->renderString(600,area.y2 - 70,oss.str());

		glPushMatrix();
		glTranslatef(area.x2-15,area.y2-200,0.0f);
		PlayerViewer::displayTeam(currentParty->getPlayers());
		glPopMatrix();

	} else {
	}

	//glColor3f(1.0f, 1.0f, 1.0f);
	//RessourceManager::instance()->getTexture("elfe3.png")->drawSquare(Rectangle<float>(area.x2-128, area.y2-192, area.x2, area.y2));


	messageManager->display();
	glFlush();
	glutSwapBuffers();
}

void GraphicEngine::notifyDeadUnit(const Unit* dead) {
	//bonusList.push_back(new Bonus(dead->getPosition(),money,BONUS_WIN_GOLD));
}

void GraphicEngine::notifyFireAmmo(const Ammo* ammo) {
	viewport->notifyFireAmmo(ammo);
}
void GraphicEngine::notifyAmmoHit(const Ammo* ammo) {
	viewport->notifyAmmoHit(ammo);
}

void GraphicEngine::notifyBuildTower(const Tower* tower) {
	viewport->notifyBuildTower(tower);
	//bonusList.push_back(new Bonus(tower->getPosition(),money,BONUS_LOSE_GOLD));
}

void GraphicEngine::notifyChooseTowerConstruction(const TowerTemplate* towerTemplate) {
	viewport->notifyChooseTowerConstruction(towerTemplate);
}

void GraphicEngine::notifyCreateParty(Party* party) {
	if (currentParty != NULL) {
		cout << "[Engine Recreate Party]" << endl;
		currentParty->removeObserver(this);
		if (viewport) {
			delete viewport;
		}
	}
	currentParty = party;
	currentParty->addObserver(this);
	viewport = new Viewport(*party,Rectangle<float>(0,0,area.x2,area.y2));
}

void GraphicEngine::notifyDestroyParty(Party* party) {
	cout << "[Engine Destroy Party]" << endl;
	if (party != currentParty) {
		party->removeObserver(this);
		return;
	}

	if (viewport) {
		delete viewport;
	}
	currentParty = NULL;
}

void GraphicEngine::notifyChangeMap(const Map* map) {

	viewport->notifyChangeMap(map);
	messageManager->addMessage("New Map : " + map->getName());
}

void GraphicEngine::notifyMessage(const string& message) {
	messageManager->addMessage(message);
}

void GraphicEngine::notifyResetGame()
{
	notifySelectObject(NULL);
}

void GraphicEngine::notifySelectObject(const Object* object)
{
	viewport->notifySelectObject(object);
}

void GraphicEngine::notifyLoadMapTemplates(const vector<const MapTemplate*>& mapTemplates) {
	mapMenu = new MapCatalog(mapTemplates,Point2D<float>(area.x1,area.y2 / 2.0f));
	mapMenu->setVisible(false);
	Item::addGlobalItem(mapMenu);
}

void GraphicEngine::notifyLoadTowerTemplates(const vector<const TowerTemplate*>& towerTemplates) {
	//cout << "Tower Template Count : " << towerTemplates.size() << endl;
	boost::format stringFormat("%1%");
	for (vector<const TowerTemplate*>::const_iterator it = towerTemplates.begin(); it != towerTemplates.end() ; ++it) {
		const TowerTemplate* towerTemplate = (*it);
		Image* content = NULL;

		string picture = "tower-lvl1.png";
		content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture(picture), 0.0f , 1000, 1, Rectangle<float>(-16,-16,16,16)));
		string text = "[" + towerTemplate->name + "]\n" + (stringFormat % towerTemplate->cost).str() + "g, " + (stringFormat % (towerTemplate->speed/1000.0f)).str() + "s, " + (stringFormat % towerTemplate->visibility).str() + " dist, " + (stringFormat % towerTemplate->ammoTemplate.damage).str() + " damage" ;
		towerButtons[towerTemplate->id] = new Button(TOWER_COMMAND + towerTemplate->id,40,content,Color(0.0f,0.2f,0.0f,1.0f));
		towerButtons[towerTemplate->id]->addAction(new PopupAction(new Text(text,Color(1,1,1,1.0f),"normal",0.6f,Font::middle,new DefaultPositionController(Point2D<float>(-15.0f, 50.0f)))));
		towerButtons[towerTemplate->id]->request();
		if (towerTemplate->parentId == 0) {
			towerMenu->addItem(towerButtons[towerTemplate->id]);
		}
	}
}

void GraphicEngine::notifyLoadUnitTemplates(const vector<const UnitTemplate*>& unitTemplates) {
}

void GraphicEngine::notifyLoadAmmoTemplates(const vector<const AmmoTemplate*>& ammoTemplates) {
}

void GraphicEngine::notifyLoadEffectTemplates(const vector<const EffectTemplate*>& effectTemplates) {
	//cout << "Effect Template Count : " << effectTemplates.size() << endl;

	boost::format stringFormat("%1%");
	int basicCount  = 0;
	for (vector<const EffectTemplate*>::const_iterator it = effectTemplates.begin(); it != effectTemplates.end() ; ++it) {
		const EffectTemplate* effectTemplate = (*it);
		Image* content = NULL;
		switch (effectTemplate->type) {
			case EFFECT_FIRE : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("fire.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_ICE : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("ice.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_EARTH : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("earth.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_POISON : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("poison.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_GOLD : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("gold.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_LIGHT : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("light.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_MALEDICTION : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("malediction.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
			case EFFECT_ELECTRICITY : content = new Image(new TextureAnimation(RessourceManager::instance()->getTexture("electricity.png"), 0.0f , 1000, 1, Rectangle<float>(-14,-14,14,14))); break;
		}
		string text = "[" + effectTemplate->name + "]\n" + (stringFormat % effectTemplate->cost).str() + "g - " + (stringFormat % (effectTemplate->time/1000.0f)).str() + "s - value = " + (stringFormat % effectTemplate->value).str();
		effectButtons[effectTemplate->id] = new Button(EFFECT_COMMAND + effectTemplate->id,40,content);
		effectButtons[effectTemplate->id]->addAction(new PopupAction(new Text(text,Color(1,1,1,1.0f),"normal",1,Font::left,new DefaultPositionController(Point2D<float>(-5.0f, 50.0f)))));
		effectButtons[effectTemplate->id]->request();
		if (effectTemplate->parentId == 0) {
			effectButtons[effectTemplate->id]->request();
			basicEffectButtons.push_back(effectButtons[effectTemplate->id]);
		}
	}
}

void GraphicEngine::requestDisplayGrid()
{
	cout << "request display grid" << endl;
	viewport->displayGrid= !viewport->displayGrid;
}

void GraphicEngine::requestFullscreenChange()
{
	setFullscreenMode(!getFullscreenMode());
}

void GraphicEngine::requestMoveMap(int direction)
{
	Point2D<float> movement(0,0);
	switch (direction) {
		case MOVE_UP : movement.y = 0.3; break;
		case MOVE_RIGHT : movement.x = 0.3; break;
		case MOVE_DOWN : movement.y = -0.3; break;
		case MOVE_LEFT : movement.x = -0.3; break;
					   
	}
	viewport->moveMapCoordinate(movement);
}

void GraphicEngine::requestOpenMenu(int menuType, Object* object)
{
	//cout << "Request open Menu : " << menuType << endl;
	switch (menuType) {
		case MENU_COMMAND : 
			buildMenu->setVisible(true);
			break;
		case MENU_MAIN :
			mainMenu->setVisible(true);
			break;
		case MENU_MAP : 
			if (mapMenu) {
				mapMenu->setVisible(true);
			}
			break;
		case MENU_TOWER : 
			towerMenu->setVisible(true);
			effectMenu->setVisible(false);

			break;
		case MENU_EFFECT :
			if (object != NULL && object->getType() == TYPE_TOWER) {
				//cout << " time-in : " << glutGet(GLUT_ELAPSED_TIME) << endl;
				Tower* tower = (Tower*) object;
				int money = controller->getPlayer()->getPartyMoney();
				const EffectTemplate* effectTemplate = tower->getEffectTemplate();
				effectMenu->clear();
				effectMenu->setPosition(viewport->getScreenPosition(object->getPosition()+0.5f));
				if (effectTemplate == NULL) {
					for_each(basicEffectButtons.begin(), basicEffectButtons.end(), bind(&Menu::addItem,effectMenu,_1));
				} else {
					const vector<const EffectTemplate*>& effects = effectTemplate->subTemplates;
					for (vector<const EffectTemplate*>::const_iterator it = effects.begin(); it != effects.end() ; ++it) {
						const EffectTemplate* subEffectTemplate = (*it);
						effectMenu->addItem(effectButtons[subEffectTemplate->id]);
					}
				}
				const vector<const TowerTemplate*>& towers = tower->getSubTemplates();
				for (vector<const TowerTemplate*>::const_iterator it = towers.begin(); it != towers.end() ; ++it) {
					const TowerTemplate* subTowerTemplate = (*it);
					effectMenu->addItem(towerButtons[subTowerTemplate->id]);
				}
				effectMenu->setVisible(true);
				//cout << " time-out : " << glutGet(GLUT_ELAPSED_TIME) << endl;
			} else {
				effectMenu->setVisible(false);
			}
			//towerMenu->setVisible(false);
			break;
		default:
			towerMenu->setVisible(false);
			effectMenu->setVisible(false);
			break;
	}
}

void GraphicEngine::requestCloseMenu(int menuType)
{
	switch (menuType) {
		case MENU_MAIN : 
			mainMenu->setVisible(false);
			break;
		case MENU_TOWER : 
			towerMenu->setVisible(false);
			break;
		case MENU_EFFECT : 
			effectMenu->setVisible(false);
			break;
		case MENU_COMMAND : 
			buildMenu->setVisible(false);
			break;
		case MENU_MAP : 
			if (mapMenu) {
				mapMenu->setVisible(false);
			}
			break;
		default:
			towerMenu->setVisible(false);
			effectMenu->setVisible(false);
	}
}

const Case& GraphicEngine::requestSelectedCase()
{
	Point2D<int> mapPosition = viewport->getMapPosition(mousePosition);
	return currentParty->getMap()->getCase(mapPosition.x, mapPosition.y);
}

void GraphicEngine::requestZoomChange(int zoomType) {
	if (viewport != NULL) {
		viewport->requestZoomChange(zoomType);
	}
}

bool GraphicEngine::isMenuOpen(int menuType)
{
	switch (menuType) {
		case MENU_MAIN : 
			return mainMenu->isVisible();
		case MENU_TOWER : 
			return towerMenu->isVisible();
		case MENU_EFFECT : 
			return effectMenu->isVisible();
		case MENU_COMMAND : 
			return buildMenu->isVisible();
		case MENU_MAP : 
			return mapMenu->isVisible();
			break;
		default:
			return false;
	}
}

void GraphicEngine::requestReloadShaders()
{
	messageManager->addMessage("Reloading Shaders",MESSAGE_TYPE_WARNING);
	Shader::reloadShaders();
}

void GraphicEngine::requestReloadColors()
{
	ColorManager::destroy();
	ColorManager::init(ConfigReader("../../data/config/colors.txt"));
	messageManager->addMessage("Reloading Colors",MESSAGE_TYPE_WARNING);
}

void GraphicEngine::processMouse(int button, int state, int x, int y)
{
	mouseButton = button;
	mouseState = state;
	Point2D<float> position(x,area.getSizeY()-y);

	if (!moving) {
		bool hittedItem = false;
		const list<Item*>& globalItems = Item::getGlobalItems();

		for (list<Item*>::const_reverse_iterator ci = globalItems.rbegin(); ci != globalItems.rend(); ++ci) {
			Item* item = *ci;
			if (!hittedItem && item->contains(position)) {
				hittedItem = true;
				if (mouseButton == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
					item->mousePress(position);
				} else if (mouseButton == GLUT_LEFT_BUTTON && state == GLUT_UP) {
					item->mouseRelease(position,true);
				}
			} else if (item->isPressed()) {
				item->mouseRelease(position,false);
			}
		}
		if (!hittedItem && currentParty != NULL && currentParty->getState() >= Party::Ingame) {
			if (mouseButton == GLUT_LEFT_BUTTON && state == GLUT_UP) {
				controller->processCommand(COMMAND_SELECT);
			}
		} else {
			allowedToMove = false;
		}
	} else if (mouseButton == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		moving = false;
	}

	if (state == GLUT_UP) {
		allowedToMove = true;
		if (mouseButton == GLUT_RIGHT_BUTTON) {
			controller->processCommand(COMMAND_UNSELECT);
		}
	} else if (state == GLUT_DOWN) {
		if (mouseButton == GLUT_LEFT_BUTTON) {
			clickPosition = position;
		}
	}



	/*if (mouseButton == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bool hittedMenu = false;
		if (focusedItem!=NULL) {
			if (focusedItem->contains(position)) {
				focusedItem->mousePress(position);
				clickedItem = focusedItem;
				hittedMenu = true;
			}
		}

		if (!hittedMenu) {
			if (clickedItem != NULL) {
				clickedItem->mouseRelease(position, false);
				clickedItem = NULL;
			}
			if (position.x > -0.5 && position.x < 9.5 && position.y > -0.5 && position.y < 9.5) {
				Point2D<int> casePosition(round(position.x),round(position.y));
				int caseType = (*(model->getMap()))(casePosition.x,casePosition.y);
				if (caseType == CASE_EMPTY) {
					controller->setSelectedCase(casePosition.y * model->getMap()->getX() + casePosition.x);

					buildMenu->setPosition(casePosition);
					effectMenu->setVisible(false);
					buildMenu->setVisible(true);
				} else if (caseType == CASE_TOWER) {
					controller->setSelectedCase(-1);
					controller->setSelectedTower(model->getMap()->getTower(casePosition));
					buildMenu->setVisible(false);
					effectMenu->setPosition(casePosition);
					effectMenu->setVisible(true);

				} else {
					controller->setSelectedCase(-1);
					buildMenu->setVisible(false);
					effectMenu->setVisible(false);
				}
			} else {
				controller->setSelectedCase(-1);
				buildMenu->setVisible(false);
				effectMenu->setVisible(false);
			}
		}
	}
	else if (mouseButton == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (clickedItem != NULL) {
			if (clickedItem->contains(position)) {
				clickedItem->mouseRelease(position,true);
			} else {
				clickedItem->mouseRelease(position,false);
			}
		}
	}*/
}

void GraphicEngine::processMouseMotion(int x, int y)
{
	Point2D<float> movement(x-mousePosition.x, area.y2 - y-mousePosition.y);
	mousePosition.x = x;
	mousePosition.y = area.y2 - y;

	if (!moving) {
		bool hittedItem = false;
		
		const list<Item*>& globalItems = Item::getGlobalItems();

		for (list<Item*>::const_reverse_iterator ci = globalItems.rbegin(); ci != globalItems.rend(); ++ci) {
			Item* item = *ci;
			if (!hittedItem && item->contains(mousePosition)) {
				hittedItem = true;
				if (item->isInside()) {
					item->mouseMove(mousePosition);
				} else {
					item->mouseEnter(mousePosition);
				}
			} else if (item->isInside()) {
				item->mouseLeave();
			}
		}
	} else {
		if (viewport != NULL) {
			Point2D<float> movementViewport(movement/ -viewport->getZoom());
			viewport->moveMapCoordinate(movementViewport);
		}
	}

	if (!moving && allowedToMove && mouseState == GLUT_DOWN && mouseButton == GLUT_LEFT) {
		float movingSquaredDistance = mousePosition.squaredDistance(clickPosition);
		if ( movingSquaredDistance > 1000.0f ){
			moving = true;
			//controller->processCommand(COMMAND_UNSELECT);
			//glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		} 
	}

	if (viewport != NULL) {
		Point2D<float> origin = viewport->getArea().getOrigin();
		viewport->mouseOver(mousePosition - origin);
	}

	/*Point2D<float> position((x/(float)width)*11-1,((height-y)/(float)height)*11-1);

	if (focusedItem!=NULL) {
		if (focusedItem->contains(position)) {
			return;
		} else {
			focusedItem->mouseLeave();
			focusedItem = NULL;
		}
	}

	const list<Item*>& globalItems = Item::getGlobalItems();

	for (list<Menu*>::iterator ci = globalItems.begin(); ci != globalItems.end() && focusedItem==NULL; ++ci) {
		Menu* menu = *ci;
		if (menu->contains(position)) {
			menu->mouseEnter(position);
			focusedItem = menu;
		}
	}*/
}