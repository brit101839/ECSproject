#pragma once

#include "ECS.h"
#include "Quadtree/Box.h"
#include "TransformComponent.h"
#include "StatsComponent.h"
#include "SkillComponent.h"
#include "../BoundingBox.h"
#include "../EventSystem.h"

class AttackComponent : public Component {
private:

	std::string _entityName;
	int _damage;
	quadtree::Box<float> _attackHitBox;
	EventSystem& _globalEventManager;
	TransformComponent* _trans;
	LocalEventComponent* _localEvent;

public:

	bool attacking;
	BoundingBox mdefaultBoundingBox;

	// AttackComponent(EnemyManager* EnemyM) : _enemyManager(EnemyM) {}
	AttackComponent(std::string entityName, EventSystem& GeventM) 
		: _entityName(entityName), _globalEventManager(GeventM) {}

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
		_localEvent->subscribe<AttackDamageEvent>([this](Event& event) {
			startCheckingAttack(event); });
		_localEvent->subscribe<AttackStepEvent>([this](Event& event) {
			endAttack(event); });
	}

	/*quadtree::Box<float> getBox() const {
		return { {boundingBox.position.x - boundingBox.width / 2, boundingBox.position.y + boundingBox.height / 2},{boundingBox.width, boundingBox.height} };
	}*/

	void startAttack(BoundingBox box) {
		attacking = true;
		mdefaultBoundingBox = box;

		// std::cout << "trigger start attack" << std::endl;

		// notify the animate component play attack animate
		AttackStepEvent event(AttackStep::startAttack);
		_localEvent->publish<AttackStepEvent>(event);

		// _componentEventManager.publish<AttackStepEvent>(event);
		/*if (entity->hasComponent<SkillCompnent>()) {
			entity->getComponent<SkillCompnent>().UseSkill();
		}*/

		// attacking = false;
	}

	// nofify all enemy check defense or got hurt
	void startCheckingAttack(Event& event) {
		auto& skillEvent = static_cast<AttackDamageEvent&>(event);
		std::cout << "Attack by entity: " << _entityName
			<< ", skill: " << skillEvent.skillName << std::endl;
		AttackCheckingEvent publishEvent(_entityName, mdefaultBoundingBox, skillEvent.damage);
		_globalEventManager.publish<AttackCheckingEvent&>(publishEvent);
	}

	void endAttack(Event& event) {
		auto& attackStepEvent = static_cast<AttackStepEvent&>(event);
		if (attackStepEvent.attackStep == AttackStep::endAttack) {
			attacking = false;
		}
	}

	void update(GLFWwindow* window, double deltaTime) override {}

	int getDamage() const { return _damage; }
};