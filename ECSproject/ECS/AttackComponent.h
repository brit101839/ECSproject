#pragma once

#include "ECS.h"
#include "Quadtree/Box.h"
#include "Components.h"
#include "../BoundingBox.h"
#include "../EventManager.h"

class AttackComponent : public Component {
private:

	std::string _entityName;
	int _damage;
	quadtree::Box<float> _attackHitBox;
	EventManager& _globalEventManager;
	EventManager& _componentEventManager;
	TransformComponent* _trans;

public:

	bool attacking;
	BoundingBox mboundingBox;

	// AttackComponent(EnemyManager* EnemyM) : _enemyManager(EnemyM) {}
	AttackComponent(std::string entityName, EventManager& GeventM, EventManager& CeventM) 
		: _entityName(entityName), _globalEventManager(GeventM), _componentEventManager(CeventM) {}

	void init() override {
		if (entity->hasComponent<StatsComponent>()) {
			_damage = entity->getComponent<StatsComponent>().getAttack();
		}
		_trans = &entity->getComponent<TransformComponent>();
		_componentEventManager.subscribe<AttackStepEvent>([this](Event& event) {
			onAttackEvent(event); });
	}

	/*quadtree::Box<float> getBox() const {
		return { {boundingBox.position.x - boundingBox.width / 2, boundingBox.position.y + boundingBox.height / 2},{boundingBox.width, boundingBox.height} };
	}*/

	void startAttack(BoundingBox box) {
		attacking = true;
		mboundingBox = box;

		// std::cout << "trigger start attack" << std::endl;
		AttackStepEvent event("startAttack");
		_componentEventManager.notify<AttackStepEvent>(event);

		attacking = false;
	}

	void onAttackEvent(Event& event) {
		auto& attackEvent = static_cast<AttackStepEvent&>(event);
		if (attackEvent.attackStep != "startAttack") {
			std::cout << "Attack by entity: " << _entityName
				<< ", skill: " << attackEvent.attackStep << std::endl;
			AttackEvent event(_entityName, mboundingBox, _damage);
			_globalEventManager.notify<AttackEvent&>(event);
		}
	}

	void endAttack() {
		attacking = false;
	}

	void update(GLFWwindow* window) override {}

	int getDamage() const { return _damage; }
};