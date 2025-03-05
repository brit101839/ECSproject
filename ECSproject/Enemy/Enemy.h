#pragma once
#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "../ECS/AIComponent.h"
#include "../EventManager.h"
#include "Skill.h"

class Enemy
{
private:
	std::string _name;
	Entity* _entity;
	Skill* _skill; // 使用策略模式的技能

	TransformComponent* _transform;
	StatsComponent* _stats;
	ColliderComponent* _collider;
	SpriteComponent* _sprite;
	
	EventManager& _globalEventSys;
	EventManager _componentEventSys;

public:
	Enemy(std::string name, Skill* skill, EventManager& globalEventSys)
		: _name(name), _skill(skill), _globalEventSys(globalEventSys)
	{
	}

	void init(Entity* entity, TransformComponent& playerTrans) {
		_entity = entity;
		_entity->addcomponent<AttackComponent>(_name, _globalEventSys, _componentEventSys);
		_entity->addcomponent<AIComponent>(playerTrans);

		_transform = &_entity->getComponent<TransformComponent>();
		_sprite = &_entity->getComponent<SpriteComponent>();
		_stats = &_entity->getComponent<StatsComponent>();
		_collider = &_entity->getComponent<ColliderComponent>();
		// _entity->getComponent<AttackComponent>().mboundingBox = { _transform->position + Vector2D(40.f, 0.f), _transform->width, _transform->height };

		_globalEventSys.subscribe<AttackStepEvent>([this](Event& event) {
			onAttackStepEvent(event); });
	}

	SpriteComponent* addSpriteComponent(const char* path, bool isAnimated, GLfloat cutWidth, GLfloat cutHeight) {
		return &_entity->addcomponent<SpriteComponent>(path, isAnimated, cutWidth, cutHeight, &_componentEventSys);
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

	void handleHurt(AttackEvent& atc) {
		if (checkAttack(atc.boundingBox)) {
			int damage = atc.damage;
			takeDamage(damage);
			onHurt();
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

	void onHurt() {
		// takeDamage()
	}

	void onAttackStepEvent(Event& event) {
		auto& attackStepEvent = static_cast<AttackStepEvent&>(event);
		if (attackStepEvent.attackStep == "playerHurtCheckComplete") {
			_entity->getComponent<AttackComponent>().endAttack();
		}
	}

	Entity* getEntity() { return _entity; }
	ColliderComponent& getCollider() { return *_collider; }
	int getHealth() const { return _stats->getHealth(); }
	int getMaxHealth() const { return _stats->getMaxHealth(); }
	EventManager &getLocalEventSys() { return _componentEventSys; }
};
