#include "render2D_common.h"
#include "Button.h" 
#include "Text.h" 
#include "Menu.h"
#include "Map.h" 
#include "commands.h" 
#include "MapCatalog.h" 

MapCatalog::MapCatalog(const vector<const MapTemplate*>& mapTemplates, Point2D<float> position, int direction) : Menu() {
	for_each(mapTemplates.begin(), mapTemplates.end(), bind(&MapCatalog::createButton,this,_1));
}

void MapCatalog::createButton(const MapTemplate* mapTemplate) {
	addItem(new Button(MAP_COMMAND+mapTemplate->id, 128, new Text(mapTemplate->name,Color(1.0f, 1.0f, 1.0f),"big_menu",1,Font::middle),Color(myRand(0.0f, 0.6f), myRand(0.0f, 0.6f), myRand(0.0f, 0.6f), 1.0f)));
}