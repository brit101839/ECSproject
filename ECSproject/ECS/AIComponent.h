#pragma once

#include "ECS.h"
#include "AIContext.h"
#include "../StateMaching/StateMaching.h"

class AIComponent :public Component
{
private:

	AIPreferBehavior _behavior;
	AIStateMachine _stateMaching;
	AIContext* _aiContext;

	const TransformComponent& _playerTrans;

public:

	EnemyState _state;

	AIComponent(const TransformComponent& playerTrans, AIPreferBehavior behavior = AIPreferBehavior::NormalAttackOnly)
		:_playerTrans(playerTrans), _behavior(behavior) {}

	void init() override {
		_aiContext = new AIContext(entity, _playerTrans, _behavior);
	};

	void enemyDying() {
		_stateMaching.changeState(AIStateFactory::createState(EnemyState::dying), *_aiContext);
	}

	void update(GLFWwindow* window) override {
		_stateMaching.update(*_aiContext);
	}

	AIContext* getAIContext() const { return _aiContext; };
};

