#pragma once

#include "Quadtree/Quadtree.h"
#include "Quadtree/Box.h"
#include "ECS/ColliderComponent.h"
#include "ECS/Collision.h"

class CollisionManager {
private:

	struct GetBox {
		quadtree::Box<float> operator()(ColliderComponent* collider) const {
			return collider->getBox();
		}
	};

	using ColliderTree = quadtree::Quadtree<ColliderComponent*, GetBox>;
	ColliderTree collidertree;

public:

	CollisionManager(quadtree::Box<float> interBox)
		:collidertree(interBox)
	{
	}

	void addCollider(ColliderComponent* trans) {
		collidertree.add(trans);
	}

	void updateCollider(ColliderComponent* trans) {
		collidertree.remove(trans);
		collidertree.add(trans);
	}

	std::vector<ColliderComponent*> query(const quadtree::Box<float>& area) {

		return collidertree.query(area);
	}

	void checkCollisions(Entity* player, const quadtree::Box<float>& area, GLFWwindow* window) {
		for (auto cc : query(area)) {
			if (cc->tag != "player" && Collision::AABB(player->getComponent<ColliderComponent>().boundingBox, cc->boundingBox)) {
				std::cout << "collision: " << cc->tag << std::endl;
				std::cout << "position:" << player->getComponent<TransformComponent>().position.x << ", " << player->getComponent<TransformComponent>().position.y << std::endl;
				Vector2D mtv = Collision::calculateMTV(player->getComponent<ColliderComponent>().boundingBox, cc->boundingBox);
				player->getComponent<TransformComponent>().position += mtv;
				// player->getComponent<TransformComponent>().update(window);
			}
		}
	}
};