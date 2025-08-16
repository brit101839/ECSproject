#include "StatsComponent.h"
#include "HealthBarComponet.h"
#include "AIComponent.h"

void StatsComponent::update(GLFWwindow* window, double deltaTime)
{
	_restingTime = ((_restingTime - deltaTime) > 0) ? _restingTime - deltaTime : 0.0f;
}

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
	mhealthPercent = float(_health) / float(_maxHealth);
	if (entity->hasComponent<HealthBarComponent>()) {
		entity->getComponent<HealthBarComponent>().mDroppingBlood = float(damage) / float(_maxHealth);
	}
	std::cout << "health persent" << mhealthPercent << std::endl;
}
