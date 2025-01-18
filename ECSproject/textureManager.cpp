#include "textureManager.h"
#include <iostream>

GLuint TextureManager::genWhiteTexture()
{
	GLuint whiteTexture;
	unsigned char whitePixel[3] = { 255, 255, 255 };
	glGenTextures(1, &whiteTexture);
	glBindTexture(GL_TEXTURE_2D, whiteTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)whitePixel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return whiteTexture;
}

GLuint TextureManager::textureManager(const char* filename, int& width, int& height)
{
	GLuint tmpBufferID;
	std::string s = filename;
	auto it = _tileSpriteManager.find(s);

	if (it != _tileSpriteManager.end()) {
		// std::cout << "exited: " << s << std::endl;
		tmpBufferID = it->second.textureID;
		width = it->second.width;
		height = it->second.height;
		//std::cout << "texture ID: " << tmpBufferID << std::endl;
		return tmpBufferID;
	}
	else {
		tmpBufferID = loadAndBufferImage(filename, width, height);
		TextureData textureData = { tmpBufferID, width, height };
		_tileSpriteManager.insert(std::pair<std::string, TextureData>(s, textureData));
		/*std::cout << "Inserted file: " << s << ", Map size: " << _tileSpriteManager.size() << std::endl;
		std::cout << "texture ID: " << tmpBufferID << std::endl;*/
		return tmpBufferID;
	}
}

GLuint TextureManager::loadAndBufferImage(const char* filename, int& width, int& height)
{
	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/

	stbi_set_flip_vertically_on_load(false);

	int nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}

	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return textureBufferID;
}