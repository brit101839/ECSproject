//
// textureManager.cpp

#ifndef __TextureManager__
#define __TextureManager__

#include "Sprite.h"
#include <vector>

class TextureManager
{
public:

	static GLuint loadAndBufferImage(const char* filename);
	static GLuint loadMapImage(const char* filename, int row, int column);
};

#endif // !__TextureManager__



