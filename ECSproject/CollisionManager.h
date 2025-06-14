#pragma once

#include "Quadtree/Quadtree.h"
#include "Quadtree/Box.h"
#include "ECS/Collision.h"
#include "Collider.h"
#include "EventSystem.h"
#include "ECS/ColliderComponent.h"
#include "ECS/ProjectileComponent.h"

class CollisionManager {
private:
	EventSystem& _globalEventSys;

	using ColliderPtr = std::shared_ptr<Collider>;

	struct GetBox {
		quadtree::Box<float> operator()(const ColliderPtr& collider) const {
			return collider->box;
		}
	};

	using ColliderTree = quadtree::Quadtree<ColliderPtr, GetBox>;
	ColliderTree collidertree;

public:

	CollisionManager(quadtree::Box<float> interBox, EventSystem& GeventM)
		:collidertree(interBox), _globalEventSys(GeventM)
	{
		_globalEventSys.subscribe<AddColliderEvent>([this](Event& event) {
			addCollider(event); });

		_globalEventSys.subscribe<RemoveColliderEvent>([this](Event& event) {
			removeCollider(event); });
	}

	void addCollider(Event& event) {
		auto& colliderEvent = static_cast<AddColliderEvent&>(event);
		ColliderPtr collider = colliderEvent.collider;

		// std::cout << "[AddCollider] box: " << collider->box.left << ", " << collider->box.top << ", " << collider->box.width << ", " << collider->box.height << std::endl;
		if (collider) {
			collidertree.add(collider);
		}
		else std::cerr << "collider adding event loss pointer!" << std::endl;
	}

	void removeCollider(Event& event) {
		auto& colliderEvent = static_cast<RemoveColliderEvent&>(event);
		ColliderPtr collider = colliderEvent.collider;

		if (collider) {
			collidertree.remove(collider);
		}
		else std::cerr << "collider removing event loss pointer!" << std::endl;
	}

	void updateCollider(ColliderPtr collider) {
		collidertree.remove(collider);
		collidertree.add(collider);
	}

	std::vector<ColliderPtr> query(const quadtree::Box<float>& area) {
		return collidertree.query(area);
	}

	void hittingObstacle(ColliderPtr collider, Entity* player) {
		std::cout << "collision: " << collider->tag << std::endl;
		std::cout << "position:" << player->getComponent<TransformComponent>().position.x << ", " << player->getComponent<TransformComponent>().position.y << std::endl;
		Vector2D mtv = Collision::calculateMTV(player->getComponent<ColliderComponent>().getBoundingBox(), collider->boundingBox);
		player->getComponent<TransformComponent>().position += mtv;
		// player->getComponent<TransformComponent>().update(window);
	}

	void hittingSpell(ColliderPtr collider, Entity* player) {
		std::cout << "spell on hit: " << collider->tag << std::endl;
		std::cout << "position:" << player->getComponent<TransformComponent>().position.x << ", " << player->getComponent<TransformComponent>().position.y << std::endl;
		if(!collider->entity->hasComponent<ProjectileComponent>()) {
			std::cerr << "hitting spell without projectile component!" << std::endl;
			return;
		}
		collider->entity->getComponent<ProjectileComponent>().explosion();
		
	}

	void checkCollisions(Entity* player, const quadtree::Box<float>& area, GLFWwindow* window) {
		for (auto cc : query(area)) {
			if (cc->tag != "player" && Collision::AABB(player->getComponent<ColliderComponent>().getBoundingBox(), cc->boundingBox)) {
				if (cc->type == ColliderType::mapObstacle) {
					hittingObstacle(cc, player);
				}
				else if(cc->type == ColliderType::spell) {
					hittingSpell(cc, player);
				}

			}
		}
		// std::cout << "collider in screen: " << query(area).size() << std::endl;
	}
};