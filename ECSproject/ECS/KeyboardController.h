//
// KeyboardController.h

#pragma once

#include "ECS.h"
#include "Components.h"
#include "../vector2D/Vector2D.h"

class KeyboardController : public Component
{
private:

	MovementComponent* posComponent;

public:

	void init() override
	{
		posComponent = &entity->getComponent<MovementComponent>();
	}

	void update(GLFWwindow* window)
	{
		posComponent->setVelocity(Vector2D(0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_W)) {
			posComponent->setVelocity_y(1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_S)) {
			posComponent->setVelocity_y(-1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_A)) {
			posComponent->setVelocity_x(-1.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			posComponent->setVelocity_x(1.0f);
		}
	}
};