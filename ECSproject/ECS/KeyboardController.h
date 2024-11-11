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
		transform->velocity = Vector2D(0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_W)) {
			transform->velocity.y = 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			transform->velocity.y = -1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			transform->velocity.x = -1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			transform->velocity.x = 1.0f;
		}
	}
};