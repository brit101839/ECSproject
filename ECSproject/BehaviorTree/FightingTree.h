#pragma once

#include "tinybehaviortree.h"
#include "../ECS/AIContext.h"

using namespace TBT;

class CanUseSkill : public BehaviorNode<AIContext&> 
{
	BehaviorResult tick(AIContext& context) override;
};

class ShouldUseSkill : public BehaviorNode<AIContext&> 
{
public:
	BehaviorResult tick(AIContext& context) override;
};

class CanUseParallelRangeSkill : public BehaviorNode<AIContext&>
{
public:
	BehaviorResult tick(AIContext& context) override;
};

class UsingSkill : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class CanNormalAttack : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class ShouldNormalAttack : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class UseNormalAttack : public BehaviorNode<AIContext&> {
public:
	BehaviorResult tick(AIContext& context) override;
};

class FightingTreeFactory 
{
public:
	std::unique_ptr<BehaviorNode<AIstate&, AIContext&>> buildFightingTree( void );
};