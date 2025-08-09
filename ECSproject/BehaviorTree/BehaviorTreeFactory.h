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

class SetFighingState : public BehaviorNode<AIstate&, AIContext&> {
public:
	BehaviorResult tick(AIstate& state, AIContext& context) override;
};

class SetExitFightingState : public BehaviorNode<AIstate&, AIContext&> {
public:
	BehaviorResult tick(AIstate& state, AIContext& context) override;
};

class TryBacking : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class Backing : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class BehaviorTreeFactory {
public:
	BehaviorTree<AIstate&, AIContext&> buildTree();
};