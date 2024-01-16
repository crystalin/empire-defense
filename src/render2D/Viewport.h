#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

#include "Window.h"
#include "Icons.h"
#include "effect.h"
#include "SmokeGenerator.h"
#include "ExplosionGenerator.h"
#include "AnimatedTextureAnimation.h"
#include "RoundAnimation.h"

class Shader;
class LaserPath;
class Party;
class Object;
class Ammo;
class Tower;
class TowerTemplate;
class Map;
class Case;

class Viewport
{
public:
	Viewport(const Party& party, const Rectangle<float>& area);
	~Viewport(void);

	void update(int timeSpent);
	void display();

	void setArea(const Rectangle<float>& area);
	void moveMapCoordinate(const Point2D<float>&);
	void setZoom(float zoom);

	float getZoom() { return zoom; }
	const Rectangle<float>& getArea () { return area; }
	const Point2D<float>& getMapCoordinate () { return mapCoordinate; }
	Point2D<float> getMapPosition(const Point2D<float>& hit);
	Point2D<float> getScreenPosition(const Point2D<float>& hit);

	void notifySelectObject(const Object* object);

	void mouseOver(const Point2D<float>& point);
	
	void notifyChangeMap(const Map* map);
	void notifyFireAmmo(const Ammo* ammo);
	void notifyAmmoHit(const Ammo* ammo);
	void notifyBuildTower(const Tower* tower);
	void requestZoomChange(int zoomType);
	void notifyChooseTowerConstruction(const TowerTemplate* towerTemplate);

	bool displayGrid;
	//TEST
	SmokeGenerator* smokeGenerator;
	SmokeGenerator* towerSmokeGenerator;
	ExplosionGenerator* explosionGenerator;
	

protected:
	const Party& party;

	RoundAnimation* effectsAnimation[EFFECT_NUMBER];

	Texture* mapTexture;

	static Color effectColor[EFFECT_NUMBER];
	static int effectIcon[EFFECT_NUMBER];

	Rectangle<float> area;
	Point2D<float> mapCoordinate;
	Point2D<float> mouseCoordinate;
	Point2D<float> caseCoordinate;

	const TowerTemplate* towerConstruction;
	bool constructionMode;

	const Case* mouseCase;
	float zoom;

	Object* selectedObject;

	void checkArea();

	//TEST
	vector<Animation*> animations;
	Animation* selectedCaseAnimation;
	Shader* unitShader;

	LaserPath* towerConstructionPath;

};

#endif
