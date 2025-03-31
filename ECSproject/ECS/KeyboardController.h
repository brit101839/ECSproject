//
// KeyboardController.h

#pragma once

// #include "Components.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "AttackComponent.h"
#include "DefenseComponent.h"
#include "../vector2D/Vector2D.h"

class KeyboardController : public Component
{
private:

	TransformComponent* _transform;
	SpriteComponent* _sprite;
	AttackComponent* _atc;
	DefenseComponent* _def;

public:

	void init() override
	{
		_transform = &entity->getComponent<TransformComponent>();
		_sprite = &entity->getComponent<SpriteComponent>();
		_atc = &entity->getComponent<AttackComponent>();
		_def = &entity->getComponent<DefenseComponent>();
	}

	void onLeftMouse();
	void onKeyboard(GLFWwindow* window);

	void update(GLFWwindow* window);
};