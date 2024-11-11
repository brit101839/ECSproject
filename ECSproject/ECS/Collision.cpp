#include "Collision.h"

bool Collision::AABB(const BoundingBox& boxA, const BoundingBox& boxB)
{
	return !(boxA.position.x + boxA.width / 2 <= boxB.position.x - boxB.width / 2 ||
		boxA.position.x - boxA.width / 2 >= boxB.position.x + boxB.width / 2 ||
		boxA.position.y + boxA.height / 2 <= boxB.position.y - boxB.height / 2 ||
		boxA.position.y - boxA.height / 2 >= boxB.position.y + boxB.height / 2);
}

Direction Collision::collisionDirect(const BoundingBox& player, const BoundingBox& box)
{
	float t_collision = (player.position.y + player.height / 2) - (box.position.y - box.height / 2);
	float b_collision = (box.position.y + box.height / 2) - (player.position.y - player.height / 2);
	float r_collision = (player.position.x + player.width / 2) - (box.position.x - box.width / 2);
	float l_collision = (box.position.x + box.width / 2) - (player.position.x - player.width / 2);

	if (t_collision < b_collision && t_collision < r_collision && t_collision < l_collision) return Up;
	if (b_collision < t_collision && b_collision < r_collision && b_collision < l_collision) return Down;
	if (r_collision < t_collision && r_collision < b_collision && r_collision < l_collision) return Right;
	if (l_collision < t_collision && l_collision < b_collision && l_collision < r_collision) return Left;

	return None;
}
