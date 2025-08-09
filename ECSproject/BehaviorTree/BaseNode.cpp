#include "BaseNode.h"

BehaviorResult TryTracking::tick(AIContext& context)
{
	return (context.getDist() < 400.f) ? BehaviorResult::SUCCESS : BehaviorResult::FAILURE;
}

BehaviorResult Tracking::tick(AIContext& context)
// if distance is 100 ~ 400, tracking the enemy
{
	if (context.getDist() > 400.f) return BehaviorResult::FAILURE;
	else if (context.getDist() < 100.f) return BehaviorResult::SUCCESS;

	Vector2D playerPos = context.getPlayerPos();
	auto direction = (playerPos - context.getTransform()->position).normalize();
	context.getTransform()->setDirection(direction);
	context.getSprite()->setAnimate(direction.x >= 0 ? "walkR" : "walkL");
	// std::cout << "tracking" << context.getDist() << std::endl;
	return BehaviorResult::RUNNING;
}

BehaviorResult Patrol::tick(AIContext& context) 
{
	context.getTransform()->setDirection(Vector2D(0.f, 0.f));
	context.getSprite()->setAnimate("idle");

	return BehaviorResult::SUCCESS;
}