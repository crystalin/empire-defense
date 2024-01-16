#include "model_common.h"
#include "party.h"
#include "ammo.h"
#include "tower.h"
#include "player.h"
#include "map.h"
#include "mapfactory.h"

Party::Party(const string& name) : name(name), spawnCountDown(10000), currentRound(1), lives(25), unitCount(0), totalTimeSpent(0), totalUpdates(0), map(NULL), state(Preparation)
{
	units.reserve(50);
	ammos.reserve(200);
	towers.reserve(200);
	players.reserve(10);
}

Party::~Party()
{
	for_each(towers.begin(), towers.end(), Factory<TowerTemplate,Tower>::store);
	for_each(ammos.begin(), ammos.end(), Factory<AmmoTemplate,Ammo>::store);
	for_each(units.begin(), units.end(), Factory<UnitTemplate,Unit>::store);

	if (map != NULL) {
		Factory<MapTemplate, Map>::store(map);
	}
}

void Party::setMap(int mapId) {
	if (state != Preparation) {
		return;
	}

	const MapTemplate* mapTemplate = Factory<MapTemplate, Map>::getTemplate(mapId);
	
	if (mapTemplate == NULL) {
		return;
	}

	this->map = Factory<MapTemplate, Map>::create(this,*mapTemplate);

	notifyChangeMap(map);
	state = Ready;
}

void Party::start() {
	
	if (state != Ready) {
		return;
	}

	map->reset();

	lives = 25;
	currentRound = 1;
	unitCount = 0;
	spawnCountDown = 15000;
	
	for_each(players.begin(), players.end(), bind2nd(mem_fun(&Player::setPartyMoney),200));
	
	state = Ingame;
	notifyMessage( (boost::format("%1%") % (spawnCountDown/1000.0f)).str() + " secondes before the game start !! Be ready" );
}

void Party::resume() {
	if (state != Paused) {
		return;
	}
	state = Ingame;
}

void Party::pause() {
	if (state != Ingame) {
		return;
	}
	state = Paused;
}

bool Party::join(Player* player) {
	if (state != Preparation) {
		return false;
	}

	vector<Player*>::iterator occ = find(players.begin(), players.end(), player);
	if (occ != players.end()) {
		return false;
	}

	player->setTeam(players.size());
	players.push_back(player);
	
	notifyAddPlayer(player);
	return true;
}


class TowerOwnerDestroyer {
private:
	const Player* player;
public:
	TowerOwnerDestroyer(const Player* player) : player(player) {}
	bool operator()(Tower* tower) {
		if (tower->getOwner() == player) {
			Factory<TowerTemplate,Tower>::store(tower);
			return true;
		}
		return false;
	}
};

void Party::exit(Player* player) {
	
	switch (state) {
		case Ingame:
		case Paused:
			towers.erase(remove_if (towers.begin(),towers.end(),TowerOwnerDestroyer(player)),towers.end());
		case Preparation:
		case Ready :
		case Finished : {
			players.erase(remove(players.begin(), players.end(), player),players.end());
		} break;
	}
}


bool Party::update(int timeSpent) {

	if (players.empty()) {
		notifyDestroyParty(this);
		return true;
	}
	if (state != Ingame) {
		return false;
	}

	totalTimeSpent += timeSpent;
	totalUpdates++;

	ammos.erase(remove_if (ammos.begin(),ammos.end(),updateAndStore<AmmoTemplate,Ammo>(timeSpent)),ammos.end());
	units.erase(remove_if (units.begin(),units.end(),updateAndStore<UnitTemplate,Unit>(timeSpent)),units.end());
	for_each(towers.begin(), towers.end(), bind(&Tower::update,_1,timeSpent));
	
	spawnCountDown -= timeSpent;
	if (spawnCountDown<=0) {
		if (unitCount == 30) {
			if (units.empty()) {
				notifyMessage("You finished the round " + (boost::format("%1%") % currentRound).str());
				cout << "[Party] Round " << currentRound << " completed " << endl;
				unitCount = 0;
				spawnCountDown = 6000;
				currentRound++;
				for_each(players.begin(), players.end(), PlayerRewardor(currentRound*5,currentRound*5));
				const UnitTemplate* unitTemplate = Factory<UnitTemplate,Unit>::getTemplate(currentRound);
				if (unitTemplate != NULL) {
					boost::format floatFormat("%1%");
					notifyMessage("New Round : " + unitTemplate->name + " [" + ( floatFormat % unitTemplate->maxLife ).str() + "pv, "
						+ ( floatFormat % unitTemplate->armor ).str() + " armor, "
						+ ( floatFormat % unitTemplate->speed ).str() + " speed]");
				}
			}
		} else {
			unitCount++;
			spawnCountDown = 1000;
			if (map->getRoadCount() > 0) {
				const UnitTemplate* unitTemplate = Factory<UnitTemplate,Unit>::getTemplate(currentRound);
				if (unitTemplate != NULL) {
					Unit *newUnit = Factory<UnitTemplate,Unit>::create<Path*>(this, *unitTemplate, (Path*)&map->getRoad(0));
					units.push_back(newUnit);
					spawnCountDown = (int)((1.0f/unitTemplate->speed) *1000) ;
				}
			}
		}
	}

	if (lives <= 0 ) {
		notifyMessage("You Lost !");
		state = Finished;
	}
	return false;
}

void Party::fireAmmo(Ammo* ammo) {
	//cout << *ammo << "added to the list" << endl;
	ammos.push_back(ammo);
	//cout << " ammo list Size : " << ammos.size() <<  endl;
	notifyFireAmmo(ammo);
}

void Party::escapeUnit(Unit* unit) {
	lives--;
}

void Party::killUnit(Unit* unit) {

	static int moneyleft = 0;
	int money = unit->getLevel() + moneyleft;
	moneyleft = money%players.size();
	int moneySplit = money/players.size();
	int experience = 5 * unit->getLevel();

	for_each(players.begin(), players.end(), PlayerRewardor(moneySplit,experience));
}

/*void Model::removeTower(Tower* tower)
{
	towers.erase(find(towers.begin(), towers.end(), tower));
}
void Model::removeAmmo(Ammo* ammo)
{
	ammos.erase(find(ammos.begin(), ammos.end(), ammo));
}
void Model::removeUnit(Unit* unit)
{
	units.erase(find(units.begin(), units.end(), unit));
}*/

const TowerTemplate* Party::getTowerTemplate(int TowerTemplateId) const 
{
	return Factory<TowerTemplate, Tower>::getTemplate(TowerTemplateId);
}

Tower* Party::buildTower(const TowerTemplate* towerTemplate, const Case& tcase, Player* player)
{
	const EffectTemplate* effectTemplate = NULL;
	if (towerTemplate == NULL) {
		cout << "[Debug] Tower Template Null" << endl;
		return NULL;
	}
	
	if (player->getPartyMoney()<towerTemplate->cost) {
		notifyMessage("You need more money for this tower");
		return NULL;
	}

	if (tcase.isOnRoad()) {
		notifyMessage("Towers are not allowed on the Road");
		return NULL;
	}

	if (towerTemplate->level !=0) {
		Tower* currentTower = tcase.getTower();
		if (currentTower == NULL) {
			notifyMessage("You need a tower to make this upgrade");
			return NULL;
		}

		if (currentTower->getOwner() != player) {
			notifyMessage("This tower doesn't belong to you");
			return NULL;
		}

		if (currentTower->getBuildingTimeLeft() > 0) {
			notifyMessage("Tower in construction. You have to wait");
			return NULL;
		}
		if (currentTower->getId() != towerTemplate->parentId) {
			notifyMessage("This upgrade is not valid for this tower");
			return NULL;
		}
		effectTemplate = currentTower->getEffectTemplate();
		towers.erase(find(towers.begin(), towers.end(), currentTower));
		map->removeTower((int)currentTower->getPosition().x, (int)currentTower->getPosition().y);
		Factory<TowerTemplate, Tower>::store(currentTower);
	} else {
		if (tcase.getTower() != NULL) {
			notifyMessage("There is already a tower on this case");
			return NULL;
		}
	}

	Tower* tower  = Factory<TowerTemplate, Tower>::create<const Case&, Player*>(this, *towerTemplate, tcase, player);
	if (effectTemplate != NULL) {
		tower->setEffectTemplate(effectTemplate);
	}

	player->losePartyMoney(towerTemplate->cost);
	towers.push_back(tower);
	map->addTower((int)tcase.getPosition().x , (int)tcase.getPosition().y, tower);
	notifyBuildTower(tower);

	return tower;
}


bool Party::addTowerEffect(Tower* tower, int EffectTemplateID, Player* player) {
	if (tower==NULL) {
		return false;
	}

	const EffectTemplate* currentEffectTemplate = tower->getEffectTemplate();
	const EffectTemplate* newEffectTemplate = Factory<EffectTemplate, Effect>::getTemplate(EffectTemplateID);

	if (newEffectTemplate == NULL) {
		return false;
	}

	if (player != tower->getOwner()) {
		notifyMessage("You are not the owner of this Tower");
		return false;
	}

	if (player->getPartyMoney() < newEffectTemplate->cost) {
		notifyMessage("You need more money for this effect");
		return false;
	}

	if (currentEffectTemplate != NULL) {
		if (currentEffectTemplate->type != newEffectTemplate->type) {
			notifyMessage("You can't change the effect type already put on this tower");
			return false;
		}
		if (currentEffectTemplate->id != newEffectTemplate->parentId) {
			notifyMessage("This upgrade is not valid for the current effect on this tower");
			return false;
		}
	} else if ( newEffectTemplate->parentId != 0) {
		notifyMessage("You can't apply this effect upgrade");
		return false;
	}

	player->losePartyMoney(newEffectTemplate->cost);
	tower->setEffectTemplate(newEffectTemplate);
	return true;
}