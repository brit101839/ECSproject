#pragma once
#include "quadtree/Box.h"
#include "BoundingBox.h"
#include "ECS/ECS.h"

enum class ColliderType
{
	role, spell, mapObstacle
};

struct Collider {
	BoundingBox boundingBox;
	quadtree::Box<float> box;
	ColliderType type;
	std::string tag;
	Entity* entity; // optional fallback to ECS

	Collider(const std::string& tag, ColliderType t = ColliderType::mapObstacle, Entity* e = nullptr)
		: tag(tag), type(t), entity(e) {
	}

	Collider(const BoundingBox& bb, const quadtree::Box<float>& b, const std::string& tag, ColliderType t = ColliderType::mapObstacle, Entity* e = nullptr)
		: boundingBox(bb), box(b), tag(tag), type(t), entity(e) {
	}
};