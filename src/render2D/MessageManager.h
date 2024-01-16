#ifndef __MODEL_MESSAGE__
#define __MODEL_MESSAGE__

#define MESSAGE_TIME 5000
#define MESSAGE_TYPE_INFORMATION 1
#define MESSAGE_TYPE_WARNING 2
#define MESSAGE_TYPE_ERROR 3

#include "FontManager.h"

class Message {
public:
	int time;
	int type;
	int id;
	Message(int id, int type) : id(id), type(type), time(MESSAGE_TIME) {}
	~Message() {}
};

class MessageManager
{
public:
	MessageManager(const Point2D<float>& position) : position(position) { }
	~MessageManager();

	void display();
	void update(int timeSpent);

	void addMessage(const string& message, int type = MESSAGE_TYPE_INFORMATION);

protected:
	Point2D<float> position;
	vector<Message> messages;
	MessageManager (const MessageManager& messageManager) {}
};

class MessageDisplayor {
private:
	int line;
	float lineSize;
public:
	MessageDisplayor(float lineSize) : line(0), lineSize(lineSize) { 
	}

	void operator()(const Message& message) {
		glPushMatrix();
		glTranslatef(0.0f, line*lineSize, 0.0f);
		float alpha;
		if (message.time < 1000) {
			alpha = message.time / 1000.0f;
		} else {
			alpha = 1.0f;
		}
		switch (message.type) {
			case MESSAGE_TYPE_INFORMATION:
				glColor4f(1.0f, 1.0f, 0.6,alpha);
				break;
			case MESSAGE_TYPE_WARNING:
				glColor4f(1.0f, 0.8f, 0.6f,alpha);
				break;
			case MESSAGE_TYPE_ERROR:
				glColor4f(1.0f, 0.6f, 0.6f,alpha);
				break;
			default: 
				glColor4f(0.7f, 0.7f, 0.7f,alpha);
				break;
		}
		glCallList(message.id);
		glPopMatrix();
		line++;
	}
};

class MessageUpdator {
	int timeSpent;
public:
	MessageUpdator(int timeSpent) : timeSpent(timeSpent) {}
	bool operator()(Message& message) {
		message.time -= timeSpent;
		if (message.time <= 0) {
			glDeleteLists(message.id,1);
			return true;
		}
		return false;
	}
};

class MessageDeletor {
public:
	void operator()(Message& message) {
		glDeleteLists(message.id,1);
	}
};

#endif
