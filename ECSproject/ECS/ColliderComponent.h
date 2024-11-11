//
// ColliderComponent.h

#pragma once

#include <string>
#include "Components.h"

typedef struct boundingBox {

	Vector2D position;
	GLfloat width, height;

} BoundingBox;

class ColliderComponent : public Component
{
private:

	std::string _tag;
	TransformComponent* trans;

public:

	BoundingBox boundingBox;

	ColliderComponent(std::string t) 
		:_tag(t)
	{

	}


	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addcomponent<TransformComponent>();
		}
		trans = &entity->getComponent<TransformComponent>();
	}

	void update(GLFWwindow* window) override
	{
		boundingBox.position = trans->position;
		boundingBox.width = trans->width;
		boundingBox.height = trans->height;
	}
};