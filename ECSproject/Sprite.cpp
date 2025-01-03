﻿#include "Sprite.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sprite::Sprite()
	:_width(0), _height(0)
{
	_textureID = 0;
	setVertices(_width, _height);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height)
	:_textureID(textureBufferID), _width(width), _height(height)
{
	setVertices(_width, _height);
	setVAO(false);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight)
	:_textureID(textureBufferID), _width(width), _height(height), _textureWidth(textureWidth), _textureHeight(textureHeight), _cutWidth(cutWidth), _cutHeight(cutHeight)
{
	setVertices(_width, _height);
	setVAO(true);
	updateVertex(0, 13);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight, int id)
	:_textureID(textureBufferID), _width(width), _height(height), _textureWidth(textureWidth), _textureHeight(textureHeight), _cutWidth(cutWidth), _cutHeight(cutHeight)
{
	setVertices(_width, _height);
	setVAO(false);
	updateVertex(id, 8);
}

Sprite::~Sprite()
{
	if (_VBO != 0)
	{
		glDeleteBuffers(1, &_VBO);
	}
}

void Sprite::setVAO(bool animated)
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	if(animated) glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_DYNAMIC_DRAW);
	else glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	if (animated) glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_DYNAMIC_DRAW);
	else glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sprite::setVertices(GLfloat width, GLfloat height)
{
						// positions					// colors          // texture coords
	_vertices[0] = { {  width / 2,  height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f } }; // top right
	_vertices[1] = { {  width / 2, -height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f } }; // bottom right
	_vertices[2] = { { -width / 2, -height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f } }; // bottom left
	_vertices[3] = { { -width / 2,  height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f } }; // top left
}

void Sprite::setVertices(GLfloat width, GLfloat height, int frameX, int frameY, int textureWidth, int textureHeight)
{
	GLfloat texOffset = 0.5f / textureWidth; // 每个 texel 的偏移值
	// 动态计算纹理坐标
	float frameWidth = _cutWidth / textureWidth;  // 每帧宽占整个纹理的比例
	float frameHeight = _cutHeight / textureHeight; // 每帧高占整个纹理的比例

	float u0 = frameX * frameWidth + texOffset;          // 左下 U 坐标
	float v0 = frameY * frameHeight + texOffset;         // 左下 V 坐标
	float u1 = u0 + frameWidth - texOffset;              // 右上 U 坐标
	float v1 = v0 + frameHeight - texOffset;             // 右上 V 坐标

	_vertices[0] = { { -width / 2, -height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { u0, v1 } }; // 左下角
	_vertices[1] = { {  width / 2, -height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { u1, v1 } }; // 右下角
	_vertices[2] = { {  width / 2,  height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { u1, v0 } }; // 右上角
	_vertices[3] = { { -width / 2,  height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { u0, v0 } }; // 左上角

	// update VBO data
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_vertices), _vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::setTile(int index, int tilePerRow)
{
}

void Sprite::render(Vector2D position, GLfloat rotation, Shader& shader, Vector2D cameraPos)
{
	// bind Texture
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	if (_flip) {
		model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));  // Flip horizontally
	}

	// glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, (float)Window_w_Size, 0.0f, (float)Window_h_Size, -1.0f, 1.0f);

	// render container
	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::updateAnimateVertex(int frameIndex, int tileY, int framePerRow)
{
	int tileX = frameIndex % framePerRow;      // 计算当前帧的列号
	setVertices(_width, _height, tileX, tileY, _textureWidth, _textureHeight);
}

void Sprite::updateVertex(int index, int tilePerRow)
{
	int tileX = index % tilePerRow;      // 计算当前帧的列号
	int tileY = index / tilePerRow;      // 计算当前帧的行号
	setVertices(_width, _height, tileX, tileY, _textureWidth, _textureHeight);
}

