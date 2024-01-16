#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Color.h"

class Texture
{
public:
	Texture();
	Texture(GLsizei width, GLsizei height);
	Texture(GLsizei width, GLsizei height, GLenum format, GLint internalFormat, GLenum dataType, GLubyte *texels, GLint filter = GL_LINEAR, GLint wrap = GL_REPEAT);
	Texture(GLsizei width, GLsizei height, GLenum format, GLint internalFormat, GLint filter = GL_LINEAR, GLint wrap = GL_REPEAT);

	GLubyte* getTexels() { return texels; };
	GLuint getID() { return id; };
	GLenum getFormat() { return format; };
	GLint getInternalFormat() { return internalFormat; };
	GLsizei getWidth() { return width; };
	GLsizei getHeight() { return height; };
	const Point2D<float>& getTopRightAlpha() { return topRightAlpha; }

	void bind() const { glBindTexture(GL_TEXTURE_2D,id); }
	void finalize();

	Texture* release() { if (--count == 0) { delete this; return NULL; } else { return this; }}
	void request() { count++; }
	int getCount() { return count; }

	void drawSquare(const Rectangle<float>& area = Rectangle<float>(0,0,32,32), const Rectangle<float>& texArea = Rectangle<float>(0,0,1,1)) const;

protected:
	~Texture();

	int count;

	GLsizei width;
	GLsizei height;

	GLenum dataType;
	GLenum format;
	GLint internalFormat;
	GLuint id;
	
	GLint filter;
	GLint wrap;

	Point2D<float> topRightAlpha;


	GLubyte *texels;

};

#endif
