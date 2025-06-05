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

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		std::cout << "position:" << _transform->position.x << ", " << _transform->position.y << std::endl;
	}

	Vector2D inputDir = Vector2D(0.f, 0.f);

	if (_atc->attacking) {
		_transform->setDirection(Vector2D(0.f, 0.f));
		return;
	}

	if (_def->doging()) {
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		_transform->setDirection(Vector2D(0.f, 0.f));
		_def->startDodge();
		if (entity->getComponent<SpriteComponent>().getFlip()) inputDir.x = -1.0f;
		else inputDir.x = 1.0f;
		_transform->setDirection(inputDir);
		return;
	}

	// if (!_transform->canMove) { return; }
	
	_sprite->setAnimate("idle");

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		inputDir.y = 1.0f;
		_sprite->setAnimate("walkUp");
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		inputDir.y = -1.0f;
		_sprite->setAnimate("walkR");
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		inputDir.x = -1.0f;
		_sprite->setAnimate("walkL");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		inputDir.x = 1.0f;
		_sprite->setAnimate("walkR");
	}

	_transform->setDirection(inputDir);
}

void KeyboardController::update(GLFWwindow* window, double deltaTime)
{
	
}
