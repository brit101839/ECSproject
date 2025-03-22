#pragma once

#include "ECS.h"
#include "Quadtree/Box.h"
#include "Components.h"
#include "../BoundingBox.h"
#include "../EventSystem.h"

class AttackComponent : public Component {
private:

	std::string _entityName;
	int _damage;
	quadtree::Box<float> _attackHitBox;
	EventSystem& _globalEventManager;
	//EventSystem& _componentEventManager;
	TransformComponent* _trans;
	LocalEventComponent* _localEvent;

public:

	bool attacking;
	BoundingBox mboundingBox;

	// AttackComponent(EnemyManager* EnemyM) : _enemyManager(EnemyM) {}
	AttackComponent(std::string entityName, EventSystem& GeventM) 
		: _entityName(entityName), _globalEventManager(GeventM) 
	{
		
	}

	void init() override {
		if (entity->hasComponent<StatsComponent>()) {
			_damage = entity->getComponent<StatsComponent>().getAttack();
		}

		if (entity->hasComponent<LocalEventComponent>()) {
			_localEvent = &entity->getComponent<LocalEventComponent>();
			
		}
		else {
			std::cerr << "not init local event component" << std::endl;
		}
		
		_trans = &entity->getComponent<TransformComponent>();
		_localEvent->subscribe<AttackStepEvent>([this](Event& event) {
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
		_localEvent->publish<AttackStepEvent>(event);
		// _componentEventManager.publish<AttackStepEvent>(event);

		attacking = false;
	}

	void onAttackEvent(Event& event) {
		auto& attackEvent = static_cast<AttackStepEvent&>(event);
		if (attackEvent.attackStep != "startAttack") {
			std::cout << "Attack by entity: " << _entityName
				<< ", skill: " << attackEvent.attackStep << std::endl;
			AttackEvent event(_entityName, mboundingBox, _damage);
			_globalEventManager.publish<AttackEvent&>(event);
		}
	}

	void endAttack() {
		attacking = false;
	}

	void update(GLFWwindow* window) override {}

	int getDamage() const { return _damage; }
};