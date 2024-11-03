//
// Vector2.h

#ifndef __Vector2__
#define __Vector2__

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct {

	GLfloat x;
	GLfloat y;

} Vector2;

Vector2 makeVector2(GLfloat x, GLfloat y);
Vector2 addVector2(Vector2 first, Vector2 second);

#endif /* __Vector2__ */