#include "DefenseComponent.h"
#include "../CollisionManager.h"

bool DefenseComponent::checkAttack(BoundingBox& box)
{
	return Collision::AABB(box, entity->getComponent<ColliderComponent>().getBoundingBox());
}
