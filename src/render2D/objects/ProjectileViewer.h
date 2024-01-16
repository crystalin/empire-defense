#ifndef __PROJECTILE_VIEWER_H__
#define __PROJECTILE_VIEWER_H__

#include "animation.h"

class Ammo;
class Texture;

#define LASER_LENGTH 2.0f
#define ARROW_LENGTH 0.7f

class ProjectileViewer
{
public:
	static void display(Ammo* ammo);
	static void init();
	static void destroy();

protected:
	ProjectileViewer();
	~ProjectileViewer();
	
	static Texture* ballTexture;
	static Texture* arrowTexture;
	static Texture* laserTexture;
	static Texture* missileTexture;
	static Texture* shadowTexture;
};

#endif
