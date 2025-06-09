#pragma once

#include "../ECS/ECS.h"
#include "MagicProjectile.h"
#include "../ProjectileManager.h"
#include "../EventSystem.h"
#include <vector>

class SpawnSystem {
private:

	Manager& _manager;
	ProjectileManager& _projectileManager;
	EventSystem& _gEventSystem;

	Entity* createEntity() {
		Entity* newEntity = &_manager.addEntity();
		_projectileManager.addEntity(newEntity);
		return newEntity;
	}

public:

	SpawnSystem(Manager& m, ProjectileManager& p, EventSystem& gEventSys)
		:_manager(m), _projectileManager(p), _gEventSystem(gEventSys) {}

	std::unique_ptr<MagicProjectile> createItem(std::string name, Vector2D position, bool flip) {
		if (name == "fire ball") {
			return std::make_unique<FireBall>(_gEventSystem, createEntity(), position, flip);
		}
		else if (name == "ice ball") {
			return std::make_unique<IceBall>(_gEventSystem, createEntity(), position, flip);
		}
	}
};