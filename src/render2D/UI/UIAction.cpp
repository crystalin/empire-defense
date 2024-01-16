#include "render2D_common.h"
#include "UIAction.h" 

UIAction::UIAction()
{
	//cout << *this << "created \n";
	static int count = 0;
	id = count++;
}

UIAction::~UIAction() 
{
	//cout << *this << "deleting \n";
}

ostream& operator << (ostream& os, const UIAction& action)
{
	return action.print(os);
}