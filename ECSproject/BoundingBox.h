#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vector2D/Vector2D.h"

class BoundingBox {
public: 
	Vector2D position;
	GLfloat width, height;

	BoundingBox(Vector2D pos = Vector2D(0.f, 0.f), GLfloat w = 0.f, GLfloat h = 0.f)
		: position(pos), width(w), height(h) {
	}
};