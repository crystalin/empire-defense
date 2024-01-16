#ifndef __ICONS__
#define __ICONS__

#include "Texture.h"

#define ICON_BACKGROUND 0

#define ICON_FIRE 1
#define ICON_ICE 2
#define ICON_POISON 3
#define ICON_EARTH 4
#define ICON_GOLD 5
#define ICON_LIGHT 6
#define ICON_MALEDICTION 7
#define ICON_ELECTRICITY 8
#define ICON_BUILD 9
#define ICON_TOWER_MISSILE 10
#define ICON_MONSTER 11
#define ICON_AMMO 12
#define ICON_EFFECT_ICE 13
#define ICON_PLUS 14
#define ICON_BACKGROUND_OVER 15
#define ICON_BACKGROUND_PRESSED 16
#define ICON_TOWER_LASER 17
#define ICON_BACKGROUND_BIG 18
#define ICON_BACKGROUND_BIG_OVER 19
#define ICON_BACKGROUND_BIG_PRESSED 20

#define MAX_ICONS 21

class Icons
{
public:
	static void initTextures();
	static void deleteTextures();
	static void displayBackground(const Rectangle<float>& area = Rectangle<float>(0,0,32,32), bool over = false, bool pressed = false);
	static void displayIcon(int type, const Rectangle<float>& area = Rectangle<float>(0,0,32,32), bool over = false, bool pressed = false);
	static void displayEffect(int type, const Rectangle<float>& area = Rectangle<float>(0,0,32,32));

protected:
	static Texture* icons[MAX_ICONS];
};

#endif
