#include "render2D_common.h"
#include "Bonus.h" 

Bonus::Bonus(Point2D<float> position, int value, int type): timeLeft(BONUS_TIME), position(position), type(type)
{
	std::ostringstream oss;
	if (type==BONUS_WIN_GOLD) {
		oss << "+" << value;
	} else if (type==BONUS_LOSE_GOLD) {
		oss << "-" << value;
	}
	bonusText = oss.str();
}

Bonus::~Bonus()
{
}

void Bonus::update(int timeSpent)
{
	if (timeLeft>0.0f) {
		timeLeft -= timeSpent;
	}
}

void Bonus::display() {
	if (type==BONUS_WIN_GOLD) {
		glColor4f(0.9f, 0.8f, 0.3f, timeLeft/(float)BONUS_TIME);
	} else if (type==BONUS_LOSE_GOLD) {
		glColor4f(0.9f, 0.3f, 0.3f, timeLeft/(float)BONUS_TIME);
	}
	//FontManager::instance()->displayText(bonusText.c_str(),position.x + cos((BONUS_TIME-timeLeft)/200.0f)*0.1,position.y+(BONUS_TIME-timeLeft)/1000.0f,6);
}