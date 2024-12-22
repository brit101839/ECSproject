#pragma once
#include "ECS/ECS.h"

class EnemyManager
{
private:

	std::vector<Entity*> _enemies;
	Manager& _manager;

public:

	explicit EnemyManager(Manager& m) : _manager(m) {}

	Entity& addEnemy(const Vector2D& position, const char* spritePath);

	void renderEnemies(Shader& shader, const Vector2D& cameraPos) {
		for (auto* enemy : _enemies) {
			if (enemy->isActive()) {
				enemy->draw(shader, cameraPos);
			}
		}
	}


};

