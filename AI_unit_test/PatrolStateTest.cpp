#include <gtest/gtest.h>
#include "pch.h"
#include "/Users/brit/source/repos/ECSproject/ECSproject/StateMaching/AIStates.h"  // 包含你定義的 AIState 類
#include "/Users/brit/source/repos/ECSproject/ECSproject/ECS/AIContext.h"  // 假設你的 AIContext 類別定義在這裡
#include <cmath>

// Mock AIContext for testing
class MockAIContext : public AIContext {
public:
    Vector2D playerPos = Vector2D(0.f, 0.f);
    Vector2D aiPos = Vector2D(0.f, 0.f);

    MockAIContext(Entity* entity, const TransformComponent& playerTrans, AIPreferBehavior behavior) : AIContext(entity, playerTrans, behavior) {};


    // Override getPlayerPos() and getTransform() for testing purposes
    const Vector2D& getPlayerPos() const {
        return playerPos;
    }

    TransformComponent* getTransform() {
        auto* transform = new TransformComponent();
        transform->position = aiPos;
        return transform;
    }
};

// Test TrackingState for transitioning to FightingState
TEST(TrackingStateTest, TransitionToFightingState) {
    Manager manager;
    auto* transform = new TransformComponent();

    MockAIContext* ai = new MockAIContext(&manager.addEntity(), *transform, AIPreferBehavior::NormalAttackOnly);
    TrackingState trackingState;

    trackingState.enter(*ai);  // Enter the tracking state

    // Set player position to be very close to AI (within 100f)
    ai->playerPos = Vector2D(50.f, 0.f);
    trackingState.update(*ai);  // Update the state

    // Assert that the state transitions to fighting
    EXPECT_EQ(trackingState.getNextState(*ai), EnemyState::fighting);
}

//// Test TrackingState for transitioning to BackingState
//TEST(TrackingStateTest, TransitionToBackingState) {
//    MockAIContext ai;
//    TrackingState trackingState;
//
//    trackingState.enter(ai);  // Enter the tracking state
//
//    // Set player position to be far away (greater than 400f)
//    ai.playerPos = Vector2D(500.f, 0.f);
//    trackingState.update(ai);  // Update the state
//
//    // Assert that the state transitions to backing
//    EXPECT_EQ(trackingState.getNextState(ai), EnemyState::backing);
//}
//
//// Test TrackingState for staying in TrackingState
//TEST(TrackingStateTest, StayInTrackingState) {
//    MockAIContext ai;
//    TrackingState trackingState;
//
//    trackingState.enter(ai);  // Enter the tracking state
//
//    // Set player position to be within a reasonable distance (between 100f and 400f)
//    ai.playerPos = Vector2D(300.f, 0.f);
//    trackingState.update(ai);  // Update the state
//
//    // Assert that the state remains tracking
//    EXPECT_EQ(trackingState.getNextState(ai), EnemyState::tracking);
//}