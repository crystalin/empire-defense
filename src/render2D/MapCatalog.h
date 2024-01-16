#ifndef __MAP_CATALOG__
#define __MAP_CATALOG__

#include "Menu.h"

class MapCatalog : public Menu
{
public:
	MapCatalog(const vector<const MapTemplate*>& mapTemplates, Point2D<float> position, int direction = MENU_HORIZONTAL);
protected:
	void createButton(const MapTemplate* mapTemplate);
};

#endif
