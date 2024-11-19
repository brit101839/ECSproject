//
// Sprite.h

#ifndef __Sprite__
#define __Sprite__

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb/stb_image.h"
#include "vector2D/Vector2D.h"

typedef struct {
	glm::vec3 positionCoord; // 顶点位置
	glm::vec2 textureCoord; // 纹理坐标
} VertexData;

class Sprite
{
private:

	GLfloat _width, _height, _cutWidth, _cutHeight;
	GLuint _textureBufferID;
	GLuint _vertexBufferID, _vaoID;
	VertexData _vertices[4];
	int _textureWidth, _textureHeight;

public:

	Sprite();
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height);
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight);
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight, int id);
	~Sprite();

	void animateInit();

	void setVertices(GLfloat width, GLfloat height);
	void setVertices(GLfloat width, GLfloat height, int frameX, int frameY, int textureWidth, int textureHeight, bool animated);
	void setTile(int index, int tilePerRow);

	void render(Vector2D position, GLfloat rotation);
	void updateVertex(int frameIndex, int framePerRow, bool animated);
};

#endif // !__Sprite__



