#pragma once

#include "tinybehaviortree.h"
#include "../ECS/AIContext.h"
#include <string>
#include <iostream>

using namespace TBT;

class IsFighting : public BehaviorNode<AIstate&> {
public:
	BehaviorResult tick(AIstate& state) override {
		return state.fighting ? BehaviorResult::SUCCESS : BehaviorResult::FAILURE;
	}
};

class TryExitFighting : public BehaviorNode<AIstate&, AIContext&> {
public:
	BehaviorResult tick(AIstate& state, AIContext& context) override;
};

class TryFighting : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override {
		if (context.getDist() > 100.0f) return BehaviorResult::FAILURE;
		return BehaviorResult::SUCCESS;
	}
};

class SetFighingState : public BehaviorNode<AIstate&> {
public:
	BehaviorResult tick(AIstate& state) override {
		state.fighting = true;
		return BehaviorResult::SUCCESS;
	}
};

class TryTracking : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override {
		return (context.getDist() < 400.f) ? BehaviorResult::SUCCESS : BehaviorResult::FAILURE;
	}
};

class Tracking : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override {
		if (context.getDist() > 400.f) return BehaviorResult::FAILURE;
		else if (context.getDist() < 100.f) return BehaviorResult::SUCCESS;
		Vector2D playerPos = context.getPlayerPos();
		auto direction = (playerPos - context.getTransform()->position).normalize();
		context.getTransform()->setDirection(direction);
		context.getSprite()->setAnimate(direction.x >= 0 ? "walkR" : "walkL");
		// std::cout << "tracking" << context.getDist() << std::endl;
		return BehaviorResult::RUNNING;
	}
};

class TryBacking : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override {
		Vector2D defaultPos = context.getDefaultPos();
		auto dist = context.getTransform()->position.distanceTo(defaultPos);
		if (dist < 80.f) return BehaviorResult::FAILURE;
		return BehaviorResult::SUCCESS;
	}
};

class Backing : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override {
		Vector2D defaultPos = context.getDefaultPos();
		auto dist = context.getTransform()->position.distanceTo(defaultPos);
		if (dist < 80.f) return BehaviorResult::SUCCESS;
		context.getTransform()->setDirection((defaultPos - context.getTransform()->position).normalize());
		if (context.getTransform()->getVelocity().x >= 0) { context.getSprite()->setAnimate("walkR"); }
		else { context.getSprite()->setAnimate("walkL"); }
		// std::cout << "backing" << context.getTransform()->position.distanceTo(defaultPos) << std::endl;
		return BehaviorResult::RUNNING;
	}
};

class Patrol : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override {
		context.getTransform()->setDirection(Vector2D(0.f, 0.f));
		context.getSprite()->setAnimate("idle");
		// std::cout << "patrol" << std::endl;
		return BehaviorResult::SUCCESS;
	}
};

class BehaviorTreeFactory {
public:
	BehaviorTree<AIstate&, AIContext&> buildTree();
};