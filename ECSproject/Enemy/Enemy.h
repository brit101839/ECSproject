#pragma once
#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "../EventManager.h"
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

	void useSkill() {
		if (_skill) {
			_skill->execute();
		}
	}

	void setSkill(Skill* newSkill) {
		_skill = newSkill;
	}

	bool checkAttack(BoundingBox& box) {
		return Collision::AABB(box, _entity->getComponent<ColliderComponent>().boundingBox);
	}

	void handleAttack(AttackEvent& atc) {
		if (checkAttack(atc.boundingBox)) {
			int damage = atc.damage;
			takeDamage(damage);
			onAttack();
			// atc.endAttack();
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
		// takeDamage()
	}

	Entity* getEntity() { return _entity; }
	ColliderComponent& getCollider() { return *_collider; }
	int getHealth() const { return _stats->getHealth(); }
	int getMaxHealth() const { return _stats->getMaxHealth(); }
};
