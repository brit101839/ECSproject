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

Vector2D::Vector2D()
	:_vec(makeVector2(0.0f, 0.0f))
{
}

Vector2D::Vector2D(GLfloat x, GLfloat y)
	:_vec(makeVector2(x, y))
{
}

Vector2D& Vector2D::Add(const Vector2D& vec)
{
	_vec.x += vec._vec.x;
	_vec.y += vec._vec.y;
	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	_vec.x -= vec._vec.x;
	_vec.y -= vec._vec.y;
	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	_vec.x *= vec._vec.x;
	_vec.y *= vec._vec.y;
	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	_vec.x /= vec._vec.x;
	_vec.y /= vec._vec.y;
	return *this;
}

Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Subtract(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.Multiply(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.Divide(v2);
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

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec._vec.x << "," << vec._vec.y << ")";
	return stream;
	// TODO: insert return statement here
}