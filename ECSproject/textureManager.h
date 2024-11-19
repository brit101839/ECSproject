//
// textureManager.cpp

#ifndef __TextureManager__
#define __TextureManager__

#include "Sprite.h"
#include <vector>
#include <map>

struct TextureData {
	GLuint textureID;
	int width;
	int height;
};

class TextureManager
{
private:

	TextureManager() {}

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	std::map<std::string, TextureData> _tileSpriteManager;

public:

	static TextureManager& getTnstance() {
		static TextureManager instance;
		return instance;
	}

	GLuint textureManager(const char* filename, int& width, int& height);
	static GLuint loadAndBufferImage(const char* filename, int& width, int& height);
};

#endif // !__TextureManager__



