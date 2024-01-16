#include "render2D_common.h"
#include "Statistics.h" 

Statistics::Statistics(): frame(0), time(0), timebase(0), fps(0)
{
}

Statistics::~Statistics()
{
}

void Statistics::update(int timeSpent)
{
	frame++;
	time += timeSpent;
	if (time - timebase > 1000) {
		fps = frame/((time-timebase)/1000.0f);
		timebase = time;		
		frame = 0;
	}
}

float Statistics::getFPS()
{
	return fps;
}