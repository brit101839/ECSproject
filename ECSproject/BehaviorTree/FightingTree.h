#pragma once

#include "tinybehaviortree.h"
#include "../ECS/AIContext.h"

using namespace TBT;

class CanUseSkill : public BehaviorNode<AIContext&> {
    BehaviorResult tick(AIContext& context) override {
        if (context.getBehavior() == AIPreferBehavior::NormalAttackOnly)
            return BehaviorResult::FAILURE;
        return BehaviorResult::SUCCESS;
    }
};

class ShouldUseSkill : public BehaviorNode<AIContext&> {
public:
    BehaviorResult tick(AIContext& context) override {
        if (context.getDist() > 100.0f) return BehaviorResult::SUCCESS;
        else if (context.getSkill()->getType() == SkillType::ParallelRange) {
            if (context.getPlayerPos().x - context.getTransform()->position.x < 100.f) return BehaviorResult::SUCCESS;
        }
        return BehaviorResult::FAILURE;
    }
};

class UsingSkill : public BehaviorNode<AIContext&> {
public:
    BehaviorResult tick(AIContext& context) override {
        context.getTransform()->setDirection(Vector2D(0.f, 0.f));
        context.getSkill()->UseSkill();
        return BehaviorResult::SUCCESS;
    }
};

class CanNormalAttack : public BehaviorNode<AIContext&> {
public:
    BehaviorResult tick(AIContext& context) override {
        if (context.getBehavior() == AIPreferBehavior::SkillOnly) 
            return BehaviorResult::FAILURE;
        return BehaviorResult::SUCCESS;
    }
};

class ShouldNormalAttack : public BehaviorNode<AIContext&> {
public:
    BehaviorResult tick(AIContext& context) override {
        if (context.getDist() < 100.0f) return BehaviorResult::SUCCESS;
        return BehaviorResult::FAILURE;
    }
};

class UseNormalAttack : public BehaviorNode<AIContext&> {
public:
    BehaviorResult tick(AIContext& context) override {
        if (context.getDist() > 100.0f) return BehaviorResult::SUCCESS;

        context.getTransform()->setDirection(Vector2D(0.f, 0.f));
        BoundingBox box;
        if (context.getSprite()->getFlip()) box = { context.getTransform()->position - Vector2D(40.f, 0.f), context.getTransform()->width, context.getTransform()->height };
        else box = { context.getTransform()->position + Vector2D(40.f, 0.f), context.getTransform()->width, context.getTransform()->height };
        context.getAttack()->startAttack(box);
        std::cout << "fighting" << std::endl;
        return BehaviorResult::RUNNING;
    }
};

class FightingTreeFactory {
public:

    std::unique_ptr<BehaviorNode<AIContext&>> buildTree() {

        auto rootNode = std::make_unique<FallbackNode<AIContext&>>();
            auto tryToUseSkill = std::make_unique<SequenceNode<AIContext&>>();
            tryToUseSkill.get()->addChild(std::make_unique<CanUseSkill>());
            tryToUseSkill.get()->addChild(std::make_unique<ShouldUseSkill>());
            tryToUseSkill.get()->addChild(std::make_unique<UsingSkill>());
        rootNode.get()->addChild(std::move(tryToUseSkill));
            auto tryToNormalAttack = std::make_unique<SequenceNode<AIContext&>>();
            tryToNormalAttack.get()->addChild(std::make_unique<CanNormalAttack>());
            tryToNormalAttack.get()->addChild(std::make_unique<ShouldNormalAttack>());
            tryToNormalAttack.get()->addChild(std::make_unique<UseNormalAttack>());
            /*tryToNormalAttack.get()->addChild(
                std::make_unique<LoggingDecorator<AIContext&>>("UseNormalAttack", std::make_unique<UseNormalAttack>()));*/
        rootNode.get()->addChild(std::move(tryToNormalAttack));

        return rootNode;
    }
};