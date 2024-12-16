#include "ColliderComponent.h"
#include "../Game.h"

ColliderComponent::ColliderComponent(std::string t)
	:tag(t), isInitialSize(false)
{
	
}

ColliderComponent::ColliderComponent(std::string t, BoundingBox newboundingBox) 
	:tag(t), isInitialSize(true), boundingBox(newboundingBox)
{

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
	}
	
	Game::colliders.push_back(this);
}

void ColliderComponent::update(GLFWwindow* window)
{
	boundingBox.position = _trans->position;
	/*boundingBox.width = _trans->width;
	boundingBox.height = _trans->height;*/
}
