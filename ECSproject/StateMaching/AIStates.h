#pragma once

// #include "StateMaching.h"
#include "../ECS/AIContext.h"

enum class EnemyState
{
    patrol, tracking, fighting, backing
};

class AIState {
public:

    virtual ~AIState() = default;

    virtual void enter() {}
    virtual void update(AIContext& ai) = 0;
    virtual void exit() {}

    virtual EnemyState getNextState(AIContext& ai) { return EnemyState::patrol; }
};

class PatrolState : public AIState {
public:
    void update(AIContext& ai) override {
        ai.getTransform()->velocity = Vector2D(0.f, 0.f);
        ai.getSprite()->setAnimate("idle");
    }

    EnemyState getNextState(AIContext& ai) override {
        auto dist = ai.getTransform()->position.distanceTo(ai.getPlayerPos());
        if (dist < 300.f) return EnemyState::tracking;
        return EnemyState::patrol;
    }
};

class FightingState : public AIState {
public:
    void update(AIContext& ai) override {
        ai.getTransform()->velocity = Vector2D(0.f, 0.f);
        ai.getSprite()->setAnimate("attack_1");
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
            ai.getTransform()->velocity = Vector2D(0.f, 0.f);
            return;
        }
        Vector2D playerPos = ai.getPlayerPos();
        auto direction = (playerPos - ai.getTransform()->position).normalize();
        ai.getTransform()->velocity = direction;
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
            ai.getTransform()->velocity = Vector2D(0.f, 0.f);
            return;
        }
        Vector2D defaultPos = ai.getDefaultPos();
        ai.getTransform()->velocity = (defaultPos - ai.getTransform()->position).normalize();
        if (ai.getTransform()->velocity.x >= 0) { ai.getSprite()->setAnimate("walkR"); }
        else { ai.getSprite()->setAnimate("walkL"); }
    }

    EnemyState getNextState(AIContext& ai) override {
        auto dist = ai.getTransform()->position.distanceTo(ai.getDefaultPos());
        if (dist < 50.f) return EnemyState::patrol;
        return EnemyState::backing;
    }
};