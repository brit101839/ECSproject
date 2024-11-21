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

	void update(GLFWwindow* window)
	{
		Vector2D& vel = _transform->velocity;

		vel = Vector2D(0.0f, 0.0f);
		_sprite->setAnimate("idle");

		if (glfwGetKey(window, GLFW_KEY_W)) {
			vel.y = 1.0f;
			_sprite->setAnimate("walkUp");
			// std::cout << "key: w" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			vel.y = -1.0f;
			_sprite->setAnimate("walkLR");
			// std::cout << "key: s" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			vel.x = -1.0f;
			_sprite->setAnimate("walkLR");
			// std::cout << "key: a" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			vel.x = 1.0f;
			_sprite->setAnimate("walkLR");
			// std::cout << "key: d" << std::endl;
		}
	}
};