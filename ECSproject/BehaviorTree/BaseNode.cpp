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

	context.getMovement()->setToDefaultSpeed();

	Vector2D playerPos = context.getPlayerPos();
	Vector2D direction = (playerPos - context.getTransform()->position).normalize();
	context.getMovement()->setDirection(direction);
	context.getSprite()->setAnimate(direction.x >= 0 ? "walkR" : "walkL");
	// std::cout << "tracking" << context.getDist() << std::endl;
	return BehaviorResult::RUNNING;
}

BehaviorResult KeepDistance::tick(AIContext& context)
// try keep dist between play 100
{
	const float desiredDistance = 200.f; // target distance
	const float tolerance = 5.f;        // +/- tolerance

	float dist = context.getDist();

	// Already in good range
	if (dist > (desiredDistance - tolerance) && dist < (desiredDistance + tolerance))
		return BehaviorResult::SUCCESS;

	context.getMovement()->setToDefaultSpeed();

	Vector2D playerPos = context.getPlayerPos();
	Vector2D selfPos = context.getTransform()->position;

	// Compute direction vector
	Vector2D direction;

	if (dist < (desiredDistance - tolerance))
	{
		// Too close ¡÷ move away from player
		direction = (selfPos - playerPos).normalize();
	}
	else
	{
		// Too far ¡÷ move toward player
		direction = (playerPos - selfPos).normalize();
	}

	// Move character
	// selfPos += direction * context.getMoveSpeed() * context.getDeltaTime();
	// context.getTransform()->position = selfPos;

	// Update facing direction and animation
	context.getMovement()->setDirection(direction);
	context.getSprite()->setAnimate(direction.x >= 0 ? "walkR" : "walkL");

	return BehaviorResult::RUNNING;
}

BehaviorResult Patrol::tick(AIContext& context) 
{
	context.getMovement()->stop();
	context.getSprite()->setAnimate("idle");

	return BehaviorResult::SUCCESS;
}

BehaviorResult setExhaustion::tick(AIContext& context)
{
	context.getState()->setRestingTime(10.0);

	return BehaviorResult::SUCCESS;
}

BehaviorResult Exhaustion::tick(AIContext& context)
{
	if(context.getState()->isResting()){
		return BehaviorResult::RUNNING;
	}

	return BehaviorResult::SUCCESS;
}