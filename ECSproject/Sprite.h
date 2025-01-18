//
// Sprite.h

#ifndef __Sprite__
#define __Sprite__

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "stb/stb_image.h"
#include "vector2D/Vector2D.h"
#include "shader/Shader.h"

#define Window_w_Size 1351
#define Window_h_Size 760

typedef struct {
	glm::vec3 positionCoord; 
	glm::vec3 colorCoord;
	glm::vec2 textureCoord; 
} VertexData;

class Sprite
{
private:

	GLfloat _width, _height, _cutWidth, _cutHeight;
	GLuint _textureID;
	unsigned int _VBO, _VAO, _EBO;
	VertexData _vertices[4];
	unsigned int _indices[6] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	int _textureWidth, _textureHeight;
	bool _flip = false;
	glm::vec3 _overlayColor = glm::vec3(1.0f, 1.0f, 1.0f);

	// Shader _shader = Shader("shader/shader.vert", "shader/shader.frag");
	

public:

	Sprite();
	Sprite(GLfloat width, GLfloat height);
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height);
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight);
	Sprite(GLuint textureBufferID, GLfloat width, GLfloat height, int textureWidth, int textureHeight, GLfloat cutWidth, GLfloat cutHeight, int id);
	~Sprite();

	void setFlip(bool flip) { _flip = flip; }
	bool getFlip() { return _flip; }
	void setOverlapColor(glm::vec3 newColor) { _overlayColor = newColor; }

	void setVAO(bool animated);

	void setVertices(GLfloat width, GLfloat height);
	void setVertices(GLfloat width, GLfloat height, int frameX, int frameY, int textureWidth, int textureHeight);
	void setTile(int index, int tilePerRow);

	void render(Vector2D position, GLfloat rotation, Shader& shader, Vector2D cameraPos);
	void renderRectangle(Vector2D position, Vector2D size, Shader& shader, Vector2D cameraPos, glm::vec3 color);
	void updateAnimateVertex(int frameIndex, int tileY, int framePerRow);
	void updateVertex(int index, int tilePerRow);
};

#endif // !__Sprite__



