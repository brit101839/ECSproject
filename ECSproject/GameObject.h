//
// Entity.h
#include "vector2D/Vector2D.h"
#include "Sprite.h"

#pragma once
class GameObject
{
private:

	Vector2D _position, _velocity;
	GLfloat _rotation;
	Sprite* _sprite;

public:

	GameObject(Sprite* sprite, Vector2D position, Vector2D velocity);
	~GameObject();

	void render();
	void update();

};

