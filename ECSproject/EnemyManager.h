#pragma once
#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"

class CollisionManager;

class EnemyManager
{
private:

	std::vector<Entity*> _enemies;
	Manager& _manager;
	TransformComponent& _playerTrans;
	CollisionManager* _colliderManager;

public:

	explicit EnemyManager(Manager& m, TransformComponent& playerT, CollisionManager* colM) 
		: _manager(m), _playerTrans(playerT), _colliderManager(colM) {}

	Entity& addEnemy(const Vector2D& position, const char* spritePath);

	void renderEnemies(Shader& shader, const Vector2D& cameraPos) {
		for (auto* enemy : _enemies) {
			if (enemy->isActive()) {
				enemy->draw(shader, cameraPos);
			}
		}
	}

	void updateEnemies(GLFWwindow* window) {
		for (auto* enemy : _enemies) {
			if (enemy->isActive()) {
				enemy->update(window);
			}
		}
	}
};

