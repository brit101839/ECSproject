#include "textureManager.h"
#include <iostream>

GLuint TextureManager::TileSpriteManager(int id, const char* filename)
{
	GLuint tmpBufferID;
	auto it = _tileSpriteManager.find(id);

	if (it != _tileSpriteManager.end()) {
		tmpBufferID = it->second;
		return tmpBufferID;
	}
	else {
		int row, column;
		row = id % 8;
		column = (id - row) / 8;
		tmpBufferID = loadMapImage(filename, row, column);
		_tileSpriteManager.insert(std::pair<int, GLuint>(id, tmpBufferID));
		return tmpBufferID;
	}
}

GLuint TextureManager::loadAndBufferImage(const char* filename, int& width, int& height)
{
	int nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}

	// stbi_set_flip_vertically_on_load(true);

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	stbi_image_free(data);

	return textureBufferID;
}

GLuint TextureManager::loadMapImage(const char* filename, int row, int column)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}

	int regionSize = 16;

	int x_start = row * 16;
	int x_end = (row + 1) * 16 - 1;
	int y_start = column * 16;
	int y_end = (column + 1) * 16 - 1;

	// check boundary
	if (x_start < 0 || x_end >= width || y_start < 0 || y_end >= height) {
		std::cerr << "Specified region is out of bounds" << std::endl;
		stbi_image_free(data);
		return 0;
	}

	std::vector<unsigned char> regionData(regionSize * regionSize * nrChannels);

	for (int y = y_start; y <= y_end; ++y) {
		for (int x = x_start; x <= x_end; ++x) {
			for (int c = 0; c < nrChannels; ++c) {
				// 原始图像中的位置
				int srcIndex = (y * width + x) * nrChannels + c;
				// 目标区域中的位置
				int destIndex = ((y - y_start) * regionSize + (x - x_start)) * nrChannels + c;
				regionData[destIndex] = data[srcIndex];
			}
		}
	}

	stbi_image_free(data);

	unsigned char* rawData = regionData.data();

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, regionSize, regionSize, 0, format, GL_UNSIGNED_BYTE, rawData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// stbi_image_free(rawData);

	return textureBufferID;
}