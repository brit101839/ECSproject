#pragma once

#include "tinybehaviortree.h"
#include "FightingTree.h"
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
    BehaviorResult tick(AIstate& state, AIContext& context) override {
        if (context.getAttack()->attacking) {
            // std::cout << "should not exit fighting" << std::endl;
            return BehaviorResult::RUNNING;
        }
        else if (context.getSkill()->usingSkill) {
            return BehaviorResult::RUNNING;
        }
        else if (context.getDist() < 100.f) {
            return BehaviorResult::FAILURE;
        }
        // state.fighting = false;
        // std::cout << "should exit fighting" << std::endl;
        return BehaviorResult::SUCCESS;
    }
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

TBT_PEEL_NODE_MACRO(PeelAIstate, TBT_ARGS(AIstate&, AIContext&), AIstate&, { std::get<0>(payload) })
TBT_PEEL_NODE_MACRO(PeelAIContext, TBT_ARGS(AIstate&, AIContext&), AIContext&, { std::get<1>(payload) })

class BehaviorTreeFactory {
public:

    BehaviorTree<AIstate&, AIContext&> buildTree(){
        auto wrapPeelAIstate = [](std::unique_ptr<BehaviorNode<AIstate&>>&& child) {
            auto peelAIstate = std::make_unique<PeelAIstate>();
            peelAIstate->setChild(std::move(child));
            return std::move(peelAIstate);
            };
        auto wrapPeelAIContext = [](std::unique_ptr<BehaviorNode<AIContext&>>&& child) {
            auto peelAIContext = std::make_unique<PeelAIContext>();
            peelAIContext->setChild(std::move(child));
            return std::move(peelAIContext);
            };

        FightingTreeFactory fightingTreeFactory;
        auto fightingRoot = fightingTreeFactory.buildTree();

        auto rootNode = std::make_unique<FallbackNode<AIstate&, AIContext&>>();
            auto tryFighting = std::make_unique<SequenceNode<AIstate&, AIContext&>>();
                auto enterFighting = std::make_unique<FallbackNode<AIstate&, AIContext&>>();
                    enterFighting.get()->addChild(wrapPeelAIstate(std::make_unique<IsFighting>()));
                    enterFighting.get()->addChild(wrapPeelAIContext(std::make_unique<TryFighting>()));
                tryFighting.get()->addChild(std::move(enterFighting));
                tryFighting.get()->addChild(wrapPeelAIstate(std::make_unique<SetFighingState>()));
                tryFighting.get()->addChild(wrapPeelAIContext(std::move(fightingRoot)));
               /* tryFighting.get()->addChild(wrapPeelAIContext(
                    std::make_unique<LoggingDecorator<AIContext&>>("FightingRoot", std::move(fightingRoot))));*/
                auto invertExit = std::make_unique<InvertNode<AIstate&, AIContext&>>();
                    invertExit.get()->setChild(std::make_unique<TryExitFighting>());
                tryFighting.get()->addChild(std::move(invertExit));
        rootNode.get()->addChild(std::move(tryFighting));
        // rootNode.get()->addChild(wrapPeelAIstate(std::make_unique<IsFighting>()));

            auto tryTracking = std::make_unique<SequenceNode<AIstate&, AIContext&>>();
            tryTracking.get()->addChild(wrapPeelAIContext(std::make_unique<TryTracking>()));
            tryTracking.get()->addChild(wrapPeelAIContext(std::make_unique<Tracking>()));
        rootNode.get()->addChild(std::move(tryTracking));

            auto tryBacking = std::make_unique<SequenceNode<AIstate&, AIContext&>>();
            tryBacking.get()->addChild(wrapPeelAIContext(std::make_unique<TryBacking>()));
            tryBacking.get()->addChild(wrapPeelAIContext(std::make_unique<Backing>()));
        rootNode.get()->addChild(std::move(tryBacking));
        rootNode.get()->addChild(wrapPeelAIContext(std::make_unique<Patrol>()));

        BehaviorTree<AIstate&, AIContext&> tree;
        tree.setRoot(std::move(rootNode));

        return tree;
    }
};