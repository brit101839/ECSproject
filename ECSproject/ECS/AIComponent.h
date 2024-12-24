#pragma once

#include "ECS.h"
#include "Components.h"

enum class EnemyState
{
	patrol, tracking, fighting, backing
};

class AIComponent :public Component
{
private:

	TransformComponent* _transform;
	SpriteComponent* _sprite;
	Vector2D _defaltPos;

	const TransformComponent& _playerTrans;

public:

	EnemyState _state;

	AIComponent(TransformComponent& playerT)
		:_playerTrans(playerT)
	{
	}

	void init() override {
		_transform = &entity->getComponent<TransformComponent>();
		_sprite = &entity->getComponent<SpriteComponent>();
		_defaltPos = _transform->position;
	};

	void update(GLFWwindow* window) override;
};

