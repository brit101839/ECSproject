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
public:
	FireBall(Entity* entity, Vector2D position, bool flip) : MagicItem(entity) {
		Vector2D posOffset, velocity;
		if (!flip) {
			posOffset = Vector2D(50.f, 0.f);
			velocity = Vector2D(2.0f, 0.0f);
		}
		else {
			posOffset = Vector2D(-50.f, 0.f);
			velocity = Vector2D(-2.0f, 0.0f);
		}
		entity->addcomponent<TransformComponent>(position + posOffset, velocity, 1.0f, 300.0f, 300.0f);
		entity->addcomponent<LocalEventComponent>();
		auto& sprite = entity->addcomponent<SpriteComponent>("D:/dependencies/resource/skill/Fire Effect 1/Firebolt SpriteSheet.png", true, 48.f, 48.f);
		sprite.addAnimation("idle", Animation(0, 4, 10, flip));
		sprite.setAnimate("idle");
	}
};

class IceBall : public MagicItem {
public:
	IceBall(Entity* entity, Vector2D position, bool flip) : MagicItem(entity) {
		Vector2D posOffset, velocity;
		if (!flip) {
			posOffset = Vector2D(50.f, 0.f);
			velocity = Vector2D(2.0f, 0.0f);
		}
		else {
			posOffset = Vector2D(-50.f, 0.f);
			velocity = Vector2D(-2.0f, 0.0f);
		}
		entity->addcomponent<TransformComponent>(position + posOffset, velocity, 1.0f, 300.0f, 300.0f);
		entity->addcomponent<LocalEventComponent>();
		auto& sprite = entity->addcomponent<SpriteComponent>("D:/dependencies/resource/skill/Ice Effect 01/Ice VFX 1/IceVFX 1 Repeatable.png", true, 48.f, 32.f);
		sprite.addAnimation("idle", Animation(0, 10, 10, flip));
		sprite.setAnimate("idle");
	}
};