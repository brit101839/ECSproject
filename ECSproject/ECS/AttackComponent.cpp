#include "AttackComponent.h"
#include "../Enemy/EnemyManager.h"

void AttackComponent::startAttack(BoundingBox box)
{
	attacking = true;
	mboundingBox = box;
	// _enemyManager->checkAttack(*this);
	AnimationSet& animationSet = entity->getComponent<SpriteComponent>().getAnimationSet();
	animationSet.attackingCheck = [this]() {
		_enemyManager->checkAttack(*this);
	};
	entity->getComponent<SpriteComponent>().setAnimate("attack_1");
	attacking = false;
}
