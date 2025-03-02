#pragma once
#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "Skill.h"

class Enemy
{
private:
	Entity* _entity;
	Skill* _skill; // 使用策略模式的技能
	TransformComponent* _transform;
	StatsComponent* _stats;
	ColliderComponent* _collider;
	SpriteComponent* _sprite;

public:
	Enemy(Entity* entity, Skill* skill)
		: _entity(entity), _skill(skill)
	{
		_transform = &_entity->getComponent<TransformComponent>();
		_sprite = &_entity->getComponent<SpriteComponent>();
		_stats = &_entity->getComponent<StatsComponent>();
		_collider = &_entity->getComponent<ColliderComponent>();
	}

	// 敌人技能
	void useSkill() {
		if (_skill) {
			_skill->execute();
		}
	}

	// 可以随时更换技能
	void setSkill(Skill* newSkill) {
		_skill = newSkill;
	}

	bool checkAttack(AttackComponent& atc) {
		return Collision::AABB(atc.mboundingBox, _entity->getComponent<ColliderComponent>().boundingBox);
	}

	void handleAttack(AttackComponent& atc) {
		if (checkAttack(atc)) {
			int damage = atc.getDamage();
			takeDamage(damage);
			onAttack();
			atc.endAttack();
			std::cout << "attacking( " << getHealth() << "/ " << getMaxHealth() << ")" << std::endl;
		}
	}

	void takeDamage(int damage) {
		_stats->takeDamage(damage);
		if (_stats->getHealth() <= 0) {
			// _entity->destroy();
		}
	}

	void onAttack() {
		// 受击时的动画或效果
	}

	Entity* getEntity() { return _entity; }
	ColliderComponent& getCollider() { return *_collider; }
	int getHealth() const { return _stats->getHealth(); }
	int getMaxHealth() const { return _stats->getMaxHealth(); }
};
