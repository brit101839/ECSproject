//
// KeyboardController.h

#pragma once

#include "Components.h"
#include "../vector2D/Vector2D.h"

class KeyboardController : public Component
{
private:

	TransformComponent* _transform;
	SpriteComponent* _sprite;

public:

	void init() override
	{
		_transform = &entity->getComponent<TransformComponent>();
		_sprite = &entity->getComponent<SpriteComponent>();
	}

	void onLeftMouse();
	void onKeyboard(GLFWwindow* window);

	void update(GLFWwindow* window);
};