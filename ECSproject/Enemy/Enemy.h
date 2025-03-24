#pragma once
#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "../ECS/AIComponent.h"
#include "../EventSystem.h"

class Enemy
{
private:
	std::string _name;
	Entity* _entity;

	TransformComponent* _transform;
	StatsComponent* _stats;
	ColliderComponent* _collider;
	SpriteComponent* _sprite;
	
	EventSystem& _globalEventSys;

public:
	Enemy(std::string name, EventSystem& globalEventSys)
		: _name(name), _globalEventSys(globalEventSys)
	{
	}

	void init(Entity* entity) {
		_entity = entity;

		_transform = &_entity->getComponent<TransformComponent>();
		_sprite = &_entity->getComponent<SpriteComponent>();
		_stats = &_entity->getComponent<StatsComponent>();
		_collider = &_entity->getComponent<ColliderComponent>();
		// _entity->getComponent<AttackComponent>().mboundingBox = { _transform->position + Vector2D(40.f, 0.f), _transform->width, _transform->height };

		_globalEventSys.subscribe<AttackStepEvent>([this](Event& event) {
			onAttackStepEvent(event); });
	}

	SpriteComponent* addSpriteComponent(const char* path, bool isAnimated, GLfloat cutWidth, GLfloat cutHeight) {
		return &_entity->addcomponent<SpriteComponent>(path, isAnimated, cutWidth, cutHeight);
	}

	void useSkill() {
		if (_entity->hasComponent<SkillCompnent>()){
			_entity->getComponent<SkillCompnent>().UseSkill();
		}
	}

	void setSkill(std::unique_ptr<Skill> newSkill) {
		if (_entity->hasComponent<SkillCompnent>()) {
			_entity->getComponent<SkillCompnent>().setSkill(std::move(newSkill));
		}
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
		_sprite->OnInjuried();
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
};
