//
// ColliderComponent.h

#pragma once

#include <string>
#include "Components.h"
#include "Quadtree/Box.h"
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
	bool isInitialSize;

public:

	size_t indexInVector;
	std::string tag;
	BoundingBox boundingBox;
	quadtree::Box<float> box;

	ColliderComponent(std::string t);
	ColliderComponent(std::string t, BoundingBox boundingBox);
		
	void init() override;

	void update(GLFWwindow* window) override;
};