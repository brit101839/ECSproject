#pragma once

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Animation.h"
#include "CollisionManager.h"
#include "EventSystem.h"
// #include "Enemy/EnemyManager.h"

class Player {
private:
    Entity* _entity;
    EventSystem& _globalEventManager;
    // EventSystem _localEventManager;
    std::shared_ptr<SpawnSystem> _spawnSys;
    std::string _name = "player";
    StatsComponent* _stats;

public:
    Player(Entity* entity, EventSystem& eventManager, std::shared_ptr<SpawnSystem> spawnSys)
        : _entity(entity), _globalEventManager(eventManager), _spawnSys(spawnSys)
    {
        _globalEventManager.subscribe<AttackStepEvent>([this](Event& event) {
            onAttackStepEvent(event); });
        _globalEventManager.subscribe<AttackEvent>([this](Event& event) {
            onAttackEvent(event); });
    }

    void componentSetting(CollisionManager* mCollisionM) {
        _entity->addcomponent<LocalEventComponent>();

        auto trans = _entity->addcomponent<TransformComponent>(Vector2D(4500.0f, -2340.0f), Vector2D(0.0f, 0.0f), 1.0f, 100.0f, 100.0f);
        
        auto& playerSprite = _entity->addcomponent<SpriteComponent>("D:/dependencies/resource/Dungeon/Adventurer Sprite Sheet v1.5.png", true, 32.f, 32.f);
        _entity->addcomponent<SkillCompnent>("fire ball", _spawnSys);
        
        playerSprite.addAnimation("idle", Animation(0, 13, 10, false));
        playerSprite.addAnimation("walkL", Animation(1, 8, 10, true));
        playerSprite.addAnimation("walkR", Animation(1, 8, 10, false));
        playerSprite.addAnimation("walkUp", Animation(11, 8, 10, false));
        playerSprite.addAnimation("attack_1", Animation(2, 8, 10, false, makeAttackDetail(2, 10)));
        playerSprite.addAnimation("attack_2", Animation(3, 8, 10, false, makeAttackDetail(1, 10)));
        playerSprite.addAnimation("attack_3", Animation(4, 10, 10, false, makeAttackDetail(3, 20)));
        playerSprite.setAnimate("idle");
        _entity->addcomponent<KeyboardController>();

        BoundingBox bound{ trans.position, 40.0f, 40.0f };
        _entity->addcomponent<ColliderComponent>(mCollisionM, "player", bound, Vector2D(0.f, -40.f));
        _entity->addcomponent<StatsComponent>(500, 10, 1);
        _entity->addcomponent<AttackComponent>(_name, _globalEventManager);
        _entity->addGroup(groupPlayer);

        _stats = &_entity->getComponent<StatsComponent>();
    }

    bool checkAttack(BoundingBox& box) {
        return Collision::AABB(box, _entity->getComponent<ColliderComponent>().boundingBox);
    }

    void takeDamage(int damage) {
        _stats->takeDamage(damage);
        if (_stats->getHealth() <= 0) {
            // _entity->destroy();
        }
    }

    void onHurt() {
        _entity->getComponent<SpriteComponent>().OnInjuried();
    }

    void handleHurtEvent(AttackEvent& atc) {
        std::cout << "checking attack by: "<< atc.attacker << std::endl;
        if (!_entity->hasComponent<ColliderComponent>()) {
            std::cout << "collider loss " << std::endl;
        }
        if (checkAttack(atc.boundingBox)) {
            int damage = atc.damage;
            takeDamage(damage);
            onHurt();
            // atc.endAttack();
            std::cout << atc.attacker << "attacking, "<< _name <<" ( " << getHealth() << "/ " << getMaxHealth() << ")" << std::endl;
        }
    }

    void onAttackEvent(Event& event) {
        auto& attackEvent = static_cast<AttackEvent&>(event);
        if (attackEvent.attacker != "player") {
            handleHurtEvent(attackEvent);
        }
    }

    void onAttackStepEvent(Event& event) {
        auto& attackStepEvent = static_cast<AttackStepEvent&>(event);
        if (attackStepEvent.attackStep == "enemyHurtCheckComplete") {
            _entity->getComponent<AttackComponent>().endAttack();
        }
    }

    std::shared_ptr<AttackDetail> makeAttackDetail(int attackFrame, int damage) {
        // AttackDetail atcD(attackFrame, damage);
        return std::make_shared<AttackDetail>(attackFrame, damage);
    }

    Entity& getEntity() { return *_entity; }
    int getHealth() const { return _stats->getHealth(); }
    int getMaxHealth() const { return _stats->getMaxHealth(); }
};
