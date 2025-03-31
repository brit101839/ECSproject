#pragma once

#include "../ECS/ECS.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/SpriteComponent.h"

class MagicItem {
protected:
	Entity* _entity;
	MagicItem(Entity* entity) : _entity(entity) {}

public:
	// virtual void onSpawn(Entity* entity, Vector2D position) = 0; // Called when the object is spawned
	virtual ~MagicItem() = default; // Ensure proper cleanup
};

class FireBall : public MagicItem {
private:

public:

	FireBall(Entity* entity, Vector2D position) : MagicItem(entity) {
		entity->addcomponent<TransformComponent>(position, Vector2D(1.0f, 0.0f), 1.0f, 300.0f, 300.0f);
		entity->addcomponent<LocalEventComponent>();
		auto& sprite = entity->addcomponent<SpriteComponent>("D:/dependencies/resource/skill/Fire Effect 1/Firebolt SpriteSheet.png", true, 48.f, 48.f);
		sprite.addAnimation("idle", Animation(0, 4, 10, false));
		sprite.setAnimate("idle");
	}

};