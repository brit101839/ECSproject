#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "AttackComponent.h"

class AIContext {
private:
    TransformComponent* _transform;
    SpriteComponent* _sprite;
    AttackComponent* _attack;
    Vector2D _defaultPos;
    const TransformComponent& _playerTrans;

public:
    AIContext(TransformComponent* transform, SpriteComponent* sprite, AttackComponent* attack, const TransformComponent& playerTrans)
        : _transform(transform), _sprite(sprite), _attack(attack), _defaultPos(transform->position), _playerTrans(playerTrans) {
    }

    TransformComponent* getTransform() const { return _transform; }
    SpriteComponent* getSprite() const { return _sprite; }
    AttackComponent* getAttack() const { return _attack; }
    Vector2D getDefaultPos() const { return _defaultPos; }
    const Vector2D& getPlayerPos() const { return _playerTrans.position; }
};
