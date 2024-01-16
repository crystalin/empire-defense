#ifndef __MODEL_H__
#define __MODEL_H__

#include "Observable.h"

class MapTemplate;
class TowerTemplate;
class EffectTemplate;
class AmmoTemplate;
class UnitTemplate;
class Map;
class Case;
class Tower;
class Observable;
class Player;
class Party;

class EDMODEL_API Model : public Observable
{
public:
	Model();
	~Model();

	void start();

	/*const vector<const MapTemplate*>& getMapTemplates();
	const vector<const TowerTemplate*>& getTowerTemplates();
	const vector<const EffectTemplate*>& getEffectTemplates();
	const vector<const AmmoTemplate*>& getAmmoTemplates();
	const vector<const UnitTemplate*>& getUnitTemplates();*/

	const vector<Player*>& getPlayers() { return players; }
	const vector<Party*>& getParties() { return parties; }

	void update(int time);

	Player* createPlayer(const string& name);
	void deletePlayer(Player* player);

	Party* createParty(const string& partyName, Player* player);

private:

	bool started;

	vector<Player*> players;
	vector<Party*> parties;

	int maxPlayerId;
};

#endif