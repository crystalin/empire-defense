#include "render2D_common.h"
#include "PlayerViewer.h" 
#include "player.h" 
#include "texture.h"
#include "RessourceManager.h"
#include "FontManager.h"
#include "ColorManager.h"


Texture* PlayerViewer::shieldTexture = NULL;

void PlayerViewer::init()
{
	shieldTexture = RessourceManager::instance()->getTexture("shield.png");
	shieldTexture->request();

}
void PlayerViewer::destroy()
{
	shieldTexture->release();
}

class TeamDisplayor {
private:
	int y;
	const Font* font;
	const Texture* shieldTexture;
public :
	TeamDisplayor(int y, const Texture* shieldTexture) : y(y), shieldTexture(shieldTexture) {
		font = FontManager::getFont("normal"); 
	}

	void operator()(const Player* player) {
		glColor4fv(ColorManager::getTeamColor(player->getTeam()).values);
		shieldTexture->drawSquare(Rectangle<float>(-16, y-16, 16, y+16));
		if (font != NULL) {
			glColor4fv((ColorManager::getTeamColor(player->getTeam()).contrast(0.03)).values);
			font->renderString(-12,y, player->getName(),Font::right);
		}
		y -= 35;
	}
};

void PlayerViewer::displayTeam(const vector<Player*>& players) {
	for_each(players.begin(), players.end(), TeamDisplayor(0,shieldTexture));
}

void PlayerViewer::display(const Player* player)
{
	glColor4fv(ColorManager::getTeamColor(player->getTeam()).values);
	shieldTexture->drawSquare(Rectangle<float>(-32, -32, 32, 32));
}