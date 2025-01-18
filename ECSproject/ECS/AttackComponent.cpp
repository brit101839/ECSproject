#include "AttackComponent.h"
#include "../EnemyManager.h"

void AttackComponent::startAttack(BoundingBox box)
{
	attacking = true;
	mboundingBox = box;
	_enemyManager->checkAttack(*this);
	attacking = false;
}
