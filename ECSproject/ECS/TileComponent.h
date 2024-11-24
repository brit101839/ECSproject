//
// TileComponent.h

#pragma once

#include "ECS.h"
#include "Components.h"
#include "../shader/Shader.h"

class TileComponent : public Component
{
private:

	Vector2D _position;
	GLfloat _width, _height;
	int _id;
	const char* _path;

public:

	TransformComponent* transform;
	SpriteComponent* sprite;

	TileComponent() = default;
	TileComponent(Vector2D position, GLfloat width, GLfloat height, int id);

	void init() override;

};