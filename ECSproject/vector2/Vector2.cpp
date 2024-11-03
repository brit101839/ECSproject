//
// Vector2.cpp

#include "Vector2.h"

Vector2 makeVector2(GLfloat x, GLfloat y)
{
	Vector2 newVector;
	newVector.x = x;
	newVector.y = y;
	return newVector;
}

Vector2 addVector2(Vector2 first, Vector2 second)
{
	Vector2 newVector;
	newVector.x = first.x + second.x;
	newVector.y = first.y + second.y;
	return newVector;
}