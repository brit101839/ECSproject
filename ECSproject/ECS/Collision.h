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

	static Direction collisionDirect(const BoundingBox& player, const BoundingBox& tiles);
};

