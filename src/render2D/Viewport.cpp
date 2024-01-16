#include "render2D_common.h"
#include "Viewport.h"
#include "FormGenerator.h"
#include "ParticleAnimation.h"
#include "LaserAnimation.h"
#include "RotatingDeformer.h"
#include "MultiAnimation.h"
#include "RessourceManager.h" 
#include "FontManager.h" 
#include "TowerViewer.h"  
#include "UnitViewer.h"
#include "Ammo.h" 
#include "Map.h" 
#include "party.h"
#include "tower.h"
#include "Shader.h" 
#include "ProjectileViewer.h" 
#include "LaserPath.h" 


Color Viewport::effectColor[EFFECT_NUMBER] = {
	Color(1.0f, 0.4f, 0.2f, 0.9f),
	Color(0.6f, 0.6f, 1.0f, 0.9f),
	Color(0.4f, 0.2f, 0.0f, 0.9f),
	Color(0.1f, 0.8f, 0.0f, 0.9f),
	Color(0.9f, 0.9f, 0.0f, 0.9f),
	Color(0.9f, 0.9f, 0.9f, 0.9f),
	Color(0.1f, 0.1f, 0.1f, 0.9f),
	Color(0.5f, 0.0f, 0.7f, 0.9f)
};

int Viewport::effectIcon[EFFECT_NUMBER] = {
	ICON_FIRE,
	ICON_ICE,
	ICON_EARTH,
	ICON_POISON,
	ICON_GOLD,
	ICON_LIGHT,
	ICON_MALEDICTION,
	ICON_ELECTRICITY
};

Viewport::Viewport(const Party& party, const Rectangle<float>& area):
	party(party), area(area), mapCoordinate(0,0), zoom(48), displayGrid(false),
	constructionMode(false), towerConstruction(NULL), mouseCoordinate(0.0f,0.0f), caseCoordinate(0.0f,0.0f),
	mouseCase(NULL), towerConstructionPath(NULL), explosionGenerator(NULL), mapTexture(NULL)
{
	animations.reserve(200);

	effectsAnimation[EFFECT_FIRE] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_FIRE]);
	effectsAnimation[EFFECT_ICE] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_ICE]);
	effectsAnimation[EFFECT_EARTH] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_EARTH]);
	effectsAnimation[EFFECT_POISON] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_POISON]);
	effectsAnimation[EFFECT_GOLD] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_GOLD]);
	effectsAnimation[EFFECT_LIGHT] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_LIGHT]);
	effectsAnimation[EFFECT_MALEDICTION] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_MALEDICTION]);
	effectsAnimation[EFFECT_ELECTRICITY] = new RoundAnimation(1000,1,Rectangle<float>(0,0,0.4f,0.4f),1.0,effectColor[EFFECT_ELECTRICITY]);

	for (int i=1; i<EFFECT_NUMBER; i++) {
		effectsAnimation[i]->update(i*1000/EFFECT_NUMBER);
	}

	// TEST
	Animation* large_target = new TextureAnimation(RessourceManager::instance()->getTexture("target-large.png"), 0.0, 1000, 1, Rectangle<float>(-0.5f,-0.5f,0.5f,0.5f),1,Color(0.5, 0.6, 0.9,1.0));
	Animation* small_target = new TextureAnimation(RessourceManager::instance()->getTexture("target-small.png"), 0.0, 1000, 1, Rectangle<float>(-0.5f,-0.5f,0.5f,0.5f),1,Color(0.6, 0.7, 1.0,0.8));
	selectedCaseAnimation = new MultiAnimation(new RotatingDeformer(large_target,1000), new RotatingDeformer(small_target,1000,false));
	//selectedCaseAnimation = new TextureAnimation(RessourceManager::instance()->getTexture("flag.png"), 0.0, 1000, 1, Rectangle<float>(0.0f,0.0f,1.0f,1.0f),1,Color(1.0, 0.3, 0.0,1.0));
	selectedCaseAnimation->stop();
	animations.push_back(selectedCaseAnimation);
	//smokeGenerator = new SmokeGenerator(3,area,Point2D<float>(0.15f,0.15f),0.4f,Color(0.8,0.6,0.4,0.4),0.0f,0.2f);
	smokeGenerator = new SmokeGenerator(2,area,Point2D<float>(0.15f,0.15f),0.6f,Color(0.8,0.6,0.4,1.0f),0.0f,0.2f);
	towerSmokeGenerator = new SmokeGenerator(2,area,Point2D<float>(0.15f,0.15f),0.6f,Color(0.8,0.7,0.6,1.0f),0.0f,0.2f);
	
	animations.push_back(smokeGenerator);
	animations.push_back(towerSmokeGenerator);

	towerConstructionPath = new LaserPath(1000,1,0.0f,2,10,0.35,Color(1.0f, 1.0f, 0.3f, 0.8f));
	towerConstructionPath->addPoint(Point2D<float>(-0.4f, -0.4f));
	towerConstructionPath->addPoint(Point2D<float>(0.4f, -0.4f));
	towerConstructionPath->addPoint(Point2D<float>(0.4f, 0.4f));
	towerConstructionPath->addPoint(Point2D<float>(-0.4f, 0.4f));
	towerConstructionPath->addPoint(Point2D<float>(-0.4f, -0.4f));
	unitShader = new Shader(RessourceManager::instance()->getFilePath("vertex.txt").c_str() , RessourceManager::instance()->getFilePath("fragment.txt").c_str());
}

Viewport::~Viewport(void)
{

	for (int i=0; i<EFFECT_NUMBER; i++) {
		delete effectsAnimation[i];
	}

	if (mapTexture != NULL) {
		mapTexture->release();
	}

	//TEST
	for_each(animations.begin(),animations.end(),DeleteObject());
	animations.clear();
	delete unitShader;
	delete towerConstructionPath;
}

void Viewport::update(int timeSpent)
{
	if (party.getState() == Party::Paused) {
		return;
	}
	if (party.getState() < Party::Ingame) {
		return;
		//animations.push_back(new LaserAnimation(Point2D<float>(area.getSizeX()/zoom,area.getSizeY()/zoom),Point2D<float>(myRand(0,area.getSizeX()/zoom),myRand(0,area.getSizeY()/zoom)),1000,0,Rectangle<float>(0,0,myRand(1,2),myRand(0.25f,1)),0.01f,Color(myRand(0,0.3f),myRand(0,0.3f),myRand(0.6,1.0f),myRand(0,1.0f))));
	}

	for (int i=0; i<EFFECT_NUMBER; i++) {
		effectsAnimation[i]->update(timeSpent);
	}

	// TEST
	towerConstructionPath->update(timeSpent);
	for (vector<Animation*>::iterator ci = animations.begin(); ci != animations.end();) {
		Animation* animation =  *ci;
		animation->update(timeSpent);
		if (animation->isFinished()) {
			ci = animations.erase(ci);
			delete animation;
		} else {
			++ci;
		}
	}

	for (vector<Ammo*>::const_iterator ci = party.getAmmos().begin(); ci != party.getAmmos().end(); ++ci) {
		Ammo *ammo = *ci;
		if (ammo->getType() == TYPE_AMMO_MISSILE) {

			const EffectTemplate* effectTemplate = ammo->getEffectTemplate();
			Point2D<float> position = ammo->getPosition();

			if (timeSpent%2 != ammo->getId()%2) {
				continue;
			}
			if (position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) continue;
			if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1) continue;
			
			if (effectTemplate != NULL) {
				smokeGenerator->addSmoke((int)(ammo->getPosition().x * 100 + ammo->getPosition().y * 100) % SMOKE_PACK_NUMBER,Rectangle<float>(position.x + 0.4f, position.y + 0.4f, position.x+0.6f, position.y+0.6f),1,effectColor[effectTemplate->type]);
			} else {
				smokeGenerator->addSmoke((int)(ammo->getPosition().x * 100 + ammo->getPosition().y * 100) % SMOKE_PACK_NUMBER,Rectangle<float>(position.x + 0.4f, position.y + 0.4f, position.x+0.6f, position.y+0.6f),1);
			}

		}
	}

	const vector<Tower*>& towers = party.getTowers();
	//for_each(towers.begin(), towers.end(), bind2nd(ptr_fun(TowerViewer::update),towerSmokeGenerator));



	/*vector<Unit*>& units = party.getUnits();
	if (!units.empty()) {
		for (vector<Unit*>::iterator ci = units.begin(); ci != units.end(); ++ci) {
			Unit* unit = *ci;
			Point2D<float> position = unit->getPosition();
			if ((int)(position.x*100 + position.y*100 )% 5 == 0 && unit->getSpeed() > 0) {
				float colorIntensity = myRand(0.5, 0.6);
				animations.push_back(new ParticleAnimation(10,6,1000,0,Rectangle<float>(position.x+0.1f, position.y+0.1f, position.x+0.3f, position.y+0.3f), Point2D<float>(0.5f,0.5f), myRand(1,2), Color(colorIntensity*1.4+0.3,colorIntensity*0.8+0.3,colorIntensity*0.1+0.3,myRand(0.5,1)),myRand(-0.2f,0.2f),myRand(0,0.2f)));
			}
		}
	}*/
}


void Viewport::checkArea() {

	if (party.getState() < Party::Ingame) {
		return;
	}
	const Map* map = party.getMap();

	if (area.getSizeX() > map->getWidth()*zoom) {
		zoom = area.getSizeX()/map->getWidth();
	}
	if (area.getSizeY() > map->getHeight()*zoom) {
		zoom = area.getSizeY()/map->getHeight();
	}

	if (mapCoordinate.x < 0) {
		mapCoordinate.x = 0;
	} else if ((mapCoordinate.x + (area.getSizeX()/zoom)) > map->getWidth()) {
		mapCoordinate.x = map->getWidth() - (area.getSizeX()/zoom);
	}
	if (mapCoordinate.y < 0) {
		mapCoordinate.y = 0;
	} else if ((mapCoordinate.y + (area.getSizeY()/zoom)) > map->getHeight()) {
		mapCoordinate.y = map->getHeight() - (area.getSizeY()/zoom);
	}
}

void Viewport::setArea(const Rectangle<float>& area) {
	this->area = area;
	checkArea();
}
void Viewport::moveMapCoordinate(const Point2D<float>& movement) {
	mapCoordinate = mapCoordinate + movement;
	checkArea();
}

void Viewport::setZoom(float zoom) {
	float oldX = area.getSizeX() / this->zoom;
	float oldY = area.getSizeY() / this->zoom;

	float newX = area.getSizeX() / zoom;
	float newY = area.getSizeY() / zoom;

	float diffX = oldX - newX;
	float diffY = oldY - newY;

	mapCoordinate.x += diffX/2.0f;
	mapCoordinate.y += diffY/2.0f;

	this->zoom = zoom;
	checkArea();
}

Point2D<float> Viewport::getMapPosition(const Point2D<float>& hit) {

	float caseX = (hit.x-area.x1)/zoom + mapCoordinate.x;
	float caseY = (hit.y-area.y1)/zoom + mapCoordinate.y;
	return Point2D<float> (caseX,caseY);
}

Point2D<float> Viewport::getScreenPosition(const Point2D<float>& hit) {

	float posX = (hit.x - mapCoordinate.x) * zoom + area.x1;
	float posY = (hit.y - mapCoordinate.y) * zoom + area.y1;
	return Point2D<float> (posX,posY);
}

void Viewport::display()
{
	if (party.getState() < Party::Ingame) {
		return;
	}
	const Map* map = party.getMap();
	std::ostringstream oss;
	
	float ratioX = 1/(float)map->getWidth();
	float ratioY = 1/(float)map->getHeight();

	float originX  = ratioX * mapCoordinate.x;
	float originY  = ratioY * mapCoordinate.y;

	float destinationX = (area.getSizeX()/zoom + mapCoordinate.x) * ratioX;
	float destinationY = (area.getSizeY()/zoom + mapCoordinate.y) * ratioY;

	glColor3f(0.8f, 0.7f, 0.5f);
	
	/*unitShader->enable();
	unitShader->uniform2f("center",0.3,0.3);
	unitShader->uniform1f("time",(glutGet(GLUT_ELAPSED_TIME)%30000) / 30000.0f);*/
	mapTexture->drawSquare(area,Rectangle<float>(originX, originY, destinationX, destinationY));
	//unitShader->disable();
	glPushMatrix();
	glTranslatef(area.x1,area.y1,0.0f);

	if (displayGrid) {
		Point2D<float> decalage((mapCoordinate.x-(int)mapCoordinate.x) * zoom, (mapCoordinate.y-(int)mapCoordinate.y) * zoom);
		int caseY = (int) mapCoordinate.y;
		for (float j=0; j<area.getSizeY() + decalage.y - 1; j+= zoom, caseY++) {
			float startY = max(j - decalage.y,0.0f);
			float endY = min(j - decalage.y + zoom,area.getSizeY());

			int caseX = (int) mapCoordinate.x;
			for (float i=0; i<area.getSizeX() + decalage.x - 1; i+= zoom, caseX++) {
				float startX = max(i - decalage.x,0.0f);
				float endX = min(i - decalage.x + zoom,area.getSizeX());

				if (endX-startX>10 && endY-startY > 10) {
					const Case& tcase = map->getCase(caseX, caseY);
					Color color;
					if (tcase.getTower() != NULL) {
						color = Color(0.3, 0.3, 1.0, 0.3);
					} else if (tcase.isOnRoad()) {
						color = Color(1.0, 0.3, 0.3, 0.3);
					//} else if (tcase.isBlocked()) {
					//	color = Color(0.3, 0.3, 0.3, 0.6);
					} else {
						color = Color(0.3, 1.0, 0.3, 0.1); 
					}
					FormGenerator::drawSquare(Rectangle<float>(startX+5 , startY+5, endX-5, endY-5), color);
				}
			}
		}
	}

	glScalef(zoom,zoom,1.0f);
	glTranslatef(-mapCoordinate.x,-mapCoordinate.y,0.0f);

	//TEST
	for (vector<Animation*>::iterator ci = animations.begin(); ci != animations.end(); ++ci) {
		Animation* animation =  *ci;
		animation->display();
	}



	float sizeX = area.getSizeX();
	float sizeY = area.getSizeY();


	static int timeLeft = 0;
	timeLeft = (timeLeft + 10) % 1000;
	const vector<Unit*>& units = party.getUnits();
	for (vector<Unit*>::const_iterator ci = units.begin(); ci != units.end(); ++ci) {
		Unit* unit = *ci;
		glColor4f(1.0f-max(unit->getLifePourcent(),0.5f), min(unit->getLifePourcent(),0.5f), 0.0f, 1.0f);
		Point2D<float> position = unit->getPosition();
		if (position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) continue;
		if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1) continue;

		glPushMatrix();
		glTranslatef((position.x+0.5),(position.y+0.5),0) ;
		UnitViewer::display(unit);
		//Icons::displayEffect(ICON_MONSTER,Rectangle<float>(-0.3,-0.3,0.3,0.3));
		glPopMatrix();
	}

	// Life & Effects
	for (vector<Unit*>::const_iterator ci = units.begin(); ci != units.end(); ++ci) {
		Unit* unit = *ci;
		glColor4f(1.0f-max(unit->getLifePourcent(),0.5f), min(unit->getLifePourcent(),0.5f), 0.0f, 1.0f);


		Point2D<float> position = unit->getPosition();
		if (position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) continue;
		if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1) continue;

		glPushMatrix();
		glTranslatef((position.x+0.5),(position.y+0.5),0) ;
		//oss.str("");
		//oss << unit->getLife();
		//FontManager::instance()->displayText(oss.str().c_str(),-0.1,0.3,0.02, FONT_BOLD);
		for (int i=0 ; i<EFFECT_NUMBER; i++) {
			float effectSize = 0.35;
			const Effect* effect= unit->getEffects()[i];
			if (effect !=NULL) {
				effectsAnimation[i]->display();
			}
		}

		glPopMatrix();
	}


	if (! party.getAmmos().empty()) {
		for (vector<Ammo*>::const_iterator ci = party.getAmmos().begin(); ci != party.getAmmos().end(); ++ci) {
			Ammo *ammo = *ci;
			Point2D<float> position = ammo->getPosition();
			if (position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) continue;
			if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1) continue;

			const EffectTemplate* effectTemplate = ammo->getEffectTemplate();
			if (effectTemplate!=NULL) {
				glColor4fv(effectColor[effectTemplate->type].values);
			} else {
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (ammo->getType() == TYPE_AMMO_MISSILE) {
				glPushMatrix();
				glTranslatef((position.x+0.5),(position.y+0.5+ammo->getHeight()),0) ;
				glRotatef(ammo->getDirection(),0,0,1);
				Icons::displayEffect(ICON_AMMO,Rectangle<float>(-0.2f, -0.2f, 0.2f, 0.2f));
				glPopMatrix();
			} else {
				ProjectileViewer::display(ammo);
			}
		}
	}
	glLineWidth(2.0f);

	const vector<Tower*>& towers = party.getTowers();
	if (!towers.empty()) {
		for (vector<Tower*>::const_iterator ci = towers.begin(); ci != towers.end(); ++ci) {
			Tower* tower = *ci;

			Point2D<float> position = tower->getPosition();
			if (position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) continue;
			if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1) continue;

			glPushMatrix();
			glTranslatef((position.x+0.5f),(position.y+0.5f),0) ;

			TowerViewer::display(tower);
			/*const EffectTemplate* effectTemplate = tower->getEffectTemplate();

			//glColor4f(0.7f, 1.0f, 0.7f, 1.0f) ;
			//FormGenerator::drawCircle(tower->getVisibility());

			int iconType = ICON_TOWER_MISSILE;
			Texture* texture = NULL;

			switch (tower->getAmmoTemplate().type) {
				case TYPE_AMMO_LASER: 
					iconType = ICON_TOWER_LASER;
					texture = RessourceManager::instance()->getTexture("arrow-tower-3.png");
					break;
				case TYPE_AMMO_PROJECTILE: 
					iconType = ICON_TOWER_MISSILE; 
					texture = RessourceManager::instance()->getTexture("tower.png");
					break;
				default: 
					texture = RessourceManager::instance()->getTexture("tower-lvl2.png");
					iconType = ICON_TOWER_MISSILE; 
					break;
			}
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f) ;
			RessourceManager::instance()->getTexture("tower-shadow.png")->drawSquare(Rectangle<float>(-0.8f, -0.85f, 0.2f, 0.15f));
			texture->drawSquare(Rectangle<float>(-0.5f, -0.5f, 0.5f, 0.5f));
				
			if (effectTemplate != NULL) {
				glColor3fv(effectColor[effectTemplate->type].values);
			} else {
				glColor3f(1.0f, 1.0f, 1.0f) ;
			}
			//Icons::displayEffect(iconType,Rectangle<float>(-0.5f, -0.5f, 0.5f, 0.5f)) ;
			//texture->drawSquare(Rectangle<float>(-0.5f, -0.5f, 0.5f, 0.5f));

			if (effectTemplate != NULL) {
				glColor3f(1.0f, 1.0f, 1.0f) ;
				glPushMatrix();
				glTranslatef(-0.2f, -0.2f, 0.0f) ;
				Icons::displayEffect(effectIcon[effectTemplate->type],Rectangle<float>(-0.2f,-0.2f,0.2f,0.2f)) ;
				glPopMatrix();
			}
			//FontManager::instance()->displayText(tower->getName().c_str(),-1,0.6,0.02, FONT_BOLD);

			/*if (tower->getWeapon()->getLevel() > 1) {
				glPushMatrix();
				glTranslatef(-0.2f, -0.3f, 0.2f) ;
				for (int i=0; i<tower->getWeapon()->getLevel()-1; i++) {
					glTranslatef(0.3,0,0);
					glColor4f(0.8f, 0.5f, 0.5f, 0.6f);
					Icons::displayEffect(ICON_PLUS, 0.15f);
				}
				glPopMatrix();
			}*/
			
			glPopMatrix();
		}
	}
	if (constructionMode) {
		glPushMatrix();
		//Color& color = caseType == CASE_EMPTY ? Color(0.0f, 1.0f, 0.0f, 0.4f) : Color(1.0f, 0.4f, 0.3f, 0.4f);

		
		glPushMatrix();
		glTranslatef(caseCoordinate.x+0.5f,caseCoordinate.y+0.5f,0);
		if (mouseCase == NULL || mouseCase->getTower() || mouseCase->isOnRoad()) {
			FormGenerator::drawSquare(Rectangle<float>(-0.4f , -0.4f, 0.4f , 0.4f),Color(1.0f, 0.4f, 0.3f, 0.4f));
		} else {
			FormGenerator::drawSquare(Rectangle<float>(-0.4f , -0.4f, 0.4f , 0.4f),Color(0.0f, 1.0f, 0.0f, 0.4f));
		}
		towerConstructionPath->display();
		glPopMatrix();

		if (mouseCase != NULL && mouseCase->getTower()) {
			glColor4f(1.0f, 0.3f, 0.3f, 0.6f);
		} else {
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		}

		glTranslatef(mouseCoordinate.x,mouseCoordinate.y+0.2f,0);
		TowerViewer::display(towerConstruction);


		glPopMatrix();
	}
	
	glPopMatrix();

	
/*
	
	for (vector<Animation*>::iterator ci = animations.begin(); ci != animations.end(); ++ci) {
		(*ci)->display();
	}

	glColor4f(0.8f, 0.8f, 0.5f, 0.7f);
	if (!bonusList.empty()){
		for (vector<Bonus*>::iterator ci = bonusList.begin(); ci != bonusList.end(); ++ci) {
			Bonus *bonus = *ci;
			bonus->display();
		}
	}
	*/
}

void Viewport::requestZoomChange(int zoomType)
{
	if (zoomType == ZOOM_MORE) {
		setZoom(getZoom()+4);
	} else {
		setZoom(getZoom()-4);
	}
}

void Viewport::notifyFireAmmo(const Ammo* ammo)
{
	Point2D<float> position = ammo->getPosition();
	Point2D<float> destination = ammo->getDestination();

	if ((position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) &&
		(destination.x<mapCoordinate.x-1 || destination.x >= mapCoordinate.x + area.getSizeX()/zoom + 1)) return;
	if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1 &&
		(destination.y<mapCoordinate.y-1 || destination.y >= mapCoordinate.y + area.getSizeY()/zoom + 1)) return;

	if (ammo->getType() == TYPE_AMMO_LASER) {
		const EffectTemplate* effectTemplate = ammo->getEffectTemplate();

		/*if (effectTemplate != NULL) {
			animations.push_back(new LaserAnimation(position+0.5f,destination+0.5f,1000,0,Rectangle<float>(0,0,1,0.3),0.03f,effectColor[effectTemplate->type]));
		} else {
			animations.push_back(new LaserAnimation(position+0.5f,destination+0.5f,1000,0,Rectangle<float>(0,0,1,0.3),0.03f));
		}*/
	} else if (ammo->getType() == TYPE_AMMO_MISSILE) {
		animations.push_back(new ParticleAnimation(20,6,1000,0,Rectangle<float>(position.x, position.y, position.x+0.4f, position.y+0.4f), Point2D<float>(0.5f,0.5f), myRand(1,2), Color(0.3,0.4,0.3,myRand(0.5,1)),myRand(-0.6f,0.6f),myRand(0,0.2f)));
	} else if (ammo->getType() == TYPE_AMMO_PROJECTILE) {
		animations.push_back(new ParticleAnimation(20,6,1000,0,Rectangle<float>(position.x, position.y, position.x+0.4f, position.y+0.4f), Point2D<float>(0.5f,0.5f), myRand(1,2), Color(0.6,0.5,0.3,myRand(0.5,1)),myRand(-0.6f,0.6f),myRand(0,0.2f)));
	}
}

void Viewport::notifyAmmoHit(const Ammo* ammo) {
	float colorIntensity = myRand(0.5, 0.6);
	const Point2D<float>& position = ammo->getPosition();
	
	if (position.x<mapCoordinate.x-1 || position.x >= mapCoordinate.x + area.getSizeX()/zoom + 1) return;
	if (position.y<mapCoordinate.y-1 || position.y >= mapCoordinate.y + area.getSizeY()/zoom + 1) return;

	if (ammo->getType() == TYPE_AMMO_LASER) {
		animations.push_back(new ParticleAnimation(20,6,1000,0,Rectangle<float>(position.x, position.y, position.x+0.4f, position.y+0.4f), Point2D<float>(0.5f,0.5f), myRand(1,2), Color(colorIntensity*0.4+0.3,colorIntensity*0.8+0.3,colorIntensity*1.1+0.3,myRand(0.5,1)),myRand(-0.6f,0.6f),myRand(0,0.2f)));
	} else if (ammo->getType() == TYPE_AMMO_MISSILE) {
		animations.push_back(new ParticleAnimation(20,6,1000,0,Rectangle<float>(position.x, position.y, position.x+0.4f, position.y+0.4f), Point2D<float>(0.5f,0.5f), myRand(1,2), Color(colorIntensity*1.4+0.3,colorIntensity*0.2+0.3,colorIntensity*0.1+0.3,myRand(0.5,1)),myRand(-0.6f,0.6f),myRand(0,0.2f)));
	} else if (ammo->getType() == TYPE_AMMO_PROJECTILE) {
		animations.push_back(new TextureAnimation(RessourceManager::instance()->getTexture("hole.png"),0.0,0.5,5000,0,Rectangle<float>(position.x+0.10f,position.y+0.10f,position.x+0.90f,position.y+0.90f),1));
		animations.push_back(new ParticleAnimation(30,6,1000,0,Rectangle<float>(position.x+0.10f,position.y+0.10f,position.x+0.70f,position.y+0.70f), Point2D<float>(0.45f,0.45f), 1.1, Color(colorIntensity*1.3+0.3,colorIntensity*0.8+0.3,colorIntensity*0.3+0.2,myRand(0.5,1)),0.0f, 0.0f, 1.0f));
		explosionGenerator->addExplosion(position+0.5f,ammo->getRadius(),Point2D<float>(party.getMap()->getWidth(), party.getMap()->getHeight()));
	}
}

void Viewport::notifyBuildTower(const Tower* tower) {

	int buildingTimeLeft = tower->getBuildingTimeLeft();
	if (buildingTimeLeft > 0) {
		const Point2D<float>& position = tower->getPosition();
		Rectangle<float> area = Rectangle<float>(position.x-0.2f, position.y-0.2f, position.x + 0.6f, position.y + 0.6f);
		animations.push_back(new ParticleAnimation(100,0.3,buildingTimeLeft+400,1,area, Point2D<float>(0.5f,0.5f), 1, Color(0.8f, 0.6f, 0.4f ,0.9f),0.0f,0.3f));
	}
}

void Viewport::notifyChangeMap(const Map* map) {

	if (mapTexture != NULL) { 
		mapTexture->release();
	}
	if (explosionGenerator != NULL) {
		animations.erase(remove(animations.begin(), animations.end(), explosionGenerator));
		delete explosionGenerator;
	}

	mapTexture = RessourceManager::instance()->getTexture("maps/" + map->getName() + ".png");
	mapTexture->request();

	explosionGenerator = new ExplosionGenerator(mapTexture,30);
	animations.push_back(explosionGenerator);

	const vector<Path>& roads = map->getRoads();
	for (vector<Path>::const_iterator it = roads.begin(); it != roads.end(); it++) {
		const Path& path = (*it);
		LaserPath* laserPath = new LaserPath(1000,0,100,1,path.size(),0.6,Color(0.6f, 0.6f, 1.0f, 1.0f));
		for (vector<PathPoint>::const_reverse_iterator ci = path.rbegin(); ci != path.rend(); ci++) {
			const PathPoint& pathPoint = *ci;
			laserPath->addPoint(pathPoint.getPosition()+0.5f);
		}
		animations.push_back(laserPath);
	}
	checkArea();
}


void Viewport::notifyChooseTowerConstruction(const TowerTemplate* towerTemplate) {
	towerConstruction = towerTemplate;
	if (towerTemplate == NULL) {
		//glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		constructionMode = false;
	} else {
		//glutSetCursor(GLUT_CURSOR_NONE);
		constructionMode = true;
	}
}


void Viewport::notifySelectObject(const Object* object)
{
	if (object == NULL) {
		selectedCaseAnimation->stop();
	} else {
		Rectangle<float> animArea = selectedCaseAnimation->getArea();
		animArea.x1 = object->getPosition().x;
		animArea.y1 = object->getPosition().y ;
		animArea.x2 = object->getPosition().x + 1.0f;
		animArea.y2 = object->getPosition().y + 1.0f;
		selectedCaseAnimation->setArea(animArea);
		selectedCaseAnimation->start();

		if (object->getType() == TYPE_TOWER) {
			Tower* tower = (Tower*)object;
			LaserPath* laserPath = LaserPath::createRoundLaser(object->getPosition()+0.5f,tower->getVisibility(),3000,0,20.0f, 4.0f, 5*tower->getVisibility(), 0.5f, Color(1.0f, 0.1f, 0.1f, 1.0f));
			animations.push_back(laserPath);
		}
	}
}

void Viewport::mouseOver(const Point2D<float>& point)
{
	if (party.getState() < Party::Ingame) {
		return;
	}
	const Map* map = party.getMap();

	static int currentCaseIndex = 0;
	static Animation* currentAnimation = NULL;
	Point2D<float> position = getMapPosition(point);
	mouseCoordinate = position;
	int caseX = (int)position.x;
	int caseY = (int)position.y;

	if (caseCoordinate.x == caseX && caseCoordinate.y == caseY) {
		return;
	}

	if (caseX >= map->getWidth() || caseY >= map->getHeight() || caseX < 0 || caseY < 0) {
		return;
	}

	mouseCase = & map->getCase(caseX,caseY);
	caseCoordinate.x = caseX;
	caseCoordinate.y = caseY;

	/*if (caseType != CASE_EMPTY ) {
		return;
	}


	if (currentAnimation != NULL) {
		currentAnimation->setLoop(0);
	}

	currentAnimation = new TextureAnimation(RessourceManager::instance()->getTexture("window.png"), 0.5f, 1000, 1, Rectangle<float>(caseX,caseY,caseX+1,caseY+1),1,Color(1.0, 0.5, 0.7,0.9));
	animations.push_back(currentAnimation);*/
}