#include "EnemyManager.h"
#include "ECS/Components.h"
#include "CollisionManager.h"

Entity& EnemyManager::addEnemy(const Vector2D& position, const char* spritePath)
{
    auto& enemy = _manager.addEntity();
    auto& trans = enemy.addcomponent<TransformComponent>(position, Vector2D(0.0f, 0.0f), 0.5f, 300.f, 300.f);
    auto& sprite = enemy.addcomponent<SpriteComponent>(spritePath, true, 96.f, 96.f);
    sprite.addAnimation("idle", Animation(0, 5, 10, true));
    sprite.addAnimation("walkR", Animation(1, 8, 10, false));
    sprite.addAnimation("walkL", Animation(1, 8, 10, true));
    sprite.addAnimation("attack_1", Animation(5, 6, 10, false, false));
    sprite.addAnimation("attack_2", Animation(3, 9, 10, false, false));
    sprite.addAnimation("attack_3", Animation(4, 5, 10, false, false));
    sprite.addAnimation("attack_4", Animation(6, 9, 10, false, false));
    sprite.setAnimate("idle");
    BoundingBox bound{ trans.position, 80.0f, 80.0f };
    enemy.addcomponent<ColliderComponent>(_colliderManager, "enemy", bound, Vector2D(0.f, -40.f));
    enemy.addcomponent<AIComponent>(_playerTrans);
    enemy.addcomponent<StatsComponent>(100, 10, 1);
    enemy.addcomponent<HealthBarComponent>();
    // enemy.addcomponent<AttackComponent>(this);
    enemy.addGroup(groupEnemies);
    _enemies.push_back(&enemy);
    
    return enemy;
}
