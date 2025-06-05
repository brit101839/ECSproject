//
// ColliderComponent.h

#pragma once

#include <string>
#include "Components.h"
#include "Quadtree/Box.h"
#include "../BoundingBox.h"
#include "../EventSystem.h"

class ColliderComponent : public Component
{
private:

	EventSystem& _globalEventsys;
	TransformComponent* _trans;
	bool isInitialSize;
	Vector2D _shift;
	std::shared_ptr<Collider> _collider;

public:

	ColliderComponent(EventSystem& GeventM, std::string t, ColliderType type = ColliderType::mapObstacle)
		:_globalEventsys(GeventM), isInitialSize(false)
	{
		_collider = std::make_shared<Collider>(t, type, entity);
	}

	ColliderComponent(EventSystem& GeventM, std::string t, BoundingBox boundingBox, Vector2D shift = Vector2D(0.0f, 0.0f), ColliderType type = ColliderType::role)
		:_globalEventsys(GeventM), isInitialSize(true), _shift(shift)
	{
		_collider = std::make_shared<Collider>(boundingBox, getBox(boundingBox), t, type, entity);
	}

	quadtree::Box<float> getBox(BoundingBox boundingBox) const {
		return { {boundingBox.position.x - boundingBox.width / 2, boundingBox.position.y + boundingBox.height / 2},{boundingBox.width, boundingBox.height} };
	}
		
	void init() override 
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addcomponent<TransformComponent>();
		}

		_trans = &entity->getComponent<TransformComponent>();
		if (!isInitialSize) {
			BoundingBox bb(_trans->position + _shift, _trans->width, _trans->height);
			_collider->boundingBox = bb;
			_collider->box = getBox(bb);
		}

		AddColliderEvent event(_collider);
		_globalEventsys.publish<AddColliderEvent>(event);
		// _colliderManager->addCollider(this);
	}

	void update(GLFWwindow* window, double deltaTime) override {
		if (_collider->type != ColliderType::mapObstacle) {
			auto& position = _trans->position;
			_collider->boundingBox.position = position + _shift;
		}
	}

	const ColliderType getType() const { return _collider->type; }
	const BoundingBox getBoundingBox() const { return _collider->boundingBox; }
};