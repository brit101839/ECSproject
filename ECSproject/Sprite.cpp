#include "Sprite.h"
#include <iostream>

Sprite::Sprite()
	:_width(0), _height(0)
{
	_textureBufferID = 0;
	setVertices(_width, _height);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height)
	:_textureBufferID(textureBufferID), _width(width), _height(height)
{
	glGenBuffers(1, &_vertexBufferID);  // 生成?????
	setVertices(_width, _height);
}

Sprite::~Sprite()
{
	// 清除 VBO 和?放?源
	if (_vertexBufferID != 0)
	{
		glDeleteBuffers(1, &_vertexBufferID);
	}
}

void Sprite::setVertices(GLfloat width, GLfloat height)
{
	// ?置每? Sprite 的????（根据不同的?高）
	_vertices[0] = { { -width, -height, 0.0f }, { 0.0f, 1.0f } };    // 左下角
	_vertices[1] = { { width, -height, 0.0f }, { 1.0f, 1.0f } };   // 右下角
	_vertices[2] = { { width, height, 0.0f }, { 1.0f, 0.0f } }; // 右上角
	_vertices[3] = { { -width, height, 0.0f }, { 0.0f, 0.0f } };  // 左上角

	// ?定???并?????据
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解?
}

GLuint Sprite::loadAndBufferImage(const char* filename)
{
	int width, height, nrChannels;
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

	_textureBufferID = textureBufferID;

	return textureBufferID;
}

void Sprite::render(GLfloat pos_x, GLfloat pos_y, GLfloat rotation)
{
	// std::cout << "VBO ID: " << _vertexBufferID << std::endl;
	glLoadIdentity();
	glTranslatef(pos_x, pos_y, 0);
	glRotatef(rotation, 0, 0, 1.0f);

	// 重置顏色為白色，避免受到之前設置的顏色影響
	glColor3f(1.0f, 1.0f, 1.0f);  // RGB = (1, 1, 1) -> 白色

	// ?定?前 Sprite 的?????
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);  // ?定?前 Sprite 的 VBO
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData), (void*)(sizeof(GLfloat) * 3));

	glBindTexture(GL_TEXTURE_2D, _textureBufferID);

	glDrawArrays(GL_QUADS, 0, 4);

	// 禁用???性
	glDisableVertexAttribArray(0);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

