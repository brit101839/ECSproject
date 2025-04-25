#include "Sprite.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sprite::Sprite()
	:_width(0), _height(0)
{
	_textureID = 0;
	setVertices(_width, _height);
}

Sprite::Sprite(GLfloat width, GLfloat height)
	:_textureID(0), _width(width), _height(height)
{
	setVertices(_width, _height, Origin::TopLeft);
	setVAO(false);
}

Sprite::Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, Origin origin)
	:_textureID(textureBufferID), _width(width), _height(height), _origin(origin)
{
	setVertices(_width, _height, _origin);
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

void Sprite::setVertices(GLfloat width, GLfloat height, Origin origin)
{
	float offsetX = 0.0f, offsetY = 0.0f;

	switch (origin) {
	case Origin::Center:
		offsetX = 0.0f; offsetY = 0.0f;
		break;
	case Origin::TopLeft:
		offsetX = width / 2.0f; offsetY = -height / 2.0f;
		break;
	case Origin::TopRight:
		offsetX = -width / 2.0f; offsetY = -height / 2.0f;
		break;
	case Origin::BottomLeft:
		offsetX = width / 2.0f; offsetY = height / 2.0f;
		break;
	case Origin::BottomRight:
		offsetX = -width / 2.0f; offsetY = height / 2.0f;
		break;
	}
	// positions															// colors      // texture coords
	_vertices[0] = { { offsetX + width / 2, offsetY + height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f } }; // top right
	_vertices[1] = { { offsetX + width / 2, offsetY - height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f } }; // bottom right
	_vertices[2] = { { offsetX - width / 2, offsetY - height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f } }; // bottom left
	_vertices[3] = { { offsetX - width / 2, offsetY + height / 2, 0.0f },{1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f } }; // top left
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
	GLfloat offsetX = _offsetX;
	if (_flip) offsetX = -_offsetX;

	model = glm::translate(model, glm::vec3(position.x + offsetX, position.y, 0.0f));
	if (_flip) {
		model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));  // Flip horizontally
	}
	else {
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  // Flip horizontally
	}
		
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	

	// glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, (float)SRC_WIDTH, 0.0f, (float)SRC_HEIGHT, -1.0f, 1.0f);

	// render container
	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("overlayColor", _overlayColor);
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::renderRectangle(Vector2D position, Vector2D scale, Shader& shader, Vector2D cameraPos, glm::vec3 color) {
	
	glBindTexture(GL_TEXTURE_2D, _textureID);

	// 設置矩形的模型矩陣
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

	// 設置視圖和投影矩陣
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
	glm::mat4 projection = glm::ortho(0.0f, (float)SRC_WIDTH, 0.0f, (float)SRC_HEIGHT, -1.0f, 1.0f);

	// 使用著色器並設置 uniform
	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("overlayColor", color);

	// 渲染矩形（假設矩形的 VAO 已初始化）
	glBindVertexArray(_VAO); // 你可以用單獨的 VAO 或與 Sprite 共用的 VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sprite::renderUI(Vector2D position, GLfloat rotation, Vector2D size, Shader& shader)
{
	// bind Texture
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));  // Flip horizontally

	// glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f); // camera dont move in UI render
	glm::mat4 projection = glm::ortho(0.0f, (float)SRC_WIDTH, (float)SRC_HEIGHT, 0.0f, -1.0f, 1.0f);

	// render container
	shader.use();
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("overlayColor", _overlayColor);
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

