#include "controller_common.h"
#include "PCController.h"
#include "GLUTWrapper.h"
#include "Object.h"
#include "Case.h"
#include "Tower.h"
#include "player.h"
#include "Map.h"

PCController::PCController(Model* model, UserInterface* userInterface): 
Controller(model,userInterface), selectedObject(NULL), constructionMode(false),
towerConstruction(NULL), party(NULL)
{
	keyBinder = KeyBinder::getInstance();
	keyBinder->readConfig(ConfigReader("../../data/config/default.txt"));
	keyBinder->bindKey(KEY_F10,COMMAND_PAUSE);
	keyBinder->bindKey(KEY_F5,GRAPHIC_COMMAND_RELOAD_SHADERS);
	keyBinder->bindKey(KEY_F6,GRAPHIC_COMMAND_RELOAD_COLORS);
	keyBinder->bindKey(KEY_F7,COMMAND_CHANGE_PLAYER);
	keyBinder->bindKey(KEY_ESC,COMMAND_UNSELECT);
	players.push_back(new Player(1,"Crystalin"));
	/*players.push_back(new Player(2,"Mohadryn"));
	players.push_back(new Player(3,"Alan"));
	players.push_back(new Player(4,"Prince Crystal"));
	players.push_back(new Player(5,"Emperator"));
	players.push_back(new Player(6,"Dux Belerum"));
	players.push_back(new Player(7,"Dark Crystal"));
	players.push_back(new Player(8,"Guerioro !"));*/
	player = players.front();
}

PCController::~PCController() {
	delete keyBinder;
	for_each(players.begin(), players.end(), DeleteObject());
}

void PCController::init(int argc, char** argv)
{
	userInterface->setController(this);
	keyBinder->addController(this);
	GLUTWrapper::addInputParser(keyBinder);
	GLUTWrapper::addTimeManager(this);
	GLUTWrapper::setDisplayManager(userInterface);
	GLUTWrapper::init(argc,argv);
	model->addObserver(this);
}

void PCController::start()
{
	model->start();
	timeBase=GLUTWrapper::getTime();
	GLUTWrapper::start();
	
}

void PCController::idle(int timeSpent)
{
	static bool first = true;
	//LOG_FILE(" -------------------------------------------- \ntime spent : " << timeSpent <<endl);
	model->update(timeSpent);
	userInterface->update(timeSpent);
}

void PCController::selectObject(const Case* tcase)
{
	if (constructionMode && (tcase == NULL)) {
		constructionMode = false;
		towerConstruction = NULL;
		userInterface->notifyChooseTowerConstruction(NULL);
	}

	if (tcase == NULL) {
		selectedObject = NULL;
		userInterface->requestCloseMenu(0);
		return;
	}

	Tower* tower = tcase->getTower();
	if (tower) {
		selectedObject = tower;
		userInterface->requestOpenMenu(MENU_EFFECT, tower);
		userInterface->notifySelectObject(tower);
	} else {
		selectedObject = tcase;
		userInterface->notifySelectObject(selectedObject);
		userInterface->requestCloseMenu(MENU_EFFECT);
	}
}

void PCController::processCommand(int id)
{
	//cout << "process command : " << id << endl;
	/*else if (id >= TOWER_COMMAND && id < TOWER_COMMAND_END) {
		if (selectedObject == NULL) {
			return;
		}
		const Case* tcase = NULL;

		if (selectedObject->getType() == TYPE_CASE) {
			tcase = (Case*)selectedObject;
		} else if (selectedObject->getType() == TYPE_TOWER) {
			Tower* tower = (Tower*) selectedObject;
			tcase = tower->getCase();
		} else {
			return;
		}
		model->buildTower(id - TOWER_COMMAND, tcase);
		userInterface->requestCloseMenu(MENU_EFFECT);
	}*/

	if (party != NULL) {
		if (party->getState() == Party::Preparation) {
			if (id >= MAP_COMMAND && id < MAP_COMMAND_END) {
				int mapId = id - MAP_COMMAND;
				party->setMap(mapId);
				party->start();

				selectObject(NULL);
				userInterface->requestCloseMenu(MENU_MAP);
				userInterface->requestOpenMenu(MENU_COMMAND);

				return;
			}
		} else if (party->getState() == Party::Paused) {
			switch (id) {
				case COMMAND_RESUME:
				case COMMAND_PAUSE:
					userInterface->requestOpenMenu(MENU_COMMAND);
					userInterface->requestCloseMenu(MENU_MAIN);
					party->resume();
					break;
				case COMMAND_START:

					for_each(players.begin()+1, players.end(), bind(&Party::exit,party, _1));
					party = model->createParty("party test", players[0]);
					for_each(players.begin()+1, players.end(), bind(&Party::join,party, _1));
					
					userInterface->requestCloseMenu(MENU_MAIN);
					userInterface->requestOpenMenu(MENU_MAP);
					selectObject(NULL);
					break;
			}
		} else if (party->getState() == Party::Finished) {
			switch (id) {
				case COMMAND_PAUSE:
					userInterface->requestOpenMenu(MENU_MAIN);
					userInterface->requestCloseMenu(MENU_COMMAND);
					break;
				case COMMAND_START:
					for_each(players.begin()+1, players.end(), bind(&Party::exit,party, _1));
					party = model->createParty("party test", players[0]);
					for_each(players.begin()+1, players.end(), bind(&Party::join,party, _1));
					
					userInterface->requestCloseMenu(MENU_MAIN);
					userInterface->requestOpenMenu(MENU_MAP);
					selectObject(NULL);
					break;
			}
		} else {
			Object* object = NULL;

			if (id >= EFFECT_COMMAND && id < EFFECT_COMMAND_END) {
				if (selectedObject != NULL && selectedObject->getType() == TYPE_TOWER) {
					Tower* tower = (Tower*) selectedObject;
					if (party->addTowerEffect(tower,id - EFFECT_COMMAND, player )) {
						userInterface->requestCloseMenu(MENU_EFFECT);
					}
				}
				return;
			} else if (id >= TOWER_COMMAND && id < TOWER_COMMAND_END) {
				const TowerTemplate* towerTemplate = party->getTowerTemplate(id - TOWER_COMMAND);
					if (selectedObject != NULL &&
						selectedObject->getType() == TYPE_TOWER &&
						towerTemplate->parentId == ((Tower*) selectedObject)->getId()) {
					Tower* tower = (Tower*) selectedObject;
					const Case& tcase = tower->getCase();
					party->buildTower(towerTemplate, tcase, player);
					userInterface->requestCloseMenu(MENU_EFFECT);
				} else {
					towerConstruction = towerTemplate;
					if (towerConstruction!=NULL) {
						constructionMode = true;
						userInterface->notifyChooseTowerConstruction(towerConstruction);
						userInterface->requestCloseMenu(MENU_TOWER);
						userInterface->requestCloseMenu(MENU_EFFECT);
					} else {
						constructionMode = false;
						userInterface->notifyChooseTowerConstruction(NULL);
					}
				}
				return;
			}
			switch (id) {
				case COMMAND_SELECT:
					{
						const Case& tcase = userInterface->requestSelectedCase();
						if (constructionMode) {
							party->buildTower(towerConstruction, tcase, player);
							userInterface->requestCloseMenu(MENU_EFFECT);

							if (!GLUTWrapper::isShiftPressed()) {
								constructionMode = false;
								towerConstruction = NULL;
								userInterface->notifyChooseTowerConstruction(NULL);
							}

						} else {
							selectObject(&tcase);
						}
					}
					break;
				case COMMAND_UNSELECT:
					selectObject(NULL);
					break;
				case COMMAND_PAUSE:
					userInterface->requestCloseMenu(MENU_COMMAND);
					userInterface->requestOpenMenu(MENU_MAIN);
					party->pause();
					break;
				case GRAPHIC_COMMAND_ZOOM_MORE:
					userInterface->requestZoomChange(ZOOM_MORE);
					break;
				case GRAPHIC_COMMAND_ZOOM_LESS:
					userInterface->requestZoomChange(ZOOM_LESS);
					break;
				case GRAPHIC_COMMAND_MOVE_LEFT:
					userInterface->requestMoveMap(MOVE_LEFT);
					break;
				case GRAPHIC_COMMAND_MOVE_RIGHT:
					userInterface->requestMoveMap(MOVE_RIGHT);
					break;
				case GRAPHIC_COMMAND_MOVE_UP:
					userInterface->requestMoveMap(MOVE_UP);
					break;
				case GRAPHIC_COMMAND_MOVE_DOWN:
					userInterface->requestMoveMap(MOVE_DOWN);
					break;
				case COMMAND_BUILD:
					if (!userInterface->isMenuOpen(MENU_TOWER)) {
						userInterface->requestOpenMenu(MENU_TOWER);
					} else {
						userInterface->requestCloseMenu(MENU_TOWER);
					}
					break;
			}
		}
	} else {
		switch (id) {
			case COMMAND_START:
				party = model->createParty("party test", players[0]);
				if (!players.empty()) {
					for_each(players.begin()+1, players.end(), bind(&Party::join,party, _1));
				}
				
				userInterface->requestCloseMenu(MENU_MAIN);
				userInterface->requestOpenMenu(MENU_MAP);

				break;
				/*case COMMAND_PAUSE:
				userInterface->requestCloseMenu(MENU_COMMAND);
				userInterface->requestOpenMenu(MENU_MAIN);
				party->pause();
				break;*/
		}
	}

	switch (id) {
		case COMMAND_EXIT:
			exit(1);
			break;
		case GRAPHIC_COMMAND_FULLSCREEN:
			userInterface->requestFullscreenChange();
			break;
		case GRAPHIC_COMMAND_RELOAD_SHADERS:
			userInterface->requestReloadShaders();
			break; 
		case GRAPHIC_COMMAND_RELOAD_COLORS:
			userInterface->requestReloadColors();
			break;
		case GRAPHIC_COMMAND_DISPLAY_GRID:
			userInterface->requestDisplayGrid();
			break;
		case COMMAND_CHANGE_PLAYER:
			vector<Player*>::iterator occ = find(players.begin(),players.end(),player);
			occ++;
			if (occ == players.end()) {
				player = players.front();
			} else {
				player = *occ;
			}
			break;
	}
}
void PCController::addInputParser(InputParser* parser){
	GLUTWrapper::addInputParser(parser);
}

void PCController::bindKey(char key, int command) {
	keyBinder->bindKey(key,command);
}

void PCController::notifyLoadTowerTemplates(const vector<const TowerTemplate*>& towerTemplates) {
	vector<const TowerTemplate*>::const_iterator it;
	vector<const TowerTemplate*>::const_iterator end= towerTemplates.end();
	for(it = towerTemplates.begin(); it != end; it++) {
		const TowerTemplate* towerTemplate = *it;
		if (towerTemplate->parentId == 0) {
			keyBinder->bindKey(towerTemplate->name[0],TOWER_COMMAND + towerTemplate->id);
		}
	}
}