#pragma once

#include "ECS.h"
#include "LocalEventComponent.h"

class StatsComponent : public Component {
private:

	int _health, _maxHealth, _attack, _defense;
	double _restingTime = 0.f;

	LocalEventComponent* _localEvent;

public:

	float mhealthPercent = 1.f;

	StatsComponent(int initialHealth, int attack, int defense)
		:_health(initialHealth), _maxHealth(initialHealth), _attack(attack), _defense(defense)
	{
	}

	void init() override 
	{
		if (entity->hasComponent<LocalEventComponent>()) {
			_localEvent = &entity->getComponent<LocalEventComponent>();
		}

		_localEvent->subscribe<AttackStepEvent>([this](Event& event) {
			endAttack(event); });
	}

	void update(GLFWwindow* window, double deltaTime) override;

	void endAttack(Event& event) {
		auto& attackStepEvent = static_cast<AttackStepEvent&>(event);
		if (attackStepEvent.attackStep == AttackStep::endAttackAndRest) {
			setRestingTime(1.0);
		}
	}

	int getHealth() const { return _health; }

	int getMaxHealth() const { return _maxHealth; }

	void takeDamage(int damage);

	int getAttack() const { return _attack; }

	void setRestingTime(double restingTime) { _restingTime = restingTime; }

	bool isResting(void) { return _restingTime > 0; }

	bool isAlive() const { return _health > 0; }
};