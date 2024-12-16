//
// Collision.h

#pragma once

#include "ColliderComponent.h"

enum Direction {
	None, Left, Right, Up, Down
};

class Collision
{
public:

	static bool AABB(const BoundingBox& boxA, const BoundingBox& boxB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

	static Direction collisionDirect(const BoundingBox& player, const BoundingBox& tiles);
	static Vector2D calculateMTV(const BoundingBox& boxA, const BoundingBox& boxB);
};

