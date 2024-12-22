#include "ColliderComponent.h"
#include "../Game.h"

ColliderComponent::ColliderComponent(std::string t)
	:tag(t), isInitialSize(false)
{
	
}

ColliderComponent::ColliderComponent(std::string t, BoundingBox newboundingBox, Vector2D shift)
	:tag(t), isInitialSize(true), boundingBox(newboundingBox), _shift(shift)
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
	
	Game::colliders.push_back(this);
	indexInVector = Game::colliders.size() - 1;
}

void ColliderComponent::update(GLFWwindow* window)
{
	auto& position = _trans->position;
	boundingBox.position = position + _shift;
	/*boundingBox.width = _trans->width;
	boundingBox.height = _trans->height;*/
}
