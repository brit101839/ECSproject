#pragma once

#include "ECS.h"
#include "Quadtree/Box.h"
#include "SpriteComponent.h"
#include "StatsComponent.h"
#include "../BoundingBox.h"
#include "../EventSystem.h"

class DefenseComponent :public Component {
private:

	bool _dodging = false;

	std::string _entityName;
	SpriteComponent* _sprite;
	StatsComponent* _stats;
	LocalEventComponent* _localEvent;

	void takeDamage(int damage) {
		_stats->takeDamage(damage);
		if (_stats->getHealth() <= 0) {
			// _entity->destroy();
		}
	}

	void onHurt() {
		_sprite->OnInjuried();
		// takeDamage()
	}

public:

	DefenseComponent(std::string entityName)
		: _entityName(entityName) { }

	void init() override {
		if (entity->hasComponent<LocalEventComponent>()) {
			_localEvent = &entity->getComponent<LocalEventComponent>();
		}
		else {
			std::cerr << "not init local event component" << std::endl;
		}
		_stats = &entity->getComponent<StatsComponent>();
		_sprite = &entity->getComponent<SpriteComponent>();

		_localEvent->subscribe<DodgeStepEvent>([this](Event& event) {
			endDodge(event); });
	}

	void UnderAttack(AttackCheckingEvent& atc) {
		if (_dodging) {
			return;
		}
		if (checkAttack(atc.boundingBox)) {
			int damage = atc.damage;
			takeDamage(damage);
			onHurt();
			// atc.endAttack();
			std::cout << "attacking( " << getHealth() << "/ " << getMaxHealth() << ")" << std::endl;
		}
	}

	void startDodge() {
		DodgeStepEvent event(DodgeStep::startDodge);
		_localEvent->publish<DodgeStepEvent>(event);
		_dodging = true;
	}

	void endDodge(Event& event) {
		auto& dodgeStepEvent = static_cast<DodgeStepEvent&>(event);
		if (dodgeStepEvent.dodgeStep == DodgeStep::endDodge) {
			_dodging = false;
		}
	}

	bool checkAttack(BoundingBox& box);
	int getHealth() const { return _stats->getHealth(); }
	int getMaxHealth() const { return _stats->getMaxHealth(); }
	bool doging() const { return _dodging; }
};