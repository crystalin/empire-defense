#ifndef __PLAYER_VIEWER_H__
#define __PLAYER_VIEWER_H__

class Player;
class PlayerTemplate;
class Texture;

class PlayerViewer
{
public:
	static void displayTeam(const vector<Player*>& players);
	static void display(const Player* player);
	static void init();
	static void destroy();

protected:
	PlayerViewer();
	~PlayerViewer();
	
	static Texture* shieldTexture;
};

#endif
