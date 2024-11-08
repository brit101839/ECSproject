//
// Sprite.h

#ifndef __Sprite__
#define __Sprite__

#include "glad/glad.h"
#include <GLFW/glfw3.h>
// #define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "vector2D/Vector2D.h"

typedef struct {
	GLfloat positionCoordinate[3];
	GLfloat textureCoordinate[2];
} VertexData;

class Sprite
{
private:

	GLfloat _width, _height;
	GLuint _textureBufferID;
	GLuint _vertexBufferID;
	VertexData _vertices[4];


public:

	Sprite();
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height);
	~Sprite();

	void setVertices(GLfloat width, GLfloat height);
	

	void render(Vector2D position, GLfloat rotation);
};

#endif // !__Sprite__



