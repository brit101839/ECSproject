#include "ECS.h"

void Entity::addGroup(Group mGroup)
{
	_groupBitSet[mGroup] = true;
	_manager.AddToGroup(this, mGroup);
}
