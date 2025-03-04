#pragma once

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Animation.h"
#include "CollisionManager.h"
#include "EventManager.h"
// #include "Enemy/EnemyManager.h"

class Player {
private:
    Entity* _entity;
    EventManager& _globalEventManager;
    EventManager _localEventManager;
    std::string _name = "player";

public:
    Player(Entity* entity, EventManager& eventManager) : _entity(entity), _globalEventManager(eventManager) {
        _globalEventManager.subscribe<AttackStepEvent>([this](Event& event) {
            onAttackStepEvent(event); });
    }

    void componentSetting(CollisionManager* mCollisionM) {
        auto trans = _entity->addcomponent<TransformComponent>(Vector2D(550.0f, -1200.0f), Vector2D(0.0f, 0.0f), 1.0f, 100.0f, 100.0f);
        auto& playerSprite = _entity->addcomponent<SpriteComponent>("D:/dependencies/resource/Dungeon/Adventurer Sprite Sheet v1.5.png", true, 32.f, 32.f, &_localEventManager);
        playerSprite.addAnimation("idle", Animation(0, 13, 10, false));
        playerSprite.addAnimation("walkL", Animation(1, 8, 10, true));
        playerSprite.addAnimation("walkR", Animation(1, 8, 10, false));
        playerSprite.addAnimation("walkUp", Animation(11, 8, 10, false));
        playerSprite.addAnimation("attack_1", Animation(2, 8, 10, false, false, AnimateState::Attacking));
        playerSprite.addAnimation("attack_2", Animation(3, 8, 10, false, false, AnimateState::Attacking));
        playerSprite.addAnimation("attack_3", Animation(4, 10, 10, false, false, AnimateState::Attacking));
        playerSprite.setAnimate("idle");
        _entity->addcomponent<KeyboardController>();

        BoundingBox bound{ trans.position, 40.0f, 40.0f };
        _entity->addcomponent<ColliderComponent>(mCollisionM, "player", bound, Vector2D(0.f, -40.f));
        _entity->addcomponent<StatsComponent>(100, 10, 1);
        _entity->addcomponent<AttackComponent>(_name, _globalEventManager, _localEventManager);
        _entity->addGroup(groupPlayer);
    }

    void onAttackStepEvent(Event& event) {
        auto& attackStepEvent = static_cast<AttackStepEvent&>(event);
        if (attackStepEvent.attackStep == "enemyAttackCheckComplete") {
            _entity->getComponent<AttackComponent>().endAttack();
        }
    }

    Entity& getEntity() { return *_entity; }
};
