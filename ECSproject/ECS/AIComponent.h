#pragma once

#include "ECS.h"
#include "AIContext.h"
#include "../StateMaching/StateMaching.h"

class AIComponent :public Component
{
private:

	AIContext* _aiContext;
	//TransformComponent* _transform;
	//SpriteComponent* _sprite;
	//Vector2D _defaltPos;

	const TransformComponent& _playerTrans;

	AIStateMachine _stateMaching;

public:

	EnemyState _state;

	AIComponent(const TransformComponent& playerTrans)
		:_playerTrans(playerTrans) {
		
	}

	void init() override {
		TransformComponent* transform = &entity->getComponent<TransformComponent>();
		SpriteComponent* sprite = &entity->getComponent<SpriteComponent>();
		AttackComponent* attack = &entity->getComponent<AttackComponent>();
		_aiContext = new AIContext(transform, sprite, attack, _playerTrans);
		_stateMaching.changeState(new TrackingState());
	};

	void enemyDying() {
		_stateMaching.changeState(AIStateFactory::createState(EnemyState::dying));
	}

	void update(GLFWwindow* window) override {
		_stateMaching.update(*_aiContext);
	}

	AIContext* getAIContext() const { return _aiContext; };

	/*TransformComponent* getTransform() const override { return _transform; }
	SpriteComponent* getSprite() const override { return _sprite; }
	Vector2D getDefaultPos() const override { return _defaltPos; }
	const Vector2D& getPlayerPos() const override { return _playerTrans.position; }*/
};

