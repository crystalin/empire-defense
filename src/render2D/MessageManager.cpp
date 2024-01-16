#include "render2D_common.h"
#include "MessageManager.h" 
#include "FontManager.h" 



MessageManager::~MessageManager() {
	for_each(messages.begin(), messages.end(), MessageDeletor());
}

void MessageManager::display() {
	if (messages.empty()) {
		return;
	}
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	for_each(messages.rbegin(), messages.rend(), MessageDisplayor(FontManager::getFont("normal")->getLineHeight()));
	glPopMatrix();
}

void MessageManager::update(int timeSpent) {
	messages.erase(remove_if (messages.begin(),messages.end(),MessageUpdator(timeSpent)),messages.end());
}
void MessageManager::addMessage(const string& message, int type) {

	const Font* font = FontManager::getFont("normal");

	if (font == NULL) {
		cout << "[MessageManager] no font found" << endl;
		return;
	}

	
	int id = glGenLists(1);
	glNewList(id,GL_COMPILE);
	font->renderString(0,0,message);
	glEndList(); 
	
	messages.push_back(Message(id,type));
}
