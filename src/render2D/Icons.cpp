#include "render2D_common.h"
#include "Icons.h" 
#include "RessourceManager.h" 

Texture* Icons::icons[MAX_ICONS];


void Icons::initTextures()
{
	icons[ICON_BACKGROUND] = RessourceManager::instance()->getTexture("icon.png");
	icons[ICON_BACKGROUND_OVER] = RessourceManager::instance()->getTexture("icon-over.png");
	icons[ICON_BACKGROUND_PRESSED] = RessourceManager::instance()->getTexture("icon-pressed.png");
	icons[ICON_BACKGROUND_BIG] = RessourceManager::instance()->getTexture("icon-big.png");
	icons[ICON_BACKGROUND_BIG_OVER] = RessourceManager::instance()->getTexture("icon-big-over.png");
	icons[ICON_BACKGROUND_BIG_PRESSED] = RessourceManager::instance()->getTexture("icon-big-pressed.png");
	icons[ICON_FIRE] = RessourceManager::instance()->getTexture("fire-icon.png");
	icons[ICON_ICE] = RessourceManager::instance()->getTexture("ice-icon.png");
	icons[ICON_POISON] = RessourceManager::instance()->getTexture("poison-icon.png");
	icons[ICON_EARTH] = RessourceManager::instance()->getTexture("earth-icon.png");
	icons[ICON_GOLD] = RessourceManager::instance()->getTexture("gold-icon.png");
	icons[ICON_LIGHT] = RessourceManager::instance()->getTexture("light-icon.png");
	icons[ICON_MALEDICTION] = RessourceManager::instance()->getTexture("malediction-icon.png");
	icons[ICON_ELECTRICITY] = RessourceManager::instance()->getTexture("electricity-icon.png");
	icons[ICON_BUILD] = RessourceManager::instance()->getTexture("build-icon.png");
	icons[ICON_TOWER_MISSILE] = RessourceManager::instance()->getTexture("tower-missile.png");
	icons[ICON_TOWER_LASER] = RessourceManager::instance()->getTexture("tower-laser.png");
	icons[ICON_MONSTER] = RessourceManager::instance()->getTexture("monster-icon.png");
	icons[ICON_AMMO] = RessourceManager::instance()->getTexture("missile.png");
	icons[ICON_EFFECT_ICE] = RessourceManager::instance()->getTexture("ice-effect.png");
	icons[ICON_PLUS] = RessourceManager::instance()->getTexture("plus-icon.png");

	for (int i = 0; i < MAX_ICONS; i++) {
		icons[i]->request();
	}
	
}

void Icons::deleteTextures() {
	//cout << "[icons] releasing textures" << endl;
	for (int i = 0; i < MAX_ICONS; i++) {
		icons[i]->release();
	}
}

void Icons::displayBackground(const Rectangle<float>& area, bool over, bool pressed)
{
	if (area.getSizeX() > 60.0f) {
		if (pressed) {
			icons[ICON_BACKGROUND_BIG_PRESSED]->bind();
		} else if (over) {
			icons[ICON_BACKGROUND_BIG_OVER]->bind();
		} else {
			icons[ICON_BACKGROUND_BIG]->bind();
		}
	} else {
		if (pressed) {
			icons[ICON_BACKGROUND_PRESSED]->bind();
		} else if (over) {
			icons[ICON_BACKGROUND_OVER]->bind();
		} else {
			icons[ICON_BACKGROUND]->bind();
		}
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(area.x1, area.y1,  0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(area.x2, area.y1,  0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(area.x2, area.y2,  0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(area.x1, area.y2,  0.0f);
	glEnd();
}

void Icons::displayIcon(int type, const Rectangle<float>& area, bool over, bool pressed)
{
	if (area.getSizeX() > 60.0f) {
		if (pressed) {
			icons[ICON_BACKGROUND_BIG_PRESSED]->bind();
		} else if (over) {
			icons[ICON_BACKGROUND_BIG_OVER]->bind();
		} else {
			icons[ICON_BACKGROUND_BIG]->bind();
		}
	} else {
		if (pressed) {
			icons[ICON_BACKGROUND_PRESSED]->bind();
		} else if (over) {
			icons[ICON_BACKGROUND_OVER]->bind();
		} else {
			icons[ICON_BACKGROUND]->bind();
		}
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(area.x1, area.y1,  0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(area.x2, area.y1,  0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(area.x2, area.y2,  0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(area.x1, area.y2,  0.0f);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	icons[type]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(area.x1, area.y1,  0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(area.x2, area.y1,  0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(area.x2, area.y2,  0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(area.x1, area.y2,  0.0f);
	glEnd();


}

void Icons::displayEffect(int type, const Rectangle<float>& area)
{
	icons[type]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(area.x1, area.y1,  0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(area.x2, area.y1,  0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(area.x2, area.y2,  0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(area.x1, area.y2,  0.0f);
	glEnd();
}
