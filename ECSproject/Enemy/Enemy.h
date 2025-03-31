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

	/*TransformComponent* _transform;
	SpriteComponent* _sprite;*/
	// StatsComponent* _stats;
	DefenseComponent* _defense;

	
	EventSystem& _globalEventSys;

public:
	Enemy(std::string name, EventSystem& globalEventSys)
		: _name(name), _globalEventSys(globalEventSys)
	{
	}

	void init(Entity* entity) {
		_entity = entity;

		//_transform = &_entity->getComponent<TransformComponent>();
		//_sprite = &_entity->getComponent<SpriteComponent>();
		// _stats = &_entity->getComponent<StatsComponent>();
		_defense = &_entity->getComponent<DefenseComponent>();
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

	void UnderAttack(AttackCheckingEvent& atc) {
		_defense->UnderAttack(atc);
	}

	Entity* getEntity() { return _entity; }
};
