#pragma once

#include "ECS.h"
#include "AIContext.h"
#include "../StateMaching/StateMaching.h"

class AIComponent :public Component, public AIContext
{
private:

	TransformComponent* _transform;
	SpriteComponent* _sprite;
	Vector2D _defaltPos;

	const TransformComponent& _playerTrans;

	AIStateMachine _stateMaching;

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

		_stateMaching.changeState(new TrackingState());
	};

	void update(GLFWwindow* window) override {
		_stateMaching.update(*this);
	}

	TransformComponent* getTransform() const override { return _transform; }
	SpriteComponent* getSprite() const override { return _sprite; }
	Vector2D getDefaultPos() const override { return _defaltPos; }
	const Vector2D& getPlayerPos() const override { return _playerTrans.position; }
};

