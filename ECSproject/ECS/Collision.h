//
// Collision.h

#pragma once

#include "../BoundingBox.h"
#include "Quadtree/Box.h"

enum Direction {
	None, Left, Right, Up, Down
};

class Collision
{
public:

	static bool AABB(const BoundingBox& boxA, const BoundingBox& boxB);
	// static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static bool isOutOfBound(quadtree::Box<float> Bound, const BoundingBox& box, float margin = 0.0f);

	static Direction collisionDirect(const BoundingBox& player, const BoundingBox& tiles);
	static Vector2D calculateMTV(const BoundingBox& boxA, const BoundingBox& boxB);
};

