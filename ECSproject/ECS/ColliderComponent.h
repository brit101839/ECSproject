//
// ColliderComponent.h

#pragma once

#include <string>
#include "Components.h"
#include "Quadtree/Box.h"
#include "../BoundingBox.h"
// #include "../Game.h"

// class Game;
class CollisionManager;

//typedef struct boundingBox {
//
//	Vector2D position;
//	GLfloat width, height;
//
//} BoundingBox;

class ColliderComponent : public Component
{
private:

	CollisionManager* _colliderManager;
	TransformComponent* _trans;
	bool isInitialSize;
	Vector2D _shift;

public:

	size_t indexInVector;
	std::string tag;
	BoundingBox boundingBox;
	quadtree::Box<float> box;

	ColliderComponent(CollisionManager* colM, std::string t);
	ColliderComponent(CollisionManager* colM, std::string t, BoundingBox boundingBox, Vector2D shift = Vector2D(0.0f, 0.0f));

	quadtree::Box<float> getBox() const {
		return { {boundingBox.position.x - boundingBox.width / 2, boundingBox.position.y + boundingBox.height / 2},{boundingBox.width, boundingBox.height} };
	}
		
	void init() override;

	void update(GLFWwindow* window) override;
};