//
// Entity.h
#include "vector2/Vector2.h"
#include "ECS/Sprite.h"

#pragma once
class GameObject
{
private:

	Vector2 _position, _velocity;
	GLfloat _rotation;
	Sprite* _sprite;

public:

	GameObject(Sprite* sprite, Vector2 position, Vector2 velocity);
	~GameObject();

	void render();
	void update();

};

