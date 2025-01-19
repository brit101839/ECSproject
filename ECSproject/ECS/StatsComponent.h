#pragma once

#include "ECS.h"

class StatsComponent : public Component {
private:

	int _health, _maxHealth, _attack, _defense;
	

public:

	float mhealthPercent = 1.f;

	StatsComponent(int initialHealth, int attack, int defense)
		:_health(initialHealth), _maxHealth(initialHealth), _attack(attack), _defense(defense)
	{
	}

	void init() override {}

	int getHealth() const { return _health; }
	int getMaxHealth() const { return _maxHealth; }

	void takeDamage(int damage);

	int getAttack() const { return _attack; }

	bool isAlive() const { return _health > 0; }
};