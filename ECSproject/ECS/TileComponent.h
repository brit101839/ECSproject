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
	int _id;
	const char* _path;
	TextureManager _textureManager;

public:

	TransformComponent* transform;
	SpriteComponent* sprite;

	TileComponent() = default;
	TileComponent(Vector2D position, GLfloat width, GLfloat height, int id);

	void init() override;

};