#include "EnemyManager.h"
#include "ECS/Components.h"

Entity& EnemyManager::addEnemy(const Vector2D& position, const char* spritePath)
{
    auto& enemy = _manager.addEntity();
    auto& trans = enemy.addcomponent<TransformComponent>(position, Vector2D(0.0f, 0.0f), 1.0f, 300.f, 300.f);
    auto& sprite = enemy.addcomponent<SpriteComponent>(spritePath, true, 96.f, 96.f);
    sprite.addAnimation("idle", Animation(0, 5, 10, true));
    sprite.setAnimate("idle");
    BoundingBox bound{ trans.position, 80.0f, 80.0f };
    enemy.addcomponent<ColliderComponent>("enemy", bound, Vector2D(0.f, -40.f));
    enemy.addGroup(groupEnemies);
    _enemies.push_back(&enemy);
    
    return enemy;
}
