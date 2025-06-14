//
// ColliderComponent.h

#pragma once

#include <string>
#include "TransformComponent.h"
#include "../Sprite.h"
#include "../Collider.h"
#include "../EventSystem.h"
#include "Collision.h"

class ColliderComponent : public Component
{
private:

	EventSystem& _globalEventsys;
	TransformComponent* _trans;
	bool isInitialSize;
	Vector2D _shift;
	std::shared_ptr<Collider> _collider;

	bool _openDebugRender = false;
	std::unique_ptr<Sprite> _sprite;

public:

	ColliderComponent(EventSystem& GeventM, std::string t, ColliderType type = ColliderType::mapObstacle)
		:_globalEventsys(GeventM), isInitialSize(false)
	{
		_collider = std::make_shared<Collider>(t, type);
	}

	ColliderComponent(EventSystem& GeventM, std::string t, BoundingBox boundingBox, Vector2D shift = Vector2D(0.0f, 0.0f), ColliderType type = ColliderType::role)
		:_globalEventsys(GeventM), isInitialSize(true), _shift(shift)
	{
		_collider = std::make_shared<Collider>(boundingBox, getBox(boundingBox), t, type);
	}

	~ColliderComponent() override 
	{
		RemoveColliderEvent event(_collider);
		_globalEventsys.publish<RemoveColliderEvent>(event);
		if (_collider.use_count() > 2) {
			std::cout << "Warning: Collider is still used by other systems! use_count = "
				<< _collider.use_count() << std::endl;
		}
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
		_collider->entity = entity;

		AddColliderEvent event(_collider);
		_globalEventsys.publish<AddColliderEvent>(event);
		// _colliderManager->addCollider(this);

		if (_openDebugRender) {
			TextureManager& textureManager = TextureManager::getInstance();
			_sprite = std::make_unique<Sprite>(textureManager.genWhiteTexture(), _collider->boundingBox.width, _collider->boundingBox.height);
		}
	}

	void update(GLFWwindow* window, double deltaTime) override {
		if (_collider->type != ColliderType::mapObstacle) {
			auto& position = _trans->position;
			_collider->boundingBox.position = position + _shift;
		}
	}

	void draw(Shader& shader, Vector2D cameraPos) override {
		if (_openDebugRender) {
			BoundingBox b = _collider->boundingBox;
			_sprite->renderRectangle(b.position, Vector2D(1.0, 1.0), shader, cameraPos, glm::vec3(0.5f, 0.5f, 0.5f));
		}
	}

	bool isOutOfBounds(quadtree::Box<float> cameraBound){
		if (!entity->isActive()) std::cerr << "the entity isnt active!" << std::endl;
		return Collision::isOutOfBound(cameraBound, _collider->boundingBox);
	}

	const ColliderType getType() const { return _collider->type; }
	const BoundingBox getBoundingBox() const { return _collider->boundingBox; }
	const quadtree::Box<float> getBox() const { return _collider->box; }
};