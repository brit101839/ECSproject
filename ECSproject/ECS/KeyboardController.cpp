#include "KeyboardController.h"
#include "../Game.h"

void KeyboardController::update(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// std::cout << "key: esc" << std::endl;
		Game::_isRunning = false;
	}

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
		_sprite->setAnimate("walkR");
		// std::cout << "key: s" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		vel.x = -1.0f;
		_sprite->setAnimate("walkL");
		// std::cout << "key: a" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		vel.x = 1.0f;
		_sprite->setAnimate("walkR");
		// std::cout << "key: d" << std::endl;
	}
}
