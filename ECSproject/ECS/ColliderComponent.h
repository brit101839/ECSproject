//
// ColliderComponent.h

#pragma once

#include <string>
#include "Components.h"
// #include "../Game.h"

// class Game;

typedef struct boundingBox {

	Vector2D position;
	GLfloat width, height;

} BoundingBox;

class ColliderComponent : public Component
{
private:

	TransformComponent* _trans;

public:

	std::string tag;
	BoundingBox boundingBox;

	ColliderComponent(std::string t);
		
	void init() override;

	void update(GLFWwindow* window) override;
};