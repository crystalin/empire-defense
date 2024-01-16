#include "render2D_common.h"
#include "RessourceManager.h"
#include "PNGLoader.h"

RessourceManager* RessourceManager::ressourceManager = NULL;

RessourceManager::RessourceManager(const string&  ressourcePath) : ressourcePath(ressourcePath), texturesMap()
{
	if (this->ressourcePath[ressourcePath.length()-1] != '/') {
		this->ressourcePath += '/';
	}
	cout << "[RessourceManager] ressourcePath : " << this->ressourcePath << endl;
}

RessourceManager::~RessourceManager()
{
	for (map<string, Texture*>::const_iterator ci = texturesMap.begin(); ci != texturesMap.end(); ++ci) {
		Texture* texture = ci->second;
		if (texture->getCount() > 1) {
			cerr << "Texture [\"" << ci->first << "\"] is not well release [count = " << texture->getCount() - 1 << "]" << endl;
			while (texture->getCount()>1) {
				texture->release();
			}
		}
		texture->release();
	}
	texturesMap.clear();
}

RessourceManager* RessourceManager::instance()
{
	if (ressourceManager==NULL) {
		ressourceManager = new RessourceManager();
	}
	return ressourceManager;
}

void RessourceManager::init(const string& ressourcePath)
{
	if (ressourceManager != NULL) {
		cerr << "[RessourceManager] init() already called before" << endl;
	} else {
		ressourceManager = new RessourceManager(ressourcePath);
	}
}

Texture* RessourceManager::loadTexture(string textureName) 
{
	string::size_type idx;
	Texture* texture;

	idx = textureName.rfind('.');

	if (idx != string::npos) {
		string extension = textureName.substr(idx+1);
		if (extension.compare("png") == 0) {
			texture = PNGLoader::ReadPNGFromFile(ressourcePath + textureName);
		}
	} else {
		cerr << "[RessourceManager] Error : no extension for the file \"" << textureName << "\"" << endl;
	}
	texture->request();
	return texture;
}



Texture* RessourceManager::getTexture(string textureName) 
{
	Texture* texture = NULL;
	map<string,Texture*>::const_iterator texIT= texturesMap.find(textureName);
	if(texIT == texturesMap.end()) {
		texture = loadTexture(textureName);
		texturesMap[textureName]=texture;
	} else {
		texture = texIT->second;
	}
	return texture;
}

string RessourceManager::getFilePath(string filename) 
{
	return ressourcePath + filename;
}