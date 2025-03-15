#include "EnemyManager.h"
#include "../ECS/Components.h"
#include "../CollisionManager.h"
#include "EnemyFactory.h"

Enemy* EnemyManager::addEnemy(std::string name, const Vector2D& position)
{
   /* Entity& enemyEntity = _manager.addEntity();
    auto& trans = enemyEntity.addcomponent<TransformComponent>(position, Vector2D(0.0f, 0.0f), 0.5f, 300.f, 300.f);
    auto& sprite = enemyEntity.addcomponent<SpriteComponent>(spritePath, true, 96.f, 96.f);*/
    
    Skill* defaultSkill = new MeleeAttack();
    Enemy* enemy = new Enemy(name, defaultSkill, _globalEventManager);

    Entity* enemyEntity = EnemyFactory::createEnemyFromJson(_manager, name, position, enemy->getLocalEventSys());

    BoundingBox bound{ position, 80.0f, 80.0f };
    enemyEntity->addcomponent<ColliderComponent>(_colliderManager, "enemy", bound, Vector2D(0.f, -40.f));
    enemyEntity->addcomponent<StatsComponent>(100, 10, 1);
    enemyEntity->addcomponent<HealthBarComponent>();
    // enemy.addcomponent<AttackComponent>(this);
    enemyEntity->addGroup(groupEnemies);

    enemy->init(enemyEntity, _playerTrans);

    // enemy->init();

    _enemies.push_back(enemy);
    
    return enemy;
}
