#pragma once

#include "../ECS/ECS.h"
#include "MagicItem.h"
#include <vector>

class SpawnSystem {
private:

	Manager& _manager;
	std::vector<Entity*> _entitys;

	Entity* createEntity() {
		Entity* newEntity = &_manager.addEntity();
		_entitys.push_back(newEntity);
		return newEntity;
	}

public:

	SpawnSystem(Manager& m)
		:_manager(m) {}

	std::unique_ptr<MagicItem> createItem(std::string name, Vector2D position, bool flip) {
		if (name == "fire ball") {
			return std::make_unique<FireBall>(createEntity(), position, flip);
		}
		else if (name == "ice ball") {
			return std::make_unique<IceBall>(createEntity(), position, flip);
		}
	}

	void render(Shader& shader, const Vector2D& cameraPos) {
		for (auto* entity : _entitys) {
			if (entity->isActive()) {
				entity->draw(shader, cameraPos);
			}
		}
	}
};