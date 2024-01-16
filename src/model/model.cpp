#include "model_common.h"
#include "model.h"
#include "ammo.h"
#include "tower.h"
#include "player.h"
#include "map.h"
#include "party.h"
#include "factory.h"

Model::Model() : maxPlayerId(0), started(false)
{
}

Model::~Model()
{
	for_each(parties.begin(), parties.end(), DeleteObject());

	Factory<AmmoTemplate, Ammo>::destroy();
	Factory<TowerTemplate, Tower>::destroy();
	Factory<EffectTemplate, Effect>::destroy();
	Factory<UnitTemplate, Unit>::destroy();
	Factory<MapTemplate, Map>::destroy();
}

void Model::start() {
	if (started) return;
	started = true;
	Factory<AmmoTemplate, Ammo>::init(ConfigReader("../ressources/ammos.txt"));
	notifyLoadAmmoTemplates(Factory<AmmoTemplate, Ammo>::getTemplates());
	Factory<EffectTemplate, Effect>::init(ConfigReader("../ressources/spells.txt"));
	notifyLoadEffectTemplates(Factory<EffectTemplate, Effect>::getTemplates());
	Factory<UnitTemplate, Unit>::init(ConfigReader("../ressources/units.txt"));
	notifyLoadUnitTemplates(Factory<UnitTemplate, Unit>::getTemplates());
	Factory<TowerTemplate, Tower>::init(ConfigReader("../ressources/towers.txt"));
	notifyLoadTowerTemplates(Factory<TowerTemplate, Tower>::getTemplates());
	Factory<MapTemplate, Map>::init(ConfigReader("../ressources/maps.txt"));
	notifyLoadMapTemplates(Factory<MapTemplate, Map>::getTemplates());
}

/*const vector<const MapTemplate*>& Model::getMapTemplates() { 
	return Factory<MapTemplate, Map>::getTemplates() ;
}
const vector<const TowerTemplate*>& Model::getTowerTemplates() { 
	return Factory<TowerTemplate, Tower>::getTemplates() ;
}
const vector<const UnitTemplate*>& Model::getUnitTemplates() { 
	return Factory<UnitTemplate, Unit>::getTemplates() ;
}
const vector<const EffectTemplate*>& Model::getEffectTemplates() { 
	return Factory<EffectTemplate, Effect>::getTemplates() ;
}
const vector<const AmmoTemplate*>& Model::getAmmoTemplates() { 
	return Factory<AmmoTemplate, Ammo>::getTemplates() ;
}*/

Player* Model::createPlayer(const string& name) {
	vector<Player*>::iterator occ = find_if(players.begin(), players.end(), bind(&Player::getName,_1) == name);
	if (occ != players.end()) {
		return NULL;
	}

	Player* player = new Player(maxPlayerId++,name);
	players.push_back(player);

	return player;
}

void Model::deletePlayer(Player* player) {
	vector<Player*>::iterator occ = find(players.begin(), players.end(), player);
	if (occ == players.end()) {
		return;
	}
	for_each(parties.begin(), parties.end(), bind(&Party::exit, _1, player));

	//Many check to be done
	//players.erase(remove(players.begin(), players.end(), player),players.end());
}


void Model::update(int timeSpent) {
	parties.erase(remove_if(parties.begin(), parties.end(), bind(&Party::update, _1, timeSpent)),parties.end());
}

Party* Model::createParty(const string& partyName, Player* player) {
	Party* party = new Party(partyName);
	party->join(player);
	parties.push_back(party);
	notifyCreateParty(party);
	return party;
}