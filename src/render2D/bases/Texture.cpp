#include "render2D_common.h"
#include "Texture.h" 


Texture::Texture() :
width(16), height(16) , format(GL_RGBA), internalFormat(4), filter(GL_LINEAR), wrap(GL_REPEAT), topRightAlpha(1.0f,1.0f)
{
	texels = new GLubyte[width * height * internalFormat];
	for (int j=0 ; j < height; j++) {
		for (int i=0 ; i < width ; i++) {
			for (int c=0; c<internalFormat; c++) {
				if (i==j || i==width-j && (c==2 || c==3)) { // draw a red cross
					texels[(j*width+i)*internalFormat+c] = 0;
				} else {
					texels[(j*width+i)*internalFormat+c] = 0xFF;
				}
			}
		}
	}
	glGenTextures (1, &id);
	finalize();
}

Texture::Texture(GLsizei width, GLsizei height) :
width(width), height(height) , format(GL_RGBA), internalFormat(4), filter(GL_LINEAR), wrap(GL_REPEAT), topRightAlpha(1.0f,1.0f)
{
	texels = new GLubyte[width * height * internalFormat];
	memset(texels,0xFF,width * height * internalFormat * sizeof(GLubyte));
	glGenTextures (1, &id);
	finalize();
}

Texture::Texture(GLsizei width, GLsizei height, GLenum format, GLint internalFormat, GLenum dataType, GLubyte *texels, GLint filter, GLint wrap) :
width(width), height(height), format(format), dataType(dataType), internalFormat(internalFormat), texels(texels), id(0), count(0), filter(filter), wrap(wrap), topRightAlpha(1.0f,1.0f)
{
	glGenTextures (1, &id);
	int error = glGetError();
	//printf("glGenTextures [%d] [error : %d] [w:%d, h:%d]\n", id, error, width, height);
	finalize();
}

Texture::Texture(GLsizei width, GLsizei height, GLenum format, GLint internalFormat, GLint filter, GLint wrap) :
width(width), height(height), format(format), internalFormat(internalFormat), dataType(GL_UNSIGNED_BYTE), id(0), count(0), filter(filter), wrap(wrap), topRightAlpha(1.0f,1.0f)
{
	texels = new GLubyte[width * height * internalFormat];
	glGenTextures (1, &id);
	int error = glGetError();
	//printf("glGenTextures [%d] [error : %d] [w:%d, h:%d]\n", id, error, width, height);
}

Texture::~Texture() {
	//printf("deleted texture [%d]\n",id);
	glDeleteTextures(1,&id);
	delete[] texels;
}

void Texture::finalize() {

	bool foundPixel = false;
	if (width * height <= 80000 && internalFormat>3) {
		for (int j=0 ; j < height && !foundPixel; j++) {
			int lastPixel = 0;
			for (int i=0 ; i < width; i++) {
				if (texels[(j*width+i)*internalFormat+3] > 30 ) {
					foundPixel = true;
					lastPixel = i;
				}
			}
			if (foundPixel) {
				topRightAlpha.x = lastPixel/(float)width;
				topRightAlpha.y = j/(float)height;
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, texels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
}

void Texture::drawSquare(const Rectangle<float>& area, const Rectangle<float>& texArea) const
{
	bind();
	glBegin(GL_QUADS);
	glTexCoord2f(texArea.x1, texArea.y1);
	glVertex2f(area.x1, area.y1);
	glTexCoord2f(texArea.x2, texArea.y1);
	glVertex2f(area.x2, area.y1);
	glTexCoord2f(texArea.x2, texArea.y2);
	glVertex2f(area.x2, area.y2);
	glTexCoord2f(texArea.x1, texArea.y2);
	glVertex2f(area.x1, area.y2);
	glEnd();
}