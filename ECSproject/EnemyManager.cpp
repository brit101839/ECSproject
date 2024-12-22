#include "EnemyManager.h"
#include "ECS/Components.h"

Entity& EnemyManager::addEnemy(const Vector2D& position, const char* spritePath)
{
    auto& enemy = _manager.addEntity();
    enemy.addcomponent<TransformComponent>(position, Vector2D(0.0f, 0.0f), 1.0f, 95.0f, 95.0f);
    enemy.addcomponent<SpriteComponent>(spritePath, false);
    auto& trans = enemy.getComponent<TransformComponent>();
    BoundingBox bound{ trans.position, 40.0f, 40.0f };
    enemy.addcomponent<ColliderComponent>("enemy", bound);
    enemy.addGroup(groupEnemies);
    _enemies.push_back(&enemy);
    
    return enemy;
}
