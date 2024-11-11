//
// Vector2.h

#ifndef __Vector2D__
#define __Vector2D__

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Vector2D
{
private:

	// Vector2 _vec;

public:

	GLfloat x;
	GLfloat y;

	Vector2D();
	Vector2D(GLfloat x, GLfloat y);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	Vector2D& operator*(const float& i);
	Vector2D& zero();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};

#endif /* __Vector2D__ */