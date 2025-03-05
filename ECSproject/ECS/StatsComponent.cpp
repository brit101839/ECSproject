#include "StatsComponent.h"
#include "HealthBarComponet.h"
#include "AIComponent.h"

void StatsComponent::takeDamage(int damage)
{
	_health -= std::max(0, damage - _defense);
	if (_health < 0) {
		_health = 0;
		// entity->getComponent<SpriteComponent>().setAnimate("dying");
		if (entity->hasComponent<AIComponent>()) {
			entity->getComponent<AIComponent>().enemyDying();
			std::cout << "dying" << std::endl;
		}
	}
	mhealthPercent = float(_health) / _maxHealth;
	if (entity->hasComponent<HealthBarComponent>()) {
		entity->getComponent<HealthBarComponent>().mDroppingBlood = float(damage) / _maxHealth;
	}
	
}
