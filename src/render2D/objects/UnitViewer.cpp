#include "render2D_common.h"
#include "UnitViewer.h" 
#include "unit.h" 
#include "texture.h"
#include "Shader.h"
#include "RessourceManager.h"


Texture* UnitViewer::unitTexture = NULL;
Texture* UnitViewer::unitLifeTexture = NULL;
Shader* UnitViewer::unitColorShader = NULL;

void UnitViewer::init()
{
	unitTexture = RessourceManager::instance()->getTexture("mage.png");
	unitTexture->request();
	unitLifeTexture = RessourceManager::instance()->getTexture("unit-life.png");
	unitLifeTexture->request();
	unitColorShader = new Shader(RessourceManager::instance()->getFilePath("color-border-vert.txt").c_str() , RessourceManager::instance()->getFilePath("color-border-frag.txt").c_str());
}
void UnitViewer::destroy()
{
	unitTexture->release();
	unitLifeTexture->release();
	delete unitColorShader;
}

void UnitViewer::update(Unit* unit)
{
}

void UnitViewer::display(const UnitTemplate* unitTemplate)
{
	float size = 0.4f;
	unitTexture->drawSquare(Rectangle<float>(-size, -size, size, size), Rectangle<float>(0, 0, 0.25f, 1));
}

void UnitViewer::display(Unit* unit)
{
	unitColorShader->enable();
	float life = unit->getLifePourcent();
	float size = 0.4f;
	float angle = unit->getDirection();
	if (angle < 135.0f && angle > 45.0f) {
		unitTexture->drawSquare(Rectangle<float>(-size, -size, size, size), Rectangle<float>(0.75f, 0, 1, 1));
	} else if (angle > 135.0f || angle < -135.0f) {
		unitTexture->drawSquare(Rectangle<float>(-size, -size, size, size), Rectangle<float>(0.50f, 0, 0.75f, 1));
	} else if (angle > -135.0f && angle < -45.0f) {
		unitTexture->drawSquare(Rectangle<float>(-size, -size, size, size), Rectangle<float>(0.25f, 0, 0.50f, 1));
	} else {
		unitTexture->drawSquare(Rectangle<float>(-size, -size, size, size), Rectangle<float>(0, 0, 0.25f, 1));
	}
	//unitLifeTexture->drawSquare(Rectangle<float>(-size+0.1f, 0.2f, -size+0.35f, 0.2f+(0.25f*life) ), Rectangle<float>(0, 0, 1, life));
	unitColorShader->disable();

}
