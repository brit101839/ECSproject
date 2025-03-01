﻿#pragma once
#include "../ECS/ECS.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/StatsComponent.h"
#include "../ECS/ColliderComponent.h"
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

	void init() {
		_sprite->addAnimation("idle", Animation(0, 5, 10, true));
		_sprite->addAnimation("walkR", Animation(1, 8, 10, false));
		_sprite->addAnimation("walkL", Animation(1, 8, 10, true));
		_sprite->addAnimation("attack_1", Animation(5, 6, 10, false, false, AnimateState::Attacking));
		_sprite->addAnimation("attack_2", Animation(3, 9, 10, false, false, AnimateState::Attacking));
		_sprite->addAnimation("attack_3", Animation(4, 5, 10, false, false, AnimateState::Attacking));
		_sprite->addAnimation("attack_4", Animation(6, 9, 10, false, false, AnimateState::Attacking));
		_sprite->addAnimation("dying", Animation(9, 6, 5, false, false, AnimateState::Dying));
		_sprite->addAnimation("died", Animation(9, 6, 5, false, false, AnimateState::Died));
		_sprite->setAnimate("idle");
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

	void takeDamage(int damage) {
		_stats->takeDamage(damage);
		if (_stats->getHealth() <= 0) {
			_entity->destroy();
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
