#pragma once

#include "ECS.h"
#include "Quadtree/Box.h"
#include "Components.h"
#include "../BoundingBox.h"

class EnemyManager;

class AttackComponent : public Component {
private:

	int _damage;
	quadtree::Box<float> _attackHitBox;
	EnemyManager* _enemyManager;
	TransformComponent* _trans;

public:

	bool attacking;
	BoundingBox mboundingBox;

	AttackComponent(EnemyManager* EnemyM) : _enemyManager(EnemyM) {}

	void init() override {
		if (entity->hasComponent<StatsComponent>()) {
			_damage = entity->getComponent<StatsComponent>().getAttack();
		}
		_trans = &entity->getComponent<TransformComponent>();
	}

	/*quadtree::Box<float> getBox() const {
		return { {boundingBox.position.x - boundingBox.width / 2, boundingBox.position.y + boundingBox.height / 2},{boundingBox.width, boundingBox.height} };
	}*/

	void startAttack(BoundingBox box);

	void endAttack() {
		attacking = false;
	}

	void update(GLFWwindow* window) override {}

	int getDamage() const { return _damage; }
};