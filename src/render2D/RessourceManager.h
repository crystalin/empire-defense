#ifndef __RESSOURCE_MANAGER__
#define __RESSOURCE_MANAGER__

#include "Texture.h"

class RessourceManager
{
public:

	RessourceManager(const string& ressourcePath = "../ressources");
	~RessourceManager();

	Texture* getTexture(string textureName);
	string getFilePath(string filename);
	static RessourceManager* instance();
	static void init(const string& );

private:

	string ressourcePath;
	map<string,Texture*> texturesMap;

	Texture* loadTexture(string textureName);
	
	static RessourceManager* ressourceManager;
};

#endif
