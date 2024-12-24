#pragma once

#include "TransformComponent.h"
#include "SpriteComponent.h"

class AIContext {
public:
    virtual TransformComponent* getTransform() const = 0;
    virtual SpriteComponent* getSprite() const = 0;
    virtual Vector2D getDefaultPos() const = 0;
    virtual const Vector2D& getPlayerPos() const = 0;

    virtual ~AIContext() = default;

};