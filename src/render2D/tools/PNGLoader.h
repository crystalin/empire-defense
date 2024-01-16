#ifndef __PNG_LOADER__
#define __PNG_LOADER__

#include "png.h"
#include "Texture.h"

class PNGLoader
{
public:

	static Texture* ReadPNGFromFile (const string& filename, GLint filter = GL_LINEAR, GLint wrap = GL_REPEAT);

};

#endif
