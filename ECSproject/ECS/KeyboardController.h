//
// KeyboardController.h

#pragma once

#include "Components.h"
#include "../vector2D/Vector2D.h"

class KeyboardController : public Component
{
private:

	TransformComponent* transform;

public:

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update(GLFWwindow* window)
	{
		Vector2D& vel = transform->velocity;

		vel = Vector2D(0.0f, 0.0f);

		if (glfwGetKey(window, GLFW_KEY_W)) {
			vel.y = 1.0f;
			// std::cout << "key: w" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			vel.y = -1.0f;
			// std::cout << "key: s" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			vel.x = -1.0f;
			// std::cout << "key: a" << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			vel.x = 1.0f;
			// std::cout << "key: d" << std::endl;
		}
	}
};