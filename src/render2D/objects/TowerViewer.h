#ifndef __TOWER_VIEWER_H__
#define __TOWER_VIEWER_H__

#include "animation.h"

class Tower;
class TowerTemplate;
class Texture;
class SmokeGenerator;
class RoundAnimation;

#define AMMO_TEXTURE_ARROW 0
#define AMMO_TEXTURE_MISSILE 1
#define AMMO_TEXTURE_BALL 2
#define AMMO_TEXTURE_LASER 3
#define AMMO_MAX_TYPE 4

#define TOWER_MAX_LEVEL 3

class TowerViewer
{
public:
	static void update(Tower* tower, SmokeGenerator* smokeGenerator);
	static void display(Tower* tower);
	static void display(const TowerTemplate* towerTemplate);
	static void init();
	static void destroy();

protected:
	TowerViewer();
	~TowerViewer();
	
	static Texture* ammoTextures[AMMO_MAX_TYPE];
	static Texture* towerTextures[TOWER_MAX_LEVEL];
	static Texture* shadowTextures[TOWER_MAX_LEVEL];

	static RoundAnimation* spellAnimation;
	static Texture* loadingTexture;
	static Texture* flagTexture;
};

#endif
