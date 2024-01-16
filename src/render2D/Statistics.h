#ifndef __STATISTICS__
#define __STATISTICS__

class Statistics
{
public:
	Statistics();
	~Statistics();

	void update(int timeSpent);

	float getFPS();

protected:

	int frame;
	int time;
	int timebase;

	float fps;
};

#endif
