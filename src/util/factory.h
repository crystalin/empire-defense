#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "configreader.h"

class Party;

template<class T1, class T2> // T1 ClassTemplate, T2 Class
class Factory
{
public:
	static const T1* getTemplate(int id);
	static void changeTemplate(T2* object, int id);

	static void init(ConfigReader& configReader);
	static void destroy();

	static const vector<const T1*>& getBasicTemplates() { return basicTemplates; }
	static const vector<const T1*>& getTemplates() { return templates; }

	static bool compare(const T1* t1, const T1* t2);
	static T2* create(int id);

	static T2* create(Party* party, const T1*);
	static T2* create(Party* party, const T1&);

	template <typename T3>
	static T2* create(Party* party, const T1*, T3);
	template <typename T3>
	static T2* create(Party* party, const T1&, T3);

	template <typename T3, typename T4>
	static T2* create(Party* party, const T1*, T3, T4);
	template <typename T3, typename T4>
	static T2* create(Party* party, const T1&, T3, T4);

	static void store(T2*);
	static void resetPool();

protected:
	static vector<const T1*> templates;
	static vector<T1*> templatesMap;
	static vector<const T1*> basicTemplates;
	static string factoryName;

	static boost::object_pool<T2> pool;
	
	static void storeTemplates(const list<T1*>& t1List);
	static void addTemplate(T1* t1);
};


template<class T1, class T2> // T1 ClassTemplate, T2 Class
boost::object_pool<T2> Factory<T1,T2>::pool(128);
template<class T1, class T2> // T1 ClassTemplate, T2 Class
vector<const T1*> Factory<T1,T2>::templates;
template<class T1, class T2> // T1 ClassTemplate, T2 Class
vector<T1*> Factory<T1,T2>::templatesMap;
template<class T1, class T2> // T1 ClassTemplate, T2 Class
vector<const T1*> Factory<T1,T2>::basicTemplates;

template<class T1, class T2> // T1 ClassTemplate, T2 Class
bool Factory<T1,T2>::compare(const T1* t1, const T1* t2)
{
	return t1->id < t2->id;
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
const T1* Factory<T1,T2>::getTemplate(int id)
{
	if ((int)templatesMap.size() <= id) {
		cout << "[" << factoryName << "] Template Id " << id << "not found" << endl;
		if (!templatesMap.empty()) {
			return templatesMap[0];
		}
	}
	return templatesMap[id];
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
T2* Factory<T1,T2>::create(int id)
{
	if (templatesMap[id] == NULL) {
		cerr << "Template ID : " << id << " invalid !" << endl;
	}
	return create(templatesMap[id]);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
void Factory<T1,T2>::destroy()
{
	for (vector<const T1*>::iterator ci = templates.begin(); ci != templates.end(); ++ci) {
		delete (*ci);
	}
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
T2* Factory<T1,T2>::create(Party* party, const T1* t1)
{
	return pool.construct(party, t1);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
T2* Factory<T1,T2>::create(Party* party, const T1& t1)
{
	return pool.construct(party, t1);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
template<typename T3>
T2* Factory<T1,T2>::create(Party* party, const T1* t1, T3 t3)
{
	return pool.construct(party, t1,t3);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
template<typename T3>
T2* Factory<T1,T2>::create(Party* party, const T1& t1, T3 t3)
{
	return pool.construct(party, t1,t3);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
template<typename T3, typename T4>
T2* Factory<T1,T2>::create(Party* party, const T1* t1, T3 t3, T4 t4)
{
	return pool.construct(party, t1, t3, t4);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
template<typename T3, typename T4>
T2* Factory<T1,T2>::create(Party* party, const T1& t1, T3 t3, T4 t4)
{
	return pool.construct(party, t1, t3, t4);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
void Factory<T1,T2>::store(T2* t2)
{
	pool.destroy(t2);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
void Factory<T1,T2>::storeTemplates(const list<T1*>& t1List)
{
	int maxId = (*max_element(t1List.begin(), t1List.end(),compare))->id;
	templates.reserve(t1List.size());
	basicTemplates.reserve(t1List.size());
	templatesMap.resize(maxId+1);
	
	for_each(t1List.begin(), t1List.end(), Factory<T1,T2>::addTemplate);
}

template<class T1, class T2> // T1 ClassTemplate, T2 Class
void Factory<T1,T2>::addTemplate(T1* t1)
{
	templates.push_back(t1);
	templatesMap[t1->id] = t1;
}

// Functor
template <class TTemplate,class Tclass>
class updateAndStore {
private :
	int timeSpent;
public : 
	inline updateAndStore(int timeSpent) : timeSpent(timeSpent) {}
	inline bool operator()(Tclass* obj) { 
		if (obj->update(timeSpent)) { 
			Factory<TTemplate, Tclass>::store(obj);
			return true;
		}
		return false;
	}
};

template <class TTemplate,class Tclass>
class storeDead {
public : 
	inline bool operator()(Tclass* obj) { 
		if (obj->isDead()) { 
			Factory<TTemplate, Tclass>::store(obj);
			return true;
		}
		return false;
	}
};

#endif