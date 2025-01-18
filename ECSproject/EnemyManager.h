#pragma once
#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"
#include "ECS/AttackComponent.h"
#include "ECS/StatsComponent.h"
#include "ECS/Collision.h"

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
		: _manager(m), _playerTrans(playerT), _colliderManager(colM) { }

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

	void checkAttack(AttackComponent& atc) {
		std::cout << "checking" << std::endl;
		for (auto* enemy : _enemies) {
			if (Collision::AABB(atc.mboundingBox, enemy->getComponent<ColliderComponent>().boundingBox)) {
				int damage = atc.getDamage();
				enemy->getComponent<StatsComponent>().takeDamage(damage);
				enemy->getComponent<SpriteComponent>().OnAttack();
				atc.endAttack();
				auto &stat = enemy->getComponent<StatsComponent>();
				std::cout << "attacking( "<< stat.getHealth() << "/ " << stat.getMaxHealth() << ")" << std::endl;
			}
		}
	}

	// std::vector<Entity*>* getEnemies() { return _enemies; }
};

