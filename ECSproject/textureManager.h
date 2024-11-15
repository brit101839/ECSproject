//
// textureManager.cpp

#ifndef __TextureManager__
#define __TextureManager__

#include "Sprite.h"
#include <vector>
#include <map>

class TextureManager
{
private:

	std::map<int, GLuint > _tileSpriteManager;

public:

	GLuint TileSpriteManager(int id, const char* filename);
	static GLuint loadAndBufferImage(const char* filename);
	static GLuint loadMapImage(const char* filename, int row, int column);
};

#endif // !__TextureManager__



