#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Animation.h"
#include "Item.h"

class Image : public Item
{
public:
	Image(Animation* animation, PositionController* positionController = new DefaultPositionController());

	virtual void update(int timeSpent);
	virtual bool contains(const Point2D<float>& mousePosition) { return (animation->getArea() + positionController->getPosition()).contains(mousePosition); }

	virtual void setVisible(bool visible);
	virtual void display();

	virtual ostream& print(ostream& os) const { return os << "Image " << id << " " << positionController->getPosition() ;}

protected:
	virtual ~Image();

	Animation* animation;
};

#endif
