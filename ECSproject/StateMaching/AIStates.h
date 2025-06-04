#pragma once

// #include "StateMaching.h"
#include "../ECS/AIContext.h"

class AIState {
public:

    virtual ~AIState() = default;

    virtual void enter(AIContext& ai) {}
    virtual void update(AIContext& ai) = 0;
    virtual void exit() {}

    virtual EnemyState getNextState(AIContext& ai) { return EnemyState::patrol; }
};

class ParentState : public AIState {
protected:
    AIState* _currentChildState = nullptr;

public:
    void setChildState(AIState* newChildState, AIContext& ai) {
        if (_currentChildState) {
            _currentChildState->exit();
            delete _currentChildState;
        }
        _currentChildState = newChildState;
        _currentChildState->enter(ai);
    }

    virtual void update(AIContext& ai) override {
        if (_currentChildState) {
            _currentChildState->update(ai);
        }
        else std::cout << "miss child state" << std::endl;
    }

    virtual ~ParentState() {
        if (_currentChildState) delete _currentChildState;
    }
};

class PatrolState : public AIState {
public:
    void update(AIContext& ai) override {
        ai.getTransform()->setDirection(Vector2D(0.f, 0.f));
        ai.getSprite()->setAnimate("idle");
    }

    EnemyState getNextState(AIContext& ai) override {
        auto dist = ai.getTransform()->position.distanceTo(ai.getPlayerPos());
        if (dist < 300.f) return EnemyState::tracking;
        return EnemyState::patrol;
    }
};

class SkillFightingState : public AIState {
public:
    void update(AIContext& ai) override {
        /*ai.getTransform()->velocity = Vector2D(0.f, 0.f);
        BoundingBox box;
        if (ai.getSprite()->getFlip()) box = { ai.getTransform()->position - Vector2D(40.f, 0.f), ai.getTransform()->width, ai.getTransform()->height };
        else box = { ai.getTransform()->position + Vector2D(40.f, 0.f), ai.getTransform()->width, ai.getTransform()->height };*/
        ai.getSkill()->UseSkill();
    }
};

class NormalAttackState : public AIState {
    void update(AIContext& ai) override {
        ai.getTransform()->setDirection(Vector2D(0.f, 0.f));
        BoundingBox box;
        if (ai.getSprite()->getFlip()) box = { ai.getTransform()->position - Vector2D(40.f, 0.f), ai.getTransform()->width, ai.getTransform()->height };
        else box = { ai.getTransform()->position + Vector2D(40.f, 0.f), ai.getTransform()->width, ai.getTransform()->height };
        ai.getAttack()->startAttack(box);
        // ai.getSprite()->setAnimate("attack_1");
    }
};

class FightingState : public ParentState {
public:
    void enter(AIContext& ai) override {
        switch (ai.getBehavior()) {
        case AIPreferBehavior::SkillOnly:
            setChildState(new SkillFightingState, ai);
            break;
        case AIPreferBehavior::NormalAttackOnly:
            setChildState(new NormalAttackState, ai);
            break;
        }
    }

    void update(AIContext& ai) override {
        ParentState::update(ai);
    }

    EnemyState getNextState(AIContext& ai) override {
        auto dist = ai.getTransform()->position.distanceTo(ai.getPlayerPos());
        if (dist > 100.f) {
            return EnemyState::tracking; // 切换到追踪状态
        }
        return EnemyState::fighting; // 保持当前状态
    }
};

class TrackingState : public AIState {
public:
    void update(AIContext& ai) override {
        if (!ai.getTransform()->canMove) {
            ai.getTransform()->setDirection(Vector2D(0.f, 0.f));
            return;
        }
        Vector2D playerPos = ai.getPlayerPos();
        auto direction = (playerPos - ai.getTransform()->position).normalize();
        ai.getTransform()->setDirection(direction);
        ai.getSprite()->setAnimate(direction.x >= 0 ? "walkR" : "walkL");
    }

    EnemyState getNextState(AIContext& ai) override {
        auto dist = ai.getTransform()->position.distanceTo(ai.getPlayerPos());
        if (dist < 100.f) {
            return EnemyState::fighting; // 切换到战斗状态
        }
        else if (dist > 400.f) {
            return EnemyState::backing; // 切换到返回状态
        }
        return EnemyState::tracking; // 保持当前状态
    }
};

class BackingState : public AIState {
public:
    void update(AIContext& ai) override {
        if (!ai.getTransform()->canMove) {
            ai.getTransform()->setDirection(Vector2D(0.f, 0.f));
            return;
        }
        Vector2D defaultPos = ai.getDefaultPos();
        ai.getTransform()->setDirection((defaultPos - ai.getTransform()->position).normalize());
        if (ai.getTransform()->getVelocity().x >= 0) { ai.getSprite()->setAnimate("walkR"); }
        else { ai.getSprite()->setAnimate("walkL"); }
    }

    EnemyState getNextState(AIContext& ai) override {
        auto dist = ai.getTransform()->position.distanceTo(ai.getDefaultPos());
        if (dist < 50.f) return EnemyState::patrol;
        return EnemyState::backing;
    }
};

class DyingState : public AIState {
public:
    void update(AIContext& ai) override {
        ai.getTransform()->setDirection(Vector2D(0.f, 0.f));
        ai.getSprite()->setAnimate("dying");
    }

    EnemyState getNextState(AIContext& ai) override {
        return EnemyState::dying;
    }
};

class AIStateFactory {
public:
    static AIState* createState(EnemyState stateName) {
        switch (stateName)
        {
        case EnemyState::patrol:
            return new PatrolState();
        case EnemyState::tracking:
            return new TrackingState();
        case EnemyState::fighting:
            return new FightingState();
        case EnemyState::backing:
            return new BackingState();
        case EnemyState::dying:
            return new DyingState();
        case EnemyState::skillfight:
            return new SkillFightingState();
        case EnemyState::normalAttack:
            return new NormalAttackState();
        default:
            break;
        }
        return nullptr;
    }
};