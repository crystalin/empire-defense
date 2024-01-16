#ifndef __OBSERVABLE__
#define __OBSERVABLE__

#include "Observer.h"

/*typedef boost::variant<Tower*,Unit*,Party*,Ammo*,Effect*> SourceVariant;

struct ObservableEvent {
	SourceVariant source;
	ObservableEvent(SourceVariant source) : source(source)  {}
};

typedef boost::signal<void (Event)> Observable;
*/

class MessageNotifyer {
private:
	const string& message;
public:
	MessageNotifyer(const string& message) : message(message) {}
	void operator() (Observer* observer) {
		observer->notifyMessage(message);
	}
};

class Observable
{
private:
	vector<Observer*> list_observers;
 
public:
	Observable() { list_observers.reserve(2); }
	void notifyDeadUnit(Unit* dead) { for_each(list_observers.begin(), list_observers.end(), bind2nd(mem_fun(&Observer::notifyDeadUnit),dead)); }
	void notifyFireAmmo(Ammo* ammo) { for_each(list_observers.begin(), list_observers.end(), bind2nd(mem_fun(&Observer::notifyFireAmmo),ammo)); }
	void notifyAmmoHit(Ammo* ammo) { for_each(list_observers.begin(), list_observers.end(), bind2nd(mem_fun(&Observer::notifyAmmoHit),ammo)); }
	void notifyBuildTower(Tower* tower) { for_each(list_observers.begin(), list_observers.end(), bind2nd(mem_fun(&Observer::notifyBuildTower),tower)); }
	void notifyResetGame() { for_each(list_observers.begin(), list_observers.end(), mem_fun(&Observer::notifyResetGame)); }
	void notifyChangeMap(Map* map) { for_each(list_observers.begin(), list_observers.end(), bind2nd(mem_fun(&Observer::notifyChangeMap),map)); }
	void notifyAddPlayer(const Player* player) { for_each(list_observers.begin(), list_observers.end(), bind2nd(mem_fun(&Observer::notifyAddPlayer),player)); }
	void notifyMessage(const string& message) { for_each(list_observers.begin(), list_observers.end(), MessageNotifyer(message)); }
	void notifyCreateParty(Party* party) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyCreateParty,_1,party)); }
	void notifyDestroyParty(Party* party) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyDestroyParty,_1,party)); }
	void notifyLoadMapTemplates(const vector<const MapTemplate*>& mapTemplates) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyLoadMapTemplates,_1,mapTemplates)); }
	void notifyLoadTowerTemplates(const vector<const TowerTemplate*>& towerTemplates) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyLoadTowerTemplates,_1,towerTemplates)); }
	void notifyLoadUnitTemplates(const vector<const UnitTemplate*>& unitTemplates) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyLoadUnitTemplates,_1,unitTemplates)); }
	void notifyLoadAmmoTemplates(const vector<const AmmoTemplate*>& ammoTemplates) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyLoadAmmoTemplates,_1,ammoTemplates)); }
	void notifyLoadEffectTemplates(const vector<const EffectTemplate*>& effectTemplates) { for_each(list_observers.begin(), list_observers.end(), bind(&Observer::notifyLoadEffectTemplates,_1,effectTemplates)); }
 
 
 
	void addObserver(Observer* observer)
	{
		this->list_observers.push_back(observer);
	}
 
	void removeObserver(Observer* observer)
	{
		list_observers.erase(find(list_observers.begin(), list_observers.end(), observer));
	}
};

#endif
