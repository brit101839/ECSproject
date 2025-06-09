#pragma once

#include "ECS/ECS.h"
#include "ECS/ColliderComponent.h"

class ProjectileManager {
private:
	std::vector<Entity*> _entitys;

public:
	ProjectileManager() = default;

	~ProjectileManager() = default;

	void addEntity(Entity* entity) {
		_entitys.push_back(entity);
	}

	void removeEntity(Entity* entity) {
		auto it = std::remove(_entitys.begin(), _entitys.end(), entity);
		if (it != _entitys.end()) {
			_entitys.erase(it, _entitys.end());
		}
	}

	void render(Shader& shader, const Vector2D& cameraPos) {
		for (auto* entity : _entitys) {
			if (entity->isActive()) {
				entity->draw(shader, cameraPos);
			}
		}
	}

	void removeOutOfBounds(quadtree::Box<float> cameraBound) {
		for(auto* entity : _entitys) {
			if (entity->isActive() && entity->getComponent<ColliderComponent>().isOutOfBounds(cameraBound)) {
				removeEntity(entity);
				entity->destroy();
			}
		}

		// std::cout << "number of projectiles: " << _entitys.size() << std::endl;
	}
};