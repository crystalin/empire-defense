#ifndef __MULTI_DEFORMER_H__
#define __MULTI_DEFORMER_H__

#include "Deformer.h"

class MultiDeformer : public Deformer
{
public:
	MultiDeformer(Animation *animation, Animation *decoration);
	virtual ~MultiDeformer();

	virtual void start() { animation->start(); decoration->start();}
	virtual void stop() { animation->stop(); decoration->stop();}
	virtual void update(int timeSpent);
	virtual bool preDisplay(int addTime = 0);
	virtual void postDisplay(int addTime = 0);
	virtual bool isFinished() { return animation->isFinished(); }

	void enableDecoration() { decoration->start(); }
	void disableDecoration() { decoration->stop(); }

	Animation* getDecoration() { return decoration; }

protected:

	Animation* decoration;

};

#endif