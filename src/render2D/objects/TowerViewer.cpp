#include "render2D_common.h"
#include "TowerViewer.h" 
#include "tower.h" 
#include "ammo.h" 
#include "texture.h"
#include "player.h"
#include "RessourceManager.h"
#include "ColorManager.h"
#include "RoundAnimation.h"
#include "SmokeGenerator.h"


Texture* TowerViewer::towerTextures[TOWER_MAX_LEVEL] = {NULL};
Texture* TowerViewer::shadowTextures[TOWER_MAX_LEVEL] = {NULL};
Texture* TowerViewer::ammoTextures[AMMO_MAX_TYPE] = {NULL};
Texture* TowerViewer::loadingTexture = NULL;
Texture* TowerViewer::flagTexture = NULL;

RoundAnimation* TowerViewer::spellAnimation = NULL;


void TowerViewer::init()
{
	towerTextures[0] = RessourceManager::instance()->getTexture("tower-lvl1.png");
	towerTextures[0]->request();
	shadowTextures[0] = RessourceManager::instance()->getTexture("tower-shadow.png");
	shadowTextures[0]->request();
	towerTextures[1] = RessourceManager::instance()->getTexture("tower-lvl2.png");
	towerTextures[1]->request();
	shadowTextures[1] = RessourceManager::instance()->getTexture("tower-shadow.png");
	shadowTextures[1]->request();
	towerTextures[2] = RessourceManager::instance()->getTexture("tower-lvl3.png");
	towerTextures[2]->request();
	shadowTextures[2] = RessourceManager::instance()->getTexture("tower-shadow.png");
	shadowTextures[2]->request();

	ammoTextures[AMMO_TEXTURE_ARROW] = RessourceManager::instance()->getTexture("icon-arrow.png");
	ammoTextures[AMMO_TEXTURE_MISSILE] = RessourceManager::instance()->getTexture("icon-missile.png");
	ammoTextures[AMMO_TEXTURE_BALL] = RessourceManager::instance()->getTexture("icon-ball.png");
	ammoTextures[AMMO_TEXTURE_LASER] = RessourceManager::instance()->getTexture("icon-laser.png");

	ammoTextures[AMMO_TEXTURE_ARROW]->request();
	ammoTextures[AMMO_TEXTURE_MISSILE]->request();
	ammoTextures[AMMO_TEXTURE_BALL]->request();
	ammoTextures[AMMO_TEXTURE_LASER]->request();

	loadingTexture = RessourceManager::instance()->getTexture("tower-loading.png");
	loadingTexture->request();

	flagTexture = RessourceManager::instance()->getTexture("flag.png");
	flagTexture->request();

	spellAnimation = new RoundAnimation(1000,1,Rectangle<float>(-0.15f,-0.1f,0.15f,0.1f),1.0,Color(1.0f, 0.0f, 0.0f, 1.0f));
	
}
void TowerViewer::destroy()
{
	for (int i=0; i < TOWER_MAX_LEVEL; i++) {
		towerTextures[i]->release();
		shadowTextures[i]->release();
	}
	ammoTextures[AMMO_TEXTURE_ARROW]->release();
	ammoTextures[AMMO_TEXTURE_LASER]->release();
	ammoTextures[AMMO_TEXTURE_MISSILE]->release();
	ammoTextures[AMMO_TEXTURE_BALL]->release();

	loadingTexture->release();
	flagTexture->release();

	delete spellAnimation;
}

void TowerViewer::update(Tower* tower, SmokeGenerator* smokeGenerator)
{
	int buildingTimeLeft = tower->getBuildingTimeLeft();
	if (buildingTimeLeft > 0) {
		const Point2D<float>& position = tower->getPosition();
		Rectangle<float> area = Rectangle<float>(position.x - 0.4, position.y - 0.4, position.x + 0.4f, position.y + 0.4f);
		smokeGenerator->addSmoke((int)tower % SMOKE_PACK_NUMBER, area, 1);
	}
}

/*void TowerViewer::displayInfo(const TowerTemplate* towerTemplate) {
	towerTemplate->id;
}*/

void TowerViewer::display(const TowerTemplate* towerTemplate)
{
	const AmmoTemplate& ammoTemplate = towerTemplate->ammoTemplate;
	
	int level = min(TOWER_MAX_LEVEL-1,max(0,towerTemplate->level));
	towerTextures[level]->drawSquare(Rectangle<float>(-0.5f, -0.5f, 0.5f, 0.5f));
	
	Texture* ammoTexture = NULL;
	float levelSize = 0.5f;
	if (level < 1) {
		levelSize = 0.35f;
	}
	switch (ammoTemplate.type) {
		case TYPE_AMMO_ARROW : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_ARROW];
			break;
		case TYPE_AMMO_MISSILE : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_MISSILE];
			break;
		case TYPE_AMMO_PROJECTILE : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_BALL];
			break;
		case TYPE_AMMO_LASER : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_LASER];
			break;
		default : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_ARROW];
			break;

	}
	ammoTexture->drawSquare(Rectangle<float>(-levelSize, -levelSize, levelSize, levelSize));
}

void TowerViewer::display(Tower* tower)
{
	int level = min(TOWER_MAX_LEVEL-1,max(0,tower->getLevel()));
	const EffectTemplate* effectTemplate = tower->getEffectTemplate();
	const AmmoTemplate& ammoTemplate = tower->getAmmoTemplate();
	const Player* player = tower->getOwner();
	int buildingTimeLeft = tower->getBuildingTimeLeft();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f) ;
	if (buildingTimeLeft > 0) {
		float pourcentBuilding = 1 - (buildingTimeLeft / (float)tower->getBuildingTime());
		//shadowTextures[level]->drawSquare(Rectangle<float>(-0.8f, -0.85f, 0.2f, 0.15f));
		towerTextures[level]->drawSquare(Rectangle<float>(-0.5f, -0.5f, 0.5f, pourcentBuilding * 0.5f));
	} else {
		shadowTextures[level]->drawSquare(Rectangle<float>(-0.8f, -0.85f, 0.2f, 0.15f));
		towerTextures[level]->drawSquare(Rectangle<float>(-0.5f, -0.5f, 0.5f, 0.5f));
	}

	float loadingPourcent = tower->getLoadingPourcent();
	
	Texture* ammoTexture = NULL;
	float levelSize = 0.4f;
	if (level < 1) {
		levelSize = 0.30f;
	}
	switch (ammoTemplate.type) {
		case TYPE_AMMO_ARROW : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_ARROW];
			break;
		case TYPE_AMMO_MISSILE : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_MISSILE];
			break;
		case TYPE_AMMO_PROJECTILE : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_BALL];
			break;
		case TYPE_AMMO_LASER : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_LASER];
			break;
		default : 
			ammoTexture = ammoTextures[AMMO_TEXTURE_ARROW];
			break;

	}
	if (buildingTimeLeft <= 0) {
		if (effectTemplate != NULL) {
			//Icons::displayEffect(effectIcon[effectTemplate->type],Rectangle<float>(-0.4f,-0.4f,0.0f,0.0f)) ;
			glColor4fv(ColorManager::getEffectColor(effectTemplate->type).values);
		}
		ammoTexture->drawSquare(Rectangle<float>(-levelSize-0.12f, -levelSize-0.12f, levelSize-0.12f, levelSize-0.12f));
		
		//glColor4f(0.7f, 0.7f, 1.0f, 1.0f) ;
		glColor4fv(ColorManager::getTeamColor(player->getTeam()).values);
		glPushMatrix();
		glRotatef(-90,0,0,1);
		flagTexture->drawSquare(Rectangle<float>(0.25 - levelSize, levelSize - 0.20f, 0.55f - levelSize, levelSize + 0.10f));
		glPopMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f) ;
	}

	if (tower->getEffectTemplate() != NULL) {
		//spellAnimation->display();
	}

	if (loadingPourcent>0.05f) {
		//cout << "loadingPourcent : " << loadingPourcent << endl;
		loadingTexture->drawSquare(Rectangle<float>(-0.3f, levelSize, -0.3f + loadingPourcent/1.8f, levelSize + 0.15f),Rectangle<float>(0, 0, loadingPourcent, 1.0f));
	}

			
}
