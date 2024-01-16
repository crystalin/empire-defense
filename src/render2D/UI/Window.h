#ifndef __WINDOW_UI_H__
#define __WINDOW_UI_H__

#include "Container.h"
#include "Color.h"
#include "SubTextureAnimation.h"

class Window : public Container<Item>
{
public:
	Window(PositionController* positionController, const Point2D<float>& size, Color color = Color(1.0, 1.0, 1.0, 1.0f));


	virtual void update(int timeSpent);
	virtual bool contains(const Point2D<float>& mousePosition);
	virtual void display();
	
	virtual void setSize(const Point2D<float>& size) { this->size = size; }
	virtual const Point2D<float>& getSize() {return size; }

	virtual ostream& print(ostream& os) const { return os << "Window " << id << " " << positionController->getPosition() ;}
protected:
	virtual ~Window();

	Point2D<float> size;
	Color color;

	static Texture* texture;
	Rectangle<float> texArea[9];

};

#endif
