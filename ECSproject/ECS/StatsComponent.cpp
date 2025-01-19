#include "StatsComponent.h"
#include "HealthBarComponet.h"

void StatsComponent::takeDamage(int damage)
{
	_health -= std::max(0, damage - _defense);
	if (_health < 0) _health = 0;
	mhealthPercent = float(_health) / _maxHealth;
	entity->getComponent<HealthBarComponent>().mDroppingBlood = float(damage) / _maxHealth;
}
