﻿#include "Sprite.h"
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
	// cteate VBO
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

	setVertices(_width, _height);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight)
	:_textureBufferID(textureBufferID), _width(width), _height(height), _textureWidth(textureWidth), _textureHeight(textureHeight), _cutWidth(cutWidth), _cutHeight(cutHeight)
{
	animateInit();
	updateVertex(0, 13, true);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight, int id)
	:_textureBufferID(textureBufferID), _width(width), _height(height), _textureWidth(textureWidth), _textureHeight(textureHeight), _cutWidth(cutWidth), _cutHeight(cutHeight)
{
	// cteate VBO
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

	updateVertex(id, 8, false);
}

Sprite::~Sprite()
{
	if (_vertexBufferID != 0)
	{
		glDeleteBuffers(1, &_vertexBufferID);
	}
}

void Sprite::animateInit()
{
	// 创建 VAO
	glGenVertexArrays(1, &_vaoID);
	glBindVertexArray(_vaoID);

	// 创建 VBO
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);

	// 初始化顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_DYNAMIC_DRAW);

	// 设置顶点属性
	glEnableVertexAttribArray(0); // 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

	glEnableVertexAttribArray(1); // 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(GLfloat) * 3));

	// 解绑 VAO 和 VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::setVertices(GLfloat width, GLfloat height)
{
	_vertices[0] = { { -width/2, -height/2, 0.0f }, { 0.0f, 1.0f } };    // 左下角
	_vertices[1] = { { width/2, -height/2, 0.0f }, { 1.0f, 1.0f } };   // 右下角
	_vertices[2] = { { width/2, height/2, 0.0f }, { 1.0f, 0.0f } }; // 右上角
	_vertices[3] = { { -width/2, height/2, 0.0f }, { 0.0f, 0.0f } };  // 左上角

	/*glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	// 初始化顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	// 设置顶点属性
	glEnableVertexAttribArray(0); // 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

	glEnableVertexAttribArray(1); // 纹理坐标属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(GLfloat) * 3));

	// 解绑 VAO 和 VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::setVertices(GLfloat width, GLfloat height, int frameX, int frameY, int textureWidth, int textureHeight, bool animated)
{
	GLfloat texOffset = 0.5f / textureWidth; // 每个 texel 的偏移值
	// 动态计算纹理坐标
	float frameWidth = _cutWidth / textureWidth;  // 每帧宽占整个纹理的比例
	float frameHeight = _cutHeight / textureHeight; // 每帧高占整个纹理的比例

	float u0 = frameX * frameWidth + texOffset;          // 左下 U 坐标
	float v0 = frameY * frameHeight + texOffset;         // 左下 V 坐标
	float u1 = u0 + frameWidth - texOffset;              // 右上 U 坐标
	float v1 = v0 + frameHeight - texOffset;             // 右上 V 坐标

	_vertices[0] = { { -width / 2, -height / 2, 0.0f }, { u0, v1 } }; // 左下角
	_vertices[1] = { {  width / 2, -height / 2, 0.0f }, { u1, v1 } }; // 右下角
	_vertices[2] = { {  width / 2,  height / 2, 0.0f }, { u1, v0 } }; // 右上角
	_vertices[3] = { { -width / 2,  height / 2, 0.0f }, { u0, v0 } }; // 左上角

	// update VBO data
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	if (animated) glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_DYNAMIC_DRAW);
	else glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::setTile(int index, int tilePerRow)
{
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

void Sprite::updateVertex(int index, int tilePerRow, bool animated)
{
	int tileX = index % tilePerRow;      // 计算当前帧的列号
	int tileY = index / tilePerRow;      // 计算当前帧的行号
	setVertices(_width, _height, tileX, tileY, _textureWidth, _textureHeight, animated);
}

