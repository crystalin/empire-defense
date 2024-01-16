#ifndef __OBSERVER__
#define __OBSERVER__

class Object;
class Unit;
class Case;
class Ammo;
class Tower;
class Map;
class Effect;
class Controller;
class TowerTemplate;
class UnitTemplate;
class AmmoTemplate;
class EffectTemplate;
class MapTemplate;
class DisplayManager;
class Player;
class Party;

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
/*
struct Observer :  boost::static_visitor<> {

	void operator()(Tower* tower) { cout << "tower" << endl; }
	void operator()(Unit* unit) { cout << "unit" << endl; }
	void operator()(Ammo* ammo) { cout << "ammo" << endl; }
	void operator()(Party* party) { cout << "party" << endl; }
};*/

class Observer
{
public:
	virtual ~Observer() {};

	virtual void	notifyDeadUnit(const Unit* dead) {}
	virtual void	notifyNewUnit(const Unit* unit) {}
	virtual void	notifyFireAmmo(const Ammo* ammo) {}
	virtual void	notifyAmmoHit(const Ammo* ammo) {}
	virtual void	notifyBuildTower(const Tower* tower) {}
	virtual void	notifyChooseTowerConstruction(const TowerTemplate* towerTemplate) {};
	virtual void	notifyResetGame() {}
	virtual void	notifyChangeMap(const Map* map) {}
	virtual void	notifyCreateParty(Party* party) {}
	virtual void	notifyDestroyParty(Party* party) {}
	virtual void	notifyMessage(const string& message) {}
	virtual void	notifyAddPlayer(const Player* player) {}
	virtual void	notifySelectObject(const Object* object) {}
	virtual void	notifyUpdateTowerList(list<Tower*>& towers) {}
	virtual void	notifyUpdateEffectList(list<Effect*>& effects) {}
	virtual void	notifyLoadMapTemplates(const vector<const MapTemplate*>& mapTemplates) {}
	virtual void	notifyLoadTowerTemplates(const vector<const TowerTemplate*>& towerTemplates) {}
	virtual void	notifyLoadUnitTemplates(const vector<const UnitTemplate*>& unitTemplates) {}
	virtual void	notifyLoadAmmoTemplates(const vector<const AmmoTemplate*>& ammoTemplates) {}
	virtual void	notifyLoadEffectTemplates(const vector<const EffectTemplate*>& effectTemplates) {}
 
};

#endif
