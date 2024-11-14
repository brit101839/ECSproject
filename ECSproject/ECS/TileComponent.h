//
// TileComponent.h

#pragma once

#include "ECS.h"
#include "Components.h"
#include "../textureManager.h"

class TileComponent : public Component
{
private:

	Vector2D _position;
	GLfloat _width, _height;
	const char* _path;

public:

	TransformComponent* transform;
	SpriteComponent* sprite;

	TileComponent() = default;

	TileComponent(Vector2D position, GLfloat width, GLfloat height)
		:_position(position), _width(width), _height(height)
	{
		_path = "D:/dependencies/resource/redShot.png";
	}

	void init() override
	{
		entity->addcomponent<TransformComponent>(_position, Vector2D(), 0.0f, 100, 100);
		transform = &entity->getComponent<TransformComponent>();

		entity->addcomponent<SpriteComponent>(_path);
		sprite = &entity->getComponent<SpriteComponent>();
	}

};