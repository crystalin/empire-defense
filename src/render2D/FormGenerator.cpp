#include "render2D_common.h"
#include "FormGenerator.h" 

void FormGenerator::drawBox(float x, float y, float z, float size)
{
	static int callID = -1;
	if (callID == -1) {
		callID = glGenLists(1);
		glNewList(callID,GL_COMPILE);
		glBegin(GL_QUADS);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		// BACK
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		// LEFT
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		// RIGHT
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		// TOP
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		// BOTTOM
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glEnd();
		glEndList(); 
	}
	glPushMatrix();
	glTranslatef(x,y,0) ;
	glScalef(size, size, 1);
	glCallList(callID);
	glPopMatrix();
}

void FormGenerator::drawSquare(const Rectangle<float>& area)
{
	glDisable(GL_TEXTURE_2D);
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
	glEnable(GL_TEXTURE_2D);
}

void FormGenerator::drawSquare(const Rectangle<float>& area, Color color)
{
	glColor4fv(color.values);
	glDisable(GL_TEXTURE_2D);
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
	glEnable(GL_TEXTURE_2D);
}

void FormGenerator::drawCircle(float radius)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_LOOP);
	for (int i=0; i < 360; i+=5) {
		float degInRad = i*DEG2RAD;
		glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void FormGenerator::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

