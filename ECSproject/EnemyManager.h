#pragma once
#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"

class EnemyManager
{
private:

	std::vector<Entity*> _enemies;
	Manager& _manager;
	TransformComponent& _playerTrans;

public:

	explicit EnemyManager(Manager& m, TransformComponent& playerT) : _manager(m), _playerTrans(playerT) {}

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

