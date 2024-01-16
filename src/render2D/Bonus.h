#ifndef __BONUS__
#define __BONUS__

#include "FontManager.h"

#define BONUS_TIME 2000

#define BONUS_WIN_GOLD 1
#define BONUS_LOSE_GOLD 2

class Bonus
{
public:
	Bonus(Point2D<float> position, int value, int type = BONUS_WIN_GOLD);
	~Bonus();

	bool isFinish() { return timeLeft <= 0.0f; };
	void display();
	void update(int timeSpent);

protected:
	int timeLeft;
	int type;
	Point2D<float> position;
	string bonusText;
};

#endif
