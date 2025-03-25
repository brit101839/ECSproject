#pragma once
#include "../ECS/ECS.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/AttackComponent.h"
#include "../ECS/StatsComponent.h"
#include "../ECS/Collision.h"
#include "Enemy.h"

class CollisionManager;

class EnemyManager
{
private:

	std::vector<Enemy*> _enemies;
	Manager& _manager;
	TransformComponent& _playerTrans;
	CollisionManager* _colliderManager;
	EventSystem& _globalEventManager;
	std::shared_ptr<SpawnSystem> _spawnSys;

public:

	explicit EnemyManager(Manager& m, TransformComponent& playerT, CollisionManager* colM, EventSystem& GeventM, std::shared_ptr<SpawnSystem> spawnSys)
		: _manager(m), _playerTrans(playerT), _colliderManager(colM), _globalEventManager(GeventM), _spawnSys(spawnSys) {
		_globalEventManager.subscribe<AttackEvent>([this](Event& event) {
			onAttackEvent(event); });
	}

	Enemy* addEnemy(std::string name, const Vector2D& position);

	void renderEnemies(Shader& shader, const Vector2D& cameraPos) {
		for (auto* enemy : _enemies) {
			if (enemy->getEntity()->isActive()) {
				enemy->getEntity()->draw(shader, cameraPos);
			}
		}
	}

	void updateEnemies(GLFWwindow* window) {
		for (auto* enemy : _enemies) {
			if (enemy->getEntity()->isActive()) {
				enemy->getEntity()->update(window);
			}
		}
	}

	void onAttackEvent(Event& event) {
		auto& attackEvent = static_cast<AttackEvent&>(event);
		if (attackEvent.attacker == "player") {
			handleHurtEvent(attackEvent);
		}
	}

	void handleHurtEvent(AttackEvent& atc) {
		std::cout << "checking attack by: " << atc.attacker << std::endl;
		for (auto* enemy : _enemies) {
			enemy->handleHurt(atc);
		}
		AttackStepEvent event("enemyHurtCheckComplete");
		_globalEventManager.publish<AttackStepEvent>(event);
	}

	std::vector<Enemy*>& getEnemies() { return _enemies; }
};
