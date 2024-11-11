//
// Collision.h

#pragma once

#include "ColliderComponent.h"

class Collision
{
public:

	static bool AABB(const BoundingBox& transA, const BoundingBox& transB);

};

