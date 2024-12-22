//
// Vector2.cpp

#include "Vector2D.h"

Vector2D::Vector2D()
	:x(0.0f), y(0.0f)
{
}

Vector2D::Vector2D(GLfloat x, GLfloat y)
	:x(x), y(y)
{
}

Vector2D& Vector2D::Add(const Vector2D& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	x /= vec.x;
	y /= vec.y;
	return *this;
}

Vector2D operator+(Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D operator-(Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

Vector2D operator*(Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x * v2.x, v1.y * v2.y);
}

Vector2D operator/(Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x / v2.x, v1.y / v2.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtract(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiply(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Divide(vec);
}

Vector2D& Vector2D::operator*(const float& i)
{
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2D& Vector2D::zero()
{
	this->x = 0;
	this->y = 0;
	 
	return *this;
	// TODO: insert return statement here
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
	// TODO: insert return statement here
}