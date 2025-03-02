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

public:

	explicit EnemyManager(Manager& m, TransformComponent& playerT, CollisionManager* colM)
		: _manager(m), _playerTrans(playerT), _colliderManager(colM) {
	}

	Enemy* addEnemy(const Vector2D& position, const char* spritePath);

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

	void checkAttack(AttackComponent& atc) {
		std::cout << "checking" << std::endl;
		for (auto* enemy : _enemies) {
			enemy->handleAttack(atc);
		}
	}

	std::vector<Enemy*>& getEnemies() { return _enemies; }
};
