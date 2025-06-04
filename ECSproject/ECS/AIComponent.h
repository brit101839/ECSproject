#pragma once

#include "ECS.h"
#include "AIContext.h"
#include "../StateMaching/StateMaching.h"
#include "../BehaviorTree/BehaviorTreeFactory.h"

class AIComponent :public Component
{
private:

	AIPreferBehavior _behavior;
	// AIStateMachine _stateMaching;
	AIContext* _aiContext;
	std::unique_ptr<BehaviorTree<AIstate&, AIContext&>> _tree;
	BehaviorTreeFactory _BTFactoty;
	AIstate _currentstate = { false };

	const TransformComponent& _playerTrans;

public:

	// EnemyState _state;

	AIComponent(const TransformComponent& playerTrans, AIPreferBehavior behavior = AIPreferBehavior::NormalAttackOnly)
		:_playerTrans(playerTrans), _behavior(behavior) {}

	void init() override {
		_aiContext = new AIContext(entity, _playerTrans, _behavior);

		_tree = std::make_unique<BehaviorTree<AIstate&, AIContext&>>(_BTFactoty.buildTree());

		/*auto tryScenario = [&](AIstate state, AIContext context) {
			_tree.get()->tickRoot(state, context);
			};*/

		//// The easiest case. The door is open.
		//std::cout << "\n# First scenario...\n";
		//tryScenario(AIstate{ false }, AIContext{ *_aiContext });
	};

	~AIComponent() override {
		_tree.reset();        // Not strictly needed ¡X unique_ptr will handle it.
		delete _aiContext;    // Only if you keep it as raw pointer (not recommended).
	}

	void enemyDying() {
		// _stateMaching.changeState(AIStateFactory::createState(EnemyState::dying), *_aiContext);
	}

	void update(GLFWwindow* window, double deltaTime) override {
		// _stateMaching.update(*_aiContext);
		_tree.get()->tickRoot(_currentstate, *_aiContext);
	}
};

