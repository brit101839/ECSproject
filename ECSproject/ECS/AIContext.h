#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "AttackComponent.h"
#include "SkillComponent.h"

enum class AIPreferBehavior
{
    NormalAttackOnly, SkillOnly
};

class AIContext {
private:
    AIPreferBehavior _behavior;
    Entity* _entity;
    Vector2D _defaultPos;
    const TransformComponent& _playerTrans;

public:
    AIContext(Entity* entity, const TransformComponent& playerTrans, AIPreferBehavior behavior)
        : _entity(entity), _playerTrans(playerTrans), _behavior(behavior) {
        _defaultPos = _entity->getComponent<TransformComponent>().position;
    }

    const AIPreferBehavior& getBehavior() const { return _behavior; }
    TransformComponent* getTransform() const { return &_entity->getComponent<TransformComponent>(); }
    SpriteComponent* getSprite() const { return &_entity->getComponent<SpriteComponent>(); }
    AttackComponent* getAttack() const { return &_entity->getComponent<AttackComponent>(); }
    SkillCompnent* getSkill() const { return &_entity->getComponent<SkillCompnent>(); }
    Vector2D getDefaultPos() const { return _defaultPos; }
    const Vector2D& getPlayerPos() const { return _playerTrans.position; }
};
