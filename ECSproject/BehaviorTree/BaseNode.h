#pragma once

#include "tinybehaviortree.h"
#include "../ECS/AIContext.h"
#include <string>
#include <iostream>

using namespace TBT;

class TryTracking : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class Tracking : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class Patrol : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

