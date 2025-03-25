#pragma once

#include "../ECS/ECS.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/SpriteComponent.h"

class SummonedItem {
protected:
	Entity* _entity;
	SummonedItem(Entity* entity) : _entity(entity) {}

public:
	// virtual void onSpawn(Entity* entity, Vector2D position) = 0; // Called when the object is spawned
	virtual ~SummonedItem() = default; // Ensure proper cleanup
};

class FireBall : public SummonedItem {
private:

public:

	FireBall(Entity* entity, Vector2D position) : SummonedItem(entity) {
		entity->addcomponent<TransformComponent>(position, Vector2D(-1.0f, 0.0f), 1.0f, 100.0f, 100.0f);
		entity->addcomponent<LocalEventComponent>();
		auto& sprite = entity->addcomponent<SpriteComponent>("D:/dependencies/resource/skill/Fire Effect 1/Firebolt SpriteSheet.png", true, 48.f, 48.f);
		sprite.addAnimation("idle", Animation(0, 11, 10, false));
		sprite.setAnimate("idle");
	}

};