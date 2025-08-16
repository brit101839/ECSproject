#pragma once

#include "../ECS/ECS.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/SpriteComponent.h"
#include "../ECS/LocalEventComponent.h"
#include "../ECS/ColliderComponent.h"
#include "../ECS/ProjectileComponent.h"
//#include "../CollisionManager.h"

class MagicProjectile {
protected:
	Entity* _entity;
	MagicProjectile(Entity* entity) : _entity(entity) {}

public:
	// virtual void onSpawn(Entity* entity, Vector2D position) = 0; // Called when the object is spawned
	virtual ~MagicProjectile() = default; // Ensure proper cleanup
};

class FireBall : public MagicProjectile {
public:
	FireBall(EventSystem& gEventSys, Entity* entity, Vector2D position, bool flip) : MagicProjectile(entity) {
		Vector2D posOffset, velocity, colliderOffset;
		if (!flip) {
			posOffset = Vector2D(50.f, 0.f);
			velocity = Vector2D(1.0f, 0.0f);
			colliderOffset = Vector2D(30.f, -40.f);
		}
		else {
			posOffset = Vector2D(-50.f, 0.f);
			velocity = Vector2D(-1.0f, 0.0f);
			colliderOffset = Vector2D(-30.f, -40.f);
		}
		entity->addcomponent<TransformComponent>(position + posOffset, 300.0f, 300.0f);
		entity->addcomponent<MovementComponent>(400.0f, velocity);
		entity->addcomponent<LocalEventComponent>(gEventSys);
		auto& sprite = entity->addcomponent<SpriteComponent>("C:/dependencies/resource/skill/Fire Effect 1/Firebolt.png", true, 48.f, 48.f);
		sprite.addAnimation("idle", Animation(0, 4, 10, flip));
		sprite.addAnimation("explosion", Animation(1, 6, 10, flip, false, AnimateState::Dying));
		sprite.setAnimate("idle");

		BoundingBox bound{ position + posOffset, 120.0f, 50.0f };
		entity->addcomponent<ColliderComponent>(gEventSys, "fire ball", bound, colliderOffset, ColliderType::spell);
		entity->addcomponent<ProjectileComponent>(10);
	}
};

class IceBall : public MagicProjectile {
public:
	IceBall(EventSystem& gEventSys, Entity* entity, Vector2D position, bool flip) : MagicProjectile(entity) {
		Vector2D posOffset, velocity;
		if (!flip) {
			posOffset = Vector2D(50.f, 0.f);
			velocity = Vector2D(1.0f, 0.0f);
		}
		else {
			posOffset = Vector2D(-50.f, 0.f);
			velocity = Vector2D(-1.0f, 0.0f);
		}
		entity->addcomponent<TransformComponent>(position + posOffset, 300.0f, 300.0f);
		entity->addcomponent<MovementComponent>(400.0f, velocity);
		entity->addcomponent<LocalEventComponent>(gEventSys);
		auto& sprite = entity->addcomponent<SpriteComponent>("C:/dependencies/resource/skill/Ice Effect 01/Ice VFX 1/IceVFX 1 Repeatable.png", true, 48.f, 32.f);
		sprite.addAnimation("idle", Animation(0, 10, 10, flip));
		sprite.setAnimate("idle");

		BoundingBox bound{ position + posOffset, 200.0f, 100.0f };
		entity->addcomponent<ColliderComponent>(gEventSys, "ice ball", bound, Vector2D(0.f, 0.f), ColliderType::spell);
		entity->addcomponent<ProjectileComponent>(10);
	}
};