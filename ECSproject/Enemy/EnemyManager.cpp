#include "EnemyManager.h"
#include "../ECS/Components.h"
#include "../CollisionManager.h"
#include "EnemyFactory.h"

Enemy* EnemyManager::addEnemy(std::string name, const Vector2D& position)
{
   /* Entity& enemyEntity = _manager.addEntity();
    auto& trans = enemyEntity.addcomponent<TransformComponent>(position, Vector2D(0.0f, 0.0f), 0.5f, 300.f, 300.f);
    auto& sprite = enemyEntity.addcomponent<SpriteComponent>(spritePath, true, 96.f, 96.f);*/
    
    // std::unique_ptr<Skill> defaultSkill = std::make_unique<MeleeAttack>();

    Entity* enemyEntity = EnemyFactory::createEnemyFromJson(_manager, name, position);

    BoundingBox bound{ position, 80.0f, 80.0f };
    enemyEntity->addcomponent<ColliderComponent>(_colliderManager, "enemy", bound, Vector2D(0.f, -40.f));
    enemyEntity->addcomponent<StatsComponent>(100, 10, 1);
    enemyEntity->addcomponent<HealthBarComponent>();
    // enemyEntity->addcomponent<SkillCompnent>("fire ball", _spawnSys);
    enemyEntity->addcomponent<AttackComponent>(name, _globalEventManager);
    enemyEntity->addcomponent<AIComponent>(_playerTrans);
    // enemy.addcomponent<AttackComponent>(this);
    enemyEntity->addGroup(groupEnemies);

    Enemy* enemy = new Enemy(name, _globalEventManager);
    enemy->init(enemyEntity);

    // enemy->init();

    _enemies.push_back(enemy);
    
    return enemy;
}
