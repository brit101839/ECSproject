#include "Collision.h"

bool Collision::AABB(const BoundingBox& boxA, const BoundingBox& boxB)
{
	return !(boxA.position.x + boxA.width / 2 <= boxB.position.x - boxB.width / 2 ||
		boxA.position.x - boxA.width / 2 >= boxB.position.x + boxB.width / 2 ||
		boxA.position.y + boxA.height / 2 <= boxB.position.y - boxB.height / 2 ||
		boxA.position.y - boxA.height / 2 >= boxB.position.y + boxB.height / 2);
}
