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

	TextureManager() {}

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	std::map<const char*, GLuint > _tileSpriteManager;

public:

	static TextureManager& getTnstance() {
		static TextureManager instance;
		return instance;
	}

	GLuint SpriteManager(const char* filename, int& width, int& height);
	static GLuint loadAndBufferImage(const char* filename, int& width, int& height);
	// static GLuint loadMapImage(const char* filename, int row, int column);
};

#endif // !__TextureManager__



