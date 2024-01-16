#ifndef __PARTY_H__
#define __PARTY_H__

#include "Observable.h"

class EDMODEL_API Party : public Observable
{
public:
	
	enum State{
		Preparation,
		Ready,
		Ingame,
		Paused,
		Finished
	};

public:
	Party(const string& name);
	~Party();
	bool join(Player* player);
	void exit(Player* player);

	void setMap(int mapId);
	const Map* getMap() const { return map; };

	bool update(int time);
	void start();
	void pause();
	void resume();

	State getState() const { return state; }

	const string& getName() const { return name; }

	const vector<Unit*>& getUnits() const { return units; };
	const vector<Tower*>& getTowers() const { return towers; };
	const vector<Ammo*>& getAmmos() const { return ammos; };
	const vector<Player*>& getPlayers() const { return players; };

	int getCurrentRound() const { return currentRound; };
	int getLives() const { return lives; };

	const TowerTemplate* getTowerTemplate(int TowerTemplateId) const ;
	Tower* buildTower(const TowerTemplate* towerTemplate, const Case& tcase, Player* player);
	bool addTowerEffect(Tower* tower, int EffectTemplateID, Player* player);

	void fireAmmo(Ammo* ammo);
	void escapeUnit(Unit* unit);
	void killUnit(Unit* unit);

	int getTotalTimeSpent() const { return totalTimeSpent; }
	int getTotalUpdates() const { return totalUpdates; }


private:
	void reset();

	string name;
	State state;

	Map* map;

	vector<Ammo*> ammos;
	vector<Unit*> units;
	vector<Tower*> towers;
	vector<Player*> players;

	int spawnCountDown;

	int currentRound;
	int lives;

	int unitCount;

	int totalTimeSpent;
	int totalUpdates;

	bool started;
	bool paused;
};

#endif