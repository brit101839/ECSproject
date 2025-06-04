#include "ColliderComponent.h"
#include "../CollisionManager.h"

ColliderComponent::ColliderComponent(CollisionManager* colM, std::string t)
	:_colliderManager(colM), tag(t), isInitialSize(false)
{
	
}

ColliderComponent::ColliderComponent(CollisionManager* colM, std::string t, BoundingBox newboundingBox, Vector2D shift)
	:_colliderManager(colM), tag(t), isInitialSize(true), boundingBox(newboundingBox), _shift(shift)
{
	box = { {newboundingBox.position.x - newboundingBox.width, newboundingBox.position.y - newboundingBox.height},
		    {newboundingBox.position.x + newboundingBox.width, newboundingBox.position.y + newboundingBox.height} };
}

void ColliderComponent::init()
{
	if (!entity->hasComponent<TransformComponent>())
	{
		entity->addcomponent<TransformComponent>();
	}

	_trans = &entity->getComponent<TransformComponent>();
	if (!isInitialSize) {
		boundingBox.width = _trans->width;
		boundingBox.height = _trans->height;
		box = { {boundingBox.position.x - boundingBox.width, boundingBox.position.y - boundingBox.height},
			{boundingBox.position.x + boundingBox.width, boundingBox.position.y + boundingBox.height} };
	}
	
	_colliderManager->addCollider(this);
}

void ColliderComponent::update(GLFWwindow* window, double deltaTime)
{
	auto& position = _trans->position;
	boundingBox.position = position + _shift;
}
