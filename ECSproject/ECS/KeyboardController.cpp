#include "KeyboardController.h"
#include "../Game.h"

void KeyboardController::onLeftMouse()
{
	auto trans = entity->getComponent<TransformComponent>();
	trans.canMove = false;
	BoundingBox box;
	if (entity->getComponent<SpriteComponent>().getFlip()) box = { trans.position - Vector2D(40.f, 0.f), trans.width, trans.height };
	else box = { trans.position + Vector2D(40.f, 0.f), trans.width, trans.height };
	entity->getComponent<AttackComponent>().startAttack(box);
}

void KeyboardController::onKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// std::cout << "key: esc" << std::endl;
		Game::_isRunning = false;
	}

	Vector2D& vel = _transform->velocity;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		std::cout << "position:" << _transform->position.x << ", " << _transform->position.y << std::endl;
	}

	if (_atc->attacking) {
		vel = Vector2D(0.0f, 0.0f);
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		_def->startDodge();
		if (entity->getComponent<SpriteComponent>().getFlip()) vel.x = -3.0f;
		else vel.x = 3.0f;
		return;
	}

	// if (!_transform->canMove) { return; }
	vel = Vector2D(0.0f, 0.0f);
	_sprite->setAnimate("idle");

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		vel.y = 3.0f;
		_sprite->setAnimate("walkUp");
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		vel.y = -3.0f;
		_sprite->setAnimate("walkR");
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		vel.x = -3.0f;
		_sprite->setAnimate("walkL");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		vel.x = 3.0f;
		_sprite->setAnimate("walkR");
	}
}

void KeyboardController::update(GLFWwindow* window)
{
	
}
