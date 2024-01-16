#include "render2D_common.h"
#include "PopupAction.h" 

PopupAction::PopupAction(Item* item) : item(item)
{
	active = false;
	item->request();
	item->setVisible(false);
}

PopupAction::~PopupAction() 
{
	item->release();
}