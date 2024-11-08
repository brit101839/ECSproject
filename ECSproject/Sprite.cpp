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
	glGenBuffers(1, &_vertexBufferID);  // 生成VBO
	setVertices(_width, _height);
}

Sprite::~Sprite()
{
	if (_vertexBufferID != 0)
	{
		glDeleteBuffers(1, &_vertexBufferID);
	}
}

void Sprite::setVertices(GLfloat width, GLfloat height)
{
	_vertices[0] = { { -width/2, -height/2, 0.0f }, { 0.0f, 1.0f } };    // 左下角
	_vertices[1] = { { width/2, -height/2, 0.0f }, { 1.0f, 1.0f } };   // 右下角
	_vertices[2] = { { width/2, height/2, 0.0f }, { 1.0f, 0.0f } }; // 右上角
	_vertices[3] = { { -width/2, height/2, 0.0f }, { 0.0f, 0.0f } };  // 左上角

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::render(Vector2D position, GLfloat rotation)
{
	// std::cout << "VBO ID: " << _vertexBufferID << std::endl;
	glLoadIdentity();
	glTranslatef(position.x, position.y, 0);
	glRotatef(rotation, 0, 0, 1.0f);

	// 重置顏色為白色，避免受到之前設置的顏色影響
	glColor3f(1.0f, 1.0f, 1.0f);  // RGB = (1, 1, 1) -> 白色

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);  // setting Sprite VBO
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

